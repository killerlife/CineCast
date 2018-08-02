#include "SatelliteRecv.h"
#include "NotifyDataProcess.h"
#include "StartDataProcess.h"
#include "FinishDataProcess.h"
#include "CancelDataProcess.h"
#include "PATDataProcess.h"
#include "../netcomm/NetCommThread.h"
#include "../netcomm/BaseOperation.h"
#include "../netcomm/cfctms.h"
#include "../externcall/ExternCall.h"
#include "../netcomm/aureolam.h"

#if SIMULATOR
#include "../netcomm/SimulatorServer.h"
#endif
#include "thread/activeThread/activeThreadManager_i.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h> 
#include <unistd.h>
#include <time.h>
#include <netcomm/GuiServer.h>
#include <netcomm/UiProtocal.h>
#include <netcomm/BaseOperation.h>
#include "log/Log.h"
#include <vector>
#include <string>
#include "../content/IContentParser.h"

#include "ini.h"

using namespace brunt;

brunt::IThreadManager* m_pManager;
brunt::IThreadManager* m_pGuiMgr;

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;
char *gMd5 = NULL;
bool gShutdownAfterFinish = false;

// bool bRecvData = false;

NotifyDataThread *pNotify;
StartDataThread *pStart;
FinishDataThread *pFinish;
CancelDataThread *pCancel;
PATDataThread* pPat;
GuiServer* guiServer;
#if SIMULATOR
SimulatorServer* simServer;
#endif

ILog* gLog;
NetCommThread *pNetComm = NULL;
NetCommThread *pLeoNetComm = NULL;

#ifdef WANDA
NetCommThread *pWanDaNetComm = NULL;
#endif // WANDA

#ifdef AUREOLAM
AureoLamThread *pAureoLam = NULL;
#endif // AUREOLAM

bool bRemoteConnect = false;

std::vector<std::string> gRunPathList;

CfcTmsThread *pCfcTms = NULL;

static void handle_sigint(int sig)
{
	pStart->Cancel();
	pNotify->Cancel();
	pPat->Reset(false);
	pPat->Clear();

	//usleep(10000);
	pNotify->Stop();
	pStart->Stop();
	pFinish->Stop();
	pCancel->Stop();
	//pPat->Stop(); //Don't call stop, because it'll automatic call while class destroy.
	guiServer->Stop();
#if SIMULATOR
	simServer->Stop();
#endif
	pNetComm->Stop();
	pLeoNetComm->Stop();
#ifdef WANDA
	pWanDaNetComm->Stop();
#endif // WANDA
#ifdef AUREOLAM
	pAureoLam->Stop();
#endif // AUREOLAM
	pCfcTms->Stop();
	sleep(1);


#if 1
	printf("delete 1\n");
	ReleaseNotify(pNotify);
	printf("delete 2\n");
	ReleaseStart(pStart);
	printf("delete 3\n");
	ReleaseFinish(pFinish);
	printf("delete 5\n");
	delete pPat;
	printf("delete 4\n");
	ReleaseCancel(pCancel);
	printf("delete 6\n");
	ReleaseGuiServer(guiServer);
	
	ReleaseCfcTms(pCfcTms);

#if SIMULATOR
	ReleaseSimulatorServer(simServer);
#endif
	sleep(1);
#endif
	printf("release main\n");
	releaseThreadManager(m_pManager);
	releaseThreadManager(m_pGuiMgr);

	gLog->Write(LOG_SYSTEM, "[System] CineCast Stopped.");
	if (gLog)
	{
		ReleaseLog(gLog);
	}
	exit(2);
} 

static FILE *of = NULL;

static void write_output(void)
{
	if(of == NULL)
		of = fopen("./ReceiveOut.info", "wb");
	char buf[200];
	sprintf(buf, "FileLength = %lld\nRecvLength = %lld\nTotalSegment = %lld\nRecvSegment = %lld\nCRCError = %lld\nLostSegment = %lld\0",
		gRecv.nFileLength, gRecv.nReceiveLength, gRecv.nTotalSegment, gRecv.nReceiveSegment, gRecv.nCrcErrorSegment, gRecv.nLostSegment);
	fseek(of, 0, SEEK_SET);
	fwrite(buf, strlen(buf), 1, of);
	sprintf(buf, "Creator = %s\nFileName = %s\nIssueDate = %s\nIssuer = %s\n\0",
		gRecv.strCreator.c_str(),
		gRecv.strFilmName.c_str(),
		gRecv.strIssueDate.c_str(),
		gRecv.strIssuer.c_str());
	fwrite(buf, strlen(buf), 1, of);
	fflush(of);
}

// 返回系统开机后的秒数 [3/1/2018 jaontolt]
unsigned long GetTickCount()  
{  
	struct timespec ts;  

	clock_gettime(CLOCK_MONOTONIC, &ts);  

	return (ts.tv_sec);  
}  

#define NETDEV_COUNT 2
#define ETH "/etc/sysconfig/network-scripts/ifcfg-enp6s"

char strDemux[1024];

// 由5秒改为3秒 [3/15/2018 jaontolt]
#define TIMER_SEC	(3)

int main(int argc, char **argv)
{
	char m_log[512];
	volatile bool bRoundCount = false;
	uint32 nTimeOut = 70; //Use this for judge Transfer is finish
	bool *pBData = CreateFilmDataFlag();
	uint32 prevFilmID = 0;
	int64 sRecv = 0;
	int64 nRate = 0;
	int md5count = 0;
	int md5verifyCount = 0;
	
	bool bRegTms = false;
	bool bCancel = false;


	gLog = CreateLog();

	sprintf(m_log, "[System] CineCast Started.");
	gLog->Write(LOG_SYSTEM, m_log);

	//////////////////////////////////////////////////////////////////////////
	// Clean System first [2/8/2018 jaontolt]
	System sys;
	sys.ClearSystem();
	//////////////////////////////////////////////////////////////////////////

	m_pManager = createThreadManager();
	m_pGuiMgr = createThreadManager();

	ITuner *pTuner = CreateTuner();
	
	SatelliteConfOperation sco;
	sco.ReadConfig();

	printf("%s %s %s %s %s\n", gTuner.strDelSys.c_str(),
	    gTuner.strFec.c_str(), gTuner.strModulation.c_str(),
	    gTuner.strRollOff.c_str(), gTuner.strPolVert.c_str());

	pTuner->SetTunerConf(gTuner);
	pTuner->Zapto(gTuner);

	//////////////////////////////////////////////////////////////////////////
	// Reconstruct Demux device [10/11/2017 killerlife]
	memset(strDemux, 0, 1024);
	int pos;
	for(pos = gTuner.strDevName.size() - 1; pos >= 0; pos--)
	{
		if(gTuner.strDevName.at(pos) == '/')
			break;
	}
	for(int i = 0; i <= pos; i++)
		strDemux[i] = gTuner.strDevName.at(i);
	strcat(strDemux, "demux0");
	printf("DEMUX: %s\n", strDemux);
	if(gLog)
		gLog->Write(LOG_SYSTEM, strDemux);
	//////////////////////////////////////////////////////////////////////////

	pStart = CreateStart();
	pNotify = CreateNotify();
	pFinish = CreateFinish();
	pCancel = CreateCancel();
	pPat = new PATDataThread;
	guiServer = CreateGuiServer();
#if SIMULATOR
	simServer = CreateSimulatorServer();
#endif
	pNetComm = new NetCommThread;
	pLeoNetComm = new NetCommThread;
#ifdef WANDA
	pWanDaNetComm = new NetCommThread;
#endif // WANDA
#ifdef AUREOLAM
	pAureoLam = new AureoLamThread;
#endif // AUREOLAM
	pCfcTms = CreateCfcTms();

	//////////////////////////////////////////////////////////////////////////
	// Read CFCTMS Configuration [6/22/2017 killerlife]
	ICMyini* ini = createMyini();
	std::string tmp, url;
	int port;
	if(ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			if(ini->read(" ", "CFCTMS_IP", tmp))
				url = tmp;
			else
				url = "127.0.0.1";
			if(ini->read(" ", "CFCTMS_PORT", tmp))
				port = atoi(tmp.c_str());
			else
				port = 10021;
		}
		extern uint32 gMachineId;
		if (ini->load("/etc/CineCast/ID"))
		{
			if (ini->read("ID_HardKey", "ID", tmp))
			{
				gMachineId = atoi(tmp.c_str());
			}
			else
			{
				if(gLog)
					gLog->Write(LOG_SYSTEM, "[TEST] read MachineId failed");
				gMachineId = 0;
			}
		}
		releaseMyini(ini);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// GUI service must start first [3/1/2018 jaontolt]
	// It will notify GUI the core status
	ThreadRunPolicy policy;
	policy.order = 0;
	policy.policy = POLICY_ERROR_EXIT;

	CThreadRunInfo threadInfo5(guiServer, policy);
	m_pGuiMgr->addThread(threadInfo5);
	guiServer->Init();
	m_pGuiMgr->setParallelNum(0, 6);
	m_pGuiMgr->run();
	//////////////////////////////////////////////////////////////////////////

	USB usb;
	bool bUpdate, bRemote, bRate;
	bUpdate = bRemote = bRate = false;

	//////////////////////////////////////////////////////////////////////////
	// Mount disk before start receive data from satellite [2/8/2018 jaontolt]
	// Mount disk before autodelete film from SSD & RAID disk [2/8/2018 jaontolt]
	//1.Mount removable disk and raid array
	//2. Check removable disk status and report to GUI
	//3. Check raid array status and report to GUI
	mke2fs fs;
	while(1)
	{
	bool bMountRD = fs.MountDisk(DISK_REMOVEABLE);
	bool bMountRaid = fs.MountDisk(DISK_RAID);

		gRecv.strExtend = "";
		if(bMountRD == false)
		{
			gRecv.strExtend += "REMOVEABLEDISK:0|";
			sleep(5);
		}
		else
		{
			std::string tmp, s;
			char c;
			for(int i = 0; i < gRecv.strExtend.size(); i++)
			{
				c = gRecv.strExtend.at(i);
				if(c != '|')
				{
					tmp += c;
				}
				else
				{
					if(tmp != "REMOVEABLEDISK:0")
						s = s + tmp + "|";
					tmp.clear();
				}
			}
			gRecv.strExtend = s;
		}
		if(bMountRaid == false)
		{
			gRecv.strExtend += "RAIDDISK:0|";
			sleep(5);
		}
		else
		{
			std::string tmp, s;
			char c;
			for(int i = 0; i < gRecv.strExtend.size(); i++)
			{
				c = gRecv.strExtend.at(i);
				if(c != '|')
				{
					tmp += c;
				}
				else
				{
					if(tmp != "RAIDDISK:0")
						s = s + tmp + "|";
					tmp.clear();
				}
			}
			gRecv.strExtend = s;
		}
		printf("%s\n", gRecv.strExtend.c_str());
		if(bMountRaid == true && bMountRD == true)
			break;
		sleep(5);

		//////////////////////////////////////////////////////////////////////////
		// 在SSD挂载失败的环节中可以升级程序 [3/1/2018 jaontolt]
		//------------------------------------------
		if(usb.USB_Mount())
		{
			if(usb.ReadyUpdate())
			{
				//////////////////////////////////////////////////////////////////////////
				// TODO: Tell UI the update file is ready!
				//////////////////////////////////////////////////////////////////////////
				bUpdate = true;
			}
			else
				bUpdate = false;
		}
		if(bUpdate == true)
		{
			gRecv.strExtend += "UPDATE:1|";
		}
		//------------------------------------------
		//////////////////////////////////////////////////////////////////////////

	}
	//////////////////////////////////////////////////////////////////////////

	//====================================================================
	//Change work directory to "/storage"
	// Move code to here [3/1/2018 jaontolt]
	// If chdir startup, the system clean can't umount /storage, and mount SSD operation will not trust
	chdir("/storage");
	//====================================================================

	uint16 pid = 0x1ff;
	uint32 mid = 0;
	printf("init thread\n");
	pNotify->Init(&pid, &mid);

	pid = 0x2ff;
	pStart->Init(&pid, NULL);

	pid = 0x3ff;
	pFinish->Init(&pid, NULL);

	pid = 0x4ff;
	pCancel->Init(&pid, NULL);

	pid = 0xfe;
	pPat->Init(&pid, NULL);

	// Move to core start [3/1/2018 jaontolt]
// 	guiServer->Init();


#if SIMULATOR
	simServer->Init();
#endif
	
	pNetComm->Init();
	pNetComm->Start();
	pLeoNetComm->Init(0);
	pLeoNetComm->Start();
#ifdef WANDA
	pWanDaNetComm->Init(1);
	pWanDaNetComm->Start();
#endif // WANDA
#ifdef AUREOLAM
	pAureoLam->Init();
	pAureoLam->Start();
#endif // AUREOLAM
	
	//////////////////////////////////////////////////////////////////////////
	// Just for CFCTMS [6/22/2017 killerlife]
	pCfcTms->Init((char*)url.c_str(), port);
	pCfcTms->Start();
	//////////////////////////////////////////////////////////////////////////

	CThreadRunInfo threadInfo(pNotify, policy);
	CThreadRunInfo threadInfo1(pStart, policy);
 	CThreadRunInfo threadInfo2(pFinish, policy);
	CThreadRunInfo threadInfo3(pCancel, policy);
	CThreadRunInfo threadInfo4(pPat, policy);
#if SIMULATOR
	CThreadRunInfo threadInfo6(simServer, policy);
#endif
	m_pManager->addThread(threadInfo); 
	m_pManager->addThread(threadInfo1);
 	m_pManager->addThread(threadInfo2);
	m_pManager->addThread(threadInfo3);
	m_pManager->addThread(threadInfo4);
#if SIMULATOR
	m_pManager->addThread(threadInfo6);
#endif
	printf("match:%X\n", pNotify->IsNotify());
 	printf("Finish:%X\n", pFinish->IsFinish());

	m_pManager->setParallelNum(0, 6);
	m_pManager->run();

	signal(SIGINT, handle_sigint);

	//------------------------------------------
	//Init status to IDLE
	gRecv.nReceiveStatus = 0x0000 + 0x00; //IDLE
	//------------------------------------------

#if 0
//TEST ONLY
// 	pCfcTms->RegSatelliteDownloadTask();
// 	pCfcTms->UpdateSatelliteDownloadStatus(10,false);
	std::string buf = "\"dcpList\":[";
	char tt[4096];
	std::string cplPath = "/raid/216706_PowerRangers_FTR-2_S_CMN-QMS_CN_51_4K_LION_20170413_DTB_IOP_OV";
	std::string cplUuid;
	IContentParser * iCP = CreateContentParser();
	iCP->openCPL(cplPath);
	iCP->getCPLInfo(cplUuid);
									
	sprintf(tt, "{\"contentUUID\":\"%s\",\"dcpPath\":\"%s\"}", 
	    cplUuid.c_str(), cplPath.c_str());
	buf += tt;
        buf += "]";
        pCfcTms->finishSatelliteDownload(buf);
//---------------------------------------------------------
#endif

	//--------------------------------------------
	//////////////////////////////////////////////////////////////////////////
	//Auto delete file while service starting
	//1. Check if free size on SSD < 10G
	//2. Delete one DCP or directory while free size on SSD > 10G
	ContentOperation co;
	// CheckWhileFull has bug in endless loop, so disable it [2/9/2018 jaontolt]
	// add delete dead link [2/9/2018 jaontolt]
	co.DeleteDeadLink(0);
	co.CheckWhileFull(0);
	//////////////////////////////////////////////////////////////////////////

	long nAutoDelCounter = 0;
	long prevTick = GetTickCount();
	long currentTick;
	bool b5sec = false;
	bool bLog = false;
	bool bNotify = false;

	while(1)
	{
// 		sleep(5);

		//------------------------------------------
		// 非紧急的 5秒钟检查一次的处理流程 [3/1/2018 jaontolt]
		currentTick = GetTickCount();
		if(abs(currentTick - prevTick) >= TIMER_SEC) // 由5秒改为3秒 [3/15/2018 jaontolt]
		{
			prevTick = currentTick;
			b5sec = true; //有些地方不能放到开头处理，故设置此变量处理

			// 升级包的处理流程 [3/1/2018 jaontolt]
		if(usb.USB_Mount())
		{
			if(usb.ReadyUpdate())
			{
				//////////////////////////////////////////////////////////////////////////
				// TODO: Tell UI the update file is ready!
				//////////////////////////////////////////////////////////////////////////
					bUpdate = true;
			}
				else
					bUpdate = false;
		}

			// TUNER设置更新处理流程 [3/1/2018 jaontolt]
		//------------------------------------------
		//If tuner configuration change, call Zapto.
		if (bTunerChange)
		{
			pTuner->Zapto(gTuner);
			bTunerChange = 0;
		}
		//------------------------------------------

			// 取TUNER信息处理流程 [3/1/2018 jaontolt]
		//---------------------------------
		//Get satellite status and log it.
		gInfo = pTuner->GetTunerInfo();
#if 0
		sprintf(m_log, "[Satellite] status=%02X, agc=%3u%%, snr=%3u%%, ber=%d, unc=%d, lock=%02X",
			gInfo.nStatus,
			gInfo.nAGC,
			gInfo.nSNR,
			gInfo.nBER,
			gInfo.nUNC,
			gInfo.nLock);
		gLog->Write(LOG_SYSTEM, m_log);
#endif
		//--------------------------------

			// 取接收状态流程 [3/1/2018 jaontolt]
		//-----------------------------------------
		//Get receive status from DVB
		gRecv.nCrcErrorSegment = pPat->CRCError();
		gRecv.nFileID = pNotify->GetFilmId();
		gRecv.nFileLength = pPat->FileLength();
		gRecv.nLostSegment = pPat->LostSegment();
		gRecv.nReceiveLength = pPat->ReciveLength();
		gRecv.nReceiveSegment = pPat->ReciveSegment();
		gRecv.nTotalSegment = pPat->TotalSegment();
		gRecv.strCreator = pStart->GetCreator();
		gRecv.strIssuer = pStart->GetIssuer();
		gRecv.strUuid = pStart->GetUUID();
		gRecv.strFilmName = pStart->GetFilmName();
		gRecv.strIssueDate = pStart->GetIssueDate();
		if(gRecv.nLostSegment == 0 && gRecv.nReceiveSegment > gRecv.nTotalSegment)
		{
			gRecv.nReceiveSegment = gRecv.nTotalSegment;
		}
		//------------------------------------------
	
		//---------------------------------------------------
		//1. Check remote connection and report status to GUI
		if(pNetComm)
		{
				// 			if (pNetComm->IsConnect())
				// 				gRecv.strExtend = "REMOTE:1|";
				// 			else
				// 				gRecv.strExtend = "REMOTE:0|";
			if (pNetComm->IsConnect())
					bRemoteConnect = bRemote = true;
			else
					bRemoteConnect = bRemote = false;
		}
		//----------------------------------------------------


		//----------------------------------------------------
			nRate = (gRecv.nReceiveLength - sRecv) * 8 / 1000 / 1000 / TIMER_SEC; // 由5秒改为3秒 [3/15/2018 jaontolt]
		sRecv = gRecv.nReceiveLength;
// 		if(nRate < 80)
// 			nRate = 80;
		if((gRecv.nReceiveStatus & 0xffff) == 1)
		{
				bRate = true;
				// 			char ss[20];
				// 			sprintf(ss, "RATE:%d|", nRate);
				// 			gRecv.strExtend += ss;
		}
			else
				bRate = false;
		//----------------------------------------------------

		}
		//------------------------------------------


		//-------------------------------------------------------
		//Use counter to make sure start auto delete previous DCP
		//Timeout is 300 seconds.
		//After deleted, timer stop.
		//Delete files what it's not in RunPathList
		if(pPat->IsPmtReady() && nAutoDelCounter <= 60 && b5sec)
		{
			nAutoDelCounter++;
// 			printf("nAutoDelCounter %d, %lld %lld\n", nAutoDelCounter, gRecv.nReceiveSegment, gRecv.nTotalSegment);

			//////////////////////////////////////////////////////////////////////////
			// 用gRecv.nReceiveSegment !=gRecv.nTotalSegment 并gRecv.nTotalSegment != 0 用于判断在任务进行中时，进行自动删片 [1/19/2018 jaontolt]
			if(nAutoDelCounter == 60 && gRecv.nReceiveSegment != gRecv.nTotalSegment && gRecv.nTotalSegment != 0)
			{
				ContentOperation co;
// 				printf("Auto Delete\n");
				if(gLog)
					gLog->Write(LOG_SYSTEM, "Auto Delete in main()");
				co.AutoDelete(0, gRunPathList, gRecv.nFileLength);
			}
		}
		//-------------------------------------------------------


		//===========================================================
		//Process Notify from DVB
		if(pNotify->IsNotify())
		{
			DPRINTF("Is NOTIFY IN\n");
			if (prevFilmID != pNotify->GetFilmId())
		{
				//Reset Pat and another things
				sprintf(m_log, "[DataReceiving] Get difference FilmID %d %d, may be a new DCP.", prevFilmID, pNotify->GetFilmId());
				gLog->Write(LOG_SYSTEM, m_log);
				prevFilmID = pNotify->GetFilmId();
				pPat->Reset(false);
				gRecv.nReceiveStatus = 0;
				bRoundCount = false;

				//Reset Auto delete counter
				nAutoDelCounter = 0;

				//Reset MD5 Counter
				md5count = 0;
				md5verifyCount = 0;

				//--------------------------------------------
				//Clear Run Path List
				gRunPathList.clear();
				//--------------------------------------------

				//--------------------------------------------
				// Make sure register satellite task to TMS [6/13/2017 killerlife]
				bRegTms = true;
				bCancel = false;
				//--------------------------------------------

				//////////////////////////////////////////////////////////////////////////
				// Make sure the start processing be after notify processing [3/16/2018 jaontolt]
				bNotify = true;
				//////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				//Set task start time for heart beat
				if(pNetComm)
				{
					pNetComm->StartRecvTask();
					pNetComm->StartRoundRecv();
				}
				if(pLeoNetComm)
				{
					pLeoNetComm->StartRecvTask();
					pLeoNetComm->StartRoundRecv();
		}
#ifdef WANDA
				if(pWanDaNetComm)
				{
					pWanDaNetComm->StartRecvTask();
					pWanDaNetComm->StartRoundRecv();
				}
#endif // WANDA
#ifdef AUREOLAM
				if (pAureoLam)
				{
					pAureoLam->StartRecvTask();
					pAureoLam->StartRoundRecv();
				}
#endif // AUREOLAM
				//////////////////////////////////////////////////////////////////////////
			}
			DPRINTF("Is NOTIFY OUT\n");
		}
		//===========================================================


		//-----------------------------------------------------------
		//Process START from DVB
 		if(pStart->IsStart() && ((gRecv.nReceiveStatus & 0xffff) != 11) && bNotify)
		{
			DPRINTF("Is START IN\n");
			pPat->Start();
			bLog = false;
			//-------------------------------------------------
			//Add the round counter and write to log
			if(bRoundCount == false)
		{
				//Add counter while both PAT and PMT are ready
				if(pPat->IsPmtReady() && pPat->IsPat())
				    {
					gRecv.nReceiveStatus += 0x010000;
				bRoundCount = true;
					
					sprintf(m_log, "+++++[DataReceiving] Round=%d %X",
					gRecv.nReceiveStatus >> 16,
					bRoundCount);
					gLog->Write(LOG_SYSTEM, m_log);

					//Set task start time for heart beat
					if((gRecv.nReceiveStatus & 0xffff0000) == 0x010000)
		{
						if(pNetComm)
					pNetComm->StartRecvTask();
#ifdef WANDA
						if(pWanDaNetComm)
							pWanDaNetComm->StartRecvTask();
#endif // WANDA
#ifdef AUREOLAM
						if(pAureoLam)
							pAureoLam->StartRecvTask();
#endif // AUREOLAM
		}
					//Set Round start time for heart beat
					if(pNetComm)
				pNetComm->StartRoundRecv();
					if(pLeoNetComm)
						pLeoNetComm->StartRoundRecv();
#ifdef WANDA
					if(pWanDaNetComm)
						pWanDaNetComm->StartRoundRecv();
#endif // WANDA
#ifdef AUREOLAM
					if(pAureoLam)
						pAureoLam->StartRoundRecv();
#endif // AUREOLAM

					//////////////////////////////////////////////////////////////////////////
					// Register satellite task to TMS [6/13/2017 killerlife]
					if(pCfcTms && bRegTms)
					{
						bRegTms = false;
						pCfcTms->RegSatelliteDownloadTask();
					}
					//////////////////////////////////////////////////////////////////////////
				    }
				
				sprintf(m_log, "[DataReceiving] Round=%d %s FilmID=%04X Round flag:%X",
					gRecv.nReceiveStatus >> 16,
			gRecv.strFilmName.c_str(),
					gRecv.nFileID,
					bRoundCount);
				gLog->Write(LOG_SYSTEM, m_log);
			}
			//-------------------------------------------------


			//=================================================
			//Set status to data receive
			if (!(gRecv.nReceiveSegment == gRecv.nTotalSegment && gRecv.nTotalSegment != 0))
			{
				gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 1;
			}
			//=================================================
			DPRINTF("Is START OUT\n");
		}
		//------------START-----------------------------------------------



		//===================================================================================
		//Process FINISH from DVB
		if(pFinish->IsFinish() && ((gRecv.nReceiveStatus & 0xffff) != 11))
		{
			DPRINTF("Is FINISH IN\n");
			//-----------------------------------------------------------
			//Set status to data received and lost analysis
			if (!(gRecv.nReceiveSegment == gRecv.nTotalSegment && gRecv.nTotalSegment != 0))
			{
				//////////////////////////////////////////////////////////////////////////
				// It change status to FINISH only in IDLE & RECEIVE status [5/30/2018 jaontolt]
				if((gRecv.nReceiveStatus & 0xffff) < 2)
				{
				printf("set state 2\n");
			gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 2; //STOP
			}
				//////////////////////////////////////////////////////////////////////////
			}
			//-----------------------------------------------------------
			
			//-----------------------------------------------------------
			//This function process lost file, and send lost file to Network Center.
			pPat->GetLostSegment();
			//-----------------------------------------------------------

			//-----------------------------------------------------------
			//Update status from DVB
			gRecv.nLostSegment = pPat->LostSegment();
			gRecv.nReceiveLength = pPat->ReciveLength();
			gRecv.nReceiveSegment = pPat->ReciveSegment();
			gRecv.nTotalSegment = pPat->TotalSegment();
			//-----------------------------------------------------------

			if(bRoundCount)
			{
				sprintf(m_log, "[DataReceived] Status=%d, Round=%d, CRCErr=%d, TotalSeg=%d, LostSeg=%d, ReceivedSeg=%d, FilmID=%04X",
					gRecv.nReceiveStatus & 0x0000ffff,
					gRecv.nReceiveStatus >> 16,
					gRecv.nCrcErrorSegment,
					gRecv.nTotalSegment,
					gRecv.nLostSegment,
					gRecv.nReceiveSegment,
					gRecv.nFileID);
				gLog->Write(LOG_SYSTEM, m_log);

				//-----------------------------------------------------------
				//If  full received DCP
				//1. Send lost file to remote center
				//2. Check and unzip subtitle zip file
				if (gRecv.nReceiveSegment == gRecv.nTotalSegment && gRecv.nTotalSegment != 0)
				{
					sprintf(m_log, "[DataReceived] Send lost Report while no lost.");
					gLog->Write(LOG_SYSTEM, m_log);

					//Send Lost Report
					gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 3; //STOP
					pPat->SendLostReport();
					
					sprintf(m_log, "[DataReceived] unzip ZM file");
					gLog->Write(LOG_SYSTEM, m_log);
					gRecv.nReceiveLength = gRecv.nFileLength;

					//Unzip subtitle zip file
					pPat->UnzipSubtitle();

					//Clear PAT status and enable round counter
				pPat->IsPat();
				bRoundCount = false;
// 					bRecvData = false;
				}
				//If not a full received DCP, send lost file to remote center
				else if(gRecv.nTotalSegment != 0)
				{
					sprintf(m_log, "[DataReceived] Send lost Report while lost.");
					gLog->Write(LOG_SYSTEM, m_log);
					gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 3; //STOP
					pPat->SendLostReport();
				pPat->IsPat();
				bRoundCount = false;
// 					bRecvData = false;
				}
				//-----------------------------------------------------------
			}
			DPRINTF("Is FINISH OUT\n");
			}
		//=================FINISH============================================================



		//-----------------------------------------------------------------------------------
		//If full received DCP, get md5 file from remote center
		if ((gRecv.nReceiveSegment == gRecv.nTotalSegment) && (gRecv.nTotalSegment != 0) && (gRecv.nReceiveStatus & 0x0000ffff) > 2)
		{
//			pPat->Reset();
			//Do Md5 Request
// 			printf("%d\n", gRecv.nReceiveStatus & 0xffff);
			if((gRecv.nReceiveStatus & 0xffff) < 8)
			{
				sprintf(m_log, "[DataReceived] Get MD5 while no lost outside finish.");
				gLog->Write(LOG_SYSTEM, m_log);
				if(pNetComm)
					pNetComm->GetMD5File(gRecv.nFileID);
				//////////////////////////////////////////////////////////////////////////
				//MD5 Counter
				if(md5count < 3)
				{
					md5count++;
				}
				else
				{
					gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) | 0x08;
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}
		//-----------------------------------------------------------------------------------

		//===================================================================================
		//If received md5 file
		//1. Verify md5
		//2. Update status
		if ((gRecv.nReceiveStatus & 0xffff) == 0x08)
		{
			//Received MD5File
			//Do md5 parser and decrypt
			Md5Class* pMd5 = CreateMd5Class();
			if(gMd5)
// 			if(0)	//用于测试取不到MD5文件重试3次逻辑
			{
				sprintf(m_log, "[DataReceived] MD5 Parser outside finish.");
				gLog->Write(LOG_SYSTEM, m_log);
				pMd5->Md5Parser(gMd5);
				gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 9;
				if(pMd5->Md5Verify(gRecv.nFileID))
				{
					sprintf(m_log, "[DataReceived] MD5 verify success outside finish.");
					gLog->Write(LOG_SYSTEM, m_log);
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 10;
					//=============================
					//Do know why, maybe remove this
					//This function process lost file, and send lost file to Network Center.
					pPat->GetLostSegment();
					//=============================
#ifdef ENABLE_RAID
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 20;
#else
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
#endif // ENABLE_RAID
				}
				else
				{
					sprintf(m_log, "[DataReceived] MD5 verify error outside finish.");
					gLog->Write(LOG_SYSTEM, m_log);
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 8;
					//=============================
					//Do know why, maybe remove this
					pPat->GetLostSegment();
					//=============================
#ifdef ENABLE_RAID
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 20;
#else
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
#endif // ENABLE_RAID
				}
				if(pNetComm)
					pNetComm->DecryptRep();
			}
			else if(md5verifyCount < 3)
			{
				md5count = 0;
				md5verifyCount++;
				gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 7;
			}
			else
			{
				md5count = md5verifyCount = 0;
#ifdef ENABLE_RAID
				gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 20;
#else
				gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
#endif // ENABLE_RAID
			}
		}
		//===================================================================================

		//-----------------------------------------------------------------------------------
		//Process CANCEL from DVB
		if(pCancel->IsCancel())
		{
			prevFilmID = 0;
			gRecv.nReceiveStatus  = 0;
			gRecv.nCrcErrorSegment = 0;
			gRecv.nFileID = 0;
			gRecv.nFileLength = 0;
			gRecv.nLostSegment = 0;
			gRecv.nReceiveLength = 0;
			gRecv.nReceiveSegment = 0;
			gRecv.nTotalSegment = 0;

			pStart->Cancel();
			pNotify->Cancel();

			gRecv.strCreator = "";
			gRecv.strIssuer = "";
			gRecv.strUuid = "";
			gRecv.strFilmName = "";
			gRecv.strIssueDate = "";
			bRoundCount = false;
			
			pCancel->ClearCancel();
			pPat->Reset(false);
			pPat->Clear();

			//////////////////////////////////////////////////////////////////////////
			// Reset Task Time and Round Time to zero [5/28/2018 jaontolt]
			if(pNetComm)
			{
				pNetComm->ResetRecvTask();
				pNetComm->ResetRoundRecv();
			}
			if(pLeoNetComm)
			{
				pLeoNetComm->ResetRecvTask();
				pLeoNetComm->ResetRoundRecv();
			}
#ifdef WANDA
			if(pWanDaNetComm)
			{
				pWanDaNetComm->ResetRecvTask();
				pWanDaNetComm->ResetRoundRecv();
			}
#endif // WANDA
#ifdef AUREOLAM
			if (pAureoLam)
			{
				pAureoLam->ResetRecvTask();
				pAureoLam->ResetRoundRecv();
			}
#endif // AUREOLAM
			//////////////////////////////////////////////////////////////////////////

			sprintf(m_log, "[CineCast] Received Cancel from satellite, restart PAT Thread.");
			if(!bLog)
			{
			gLog->Write(LOG_SYSTEM, m_log);
				bLog = true;
			}

			//////////////////////////////////////////////////////////////////////////
			// Update satellite status to TMS [6/13/2017 killerlife]
			bCancel = true;
			//////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////////////////////////
			// To make sure the start processing be after notify processing [3/16/2018 jaontolt]
			bNotify = false;
			//////////////////////////////////////////////////////////////////////////
		}
		//-----------------------------------------------------------------------------------


		//===================================================================================
		//If task finished, reset PAT and round counter for next task.
#ifdef ENABLE_RAID
		if((gRecv.nReceiveStatus & 0xffff) == 20)
#else
		if((gRecv.nReceiveStatus & 0xffff) == 11)
#endif
		{
// 			gRecv.nReceiveStatus = 11;
			pPat->Reset();

			//////////////////////////////////////////////////////////////////////////
			// To make sure the start processing be after notify processing [3/16/2018 jaontolt]
			bNotify = false;
			//////////////////////////////////////////////////////////////////////////

			sprintf(m_log, "[CineCast] Task finished, restart PAT Thread.");
			if(!bLog)
			gLog->Write(LOG_SYSTEM, m_log);
			if(gShutdownAfterFinish)
			{
				if(!bLog)
				gLog->Write(LOG_SYSTEM, "[CineCast] Finish!!! Power-off...");
#if 0
				system("/bin/sync");
				system("/sbin/init 0");
#else
				IExternCall *pEc = CreateExternCall();
				pEc->RunCommand("/bin/sync");
				while(!pEc->IsFinish())
				{
					usleep(200000);
				}
				pEc->RunCommand("/sbin/init 0");
				while(!pEc->IsFinish())
				{
					usleep(200000);
				}
#endif
			}
			else
			{
				if(!bLog)
				gLog->Write(LOG_SYSTEM, "[CineCast] Finish!!! Keep Power-on...");
			}
			bLog = true;

			//////////////////////////////////////////////////////////////////////////
			// Reset Task Time and Round Time to zero [5/28/2018 jaontolt]
			if(pNetComm)
			{
				pNetComm->ResetRecvTask();
				pNetComm->ResetRoundRecv();
			}
			if(pLeoNetComm)
			{
				pLeoNetComm->ResetRecvTask();
				pLeoNetComm->ResetRoundRecv();
			}
#ifdef WANDA
			if(pWanDaNetComm)
			{
				pWanDaNetComm->ResetRecvTask();
				pWanDaNetComm->ResetRoundRecv();
			}
#endif // WANDA
#ifdef AUREOLAM
			if (pAureoLam)
			{
				pAureoLam->ResetRecvTask();
				pAureoLam->ResetRoundRecv();
			}
#endif // AUREOLAM
			//////////////////////////////////////////////////////////////////////////
		}
		//===================================================================================

		if(b5sec)
		{
			//////////////////////////////////////////////////////////////////////////
			// Update satellite status to TMS [6/13/2017 killerlife]
			if(pCfcTms && ((gRecv.nReceiveStatus & 0xffff) != 0))
			{
				pCfcTms->UpdateSatelliteDownloadStatus(nRate, bCancel);
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			// Update Status to UI
			gRecv.strExtend = "";
			if(bUpdate)
				gRecv.strExtend += "UPDATE:1|";
			if(bRemote)
				gRecv.strExtend += "REMOTE:1|";
			else
				gRecv.strExtend += "REMOTE:0|";
			if(bRate)
			{
				char ss[20];
				sprintf(ss, "RATE:%d|", nRate);
				gRecv.strExtend += ss;
			}
			//////////////////////////////////////////////////////////////////////////
		}

		b5sec = false;
	}
	return 0;
}