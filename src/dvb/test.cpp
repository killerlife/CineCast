#include "SatelliteRecv.h"
#include "NotifyDataProcess.h"
#include "StartDataProcess.h"
#include "FinishDataProcess.h"
#include "CancelDataProcess.h"
#include "PATDataProcess.h"
#include "../netcomm/NetCommThread.h"
#include "../netcomm/BaseOperation.h"
#if SIMULATOR
#include "../netcomm/SimulatorServer.h"
#endif
#include "thread/activeThread/activeThreadManager_i.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h> 
#include <unistd.h>
#include <netcomm/GuiServer.h>
#include <netcomm/UiProtocal.h>
#include <netcomm/BaseOperation.h>
#include "log/Log.h"
#include <vector>
#include <string>

#include "ini.h"

using namespace brunt;

brunt::IThreadManager* m_pManager;

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
std::vector<std::string> gRunPathList;

static void handle_sigint(int sig)
{
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
#if SIMULATOR
	ReleaseSimulatorServer(simServer);
#endif
	sleep(1);
#endif
	printf("release main\n");
	releaseThreadManager(m_pManager);

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

#define NETDEV_COUNT 2
#define ETH "/etc/sysconfig/network-scripts/ifcfg-enp6s"

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
	//====================================================================
	//Change work directory to "/storage"
	chdir("/storage");
	//====================================================================

	gLog = CreateLog();

	sprintf(m_log, "[System] CineCast Started.");
	gLog->Write(LOG_SYSTEM, m_log);

	m_pManager = createThreadManager();

	ITuner *pTuner = CreateTuner();
	
	SatelliteConfOperation sco;
	sco.ReadConfig();

	printf("%s %s %s %s %s\n", gTuner.strDelSys.c_str(),
	    gTuner.strFec.c_str(), gTuner.strModulation.c_str(),
	    gTuner.strRollOff.c_str(), gTuner.strPolVert.c_str());

	pTuner->SetTunerConf(gTuner);
	pTuner->Zapto(gTuner);

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

	guiServer->Init();
#if SIMULATOR
	simServer->Init();
#endif

	pNetComm->Init();
	pNetComm->Start();

	ThreadRunPolicy policy;
	policy.order = 0;
	policy.policy = POLICY_ERROR_EXIT;

	CThreadRunInfo threadInfo(pNotify, policy);
	CThreadRunInfo threadInfo1(pStart, policy);
 	CThreadRunInfo threadInfo2(pFinish, policy);
	CThreadRunInfo threadInfo3(pCancel, policy);
	CThreadRunInfo threadInfo4(pPat, policy);
	CThreadRunInfo threadInfo5(guiServer, policy);
#if SIMULATOR
	CThreadRunInfo threadInfo6(simServer, policy);
#endif
	m_pManager->addThread(threadInfo); 
	m_pManager->addThread(threadInfo1);
 	m_pManager->addThread(threadInfo2);
	m_pManager->addThread(threadInfo3);
	m_pManager->addThread(threadInfo4);
	m_pManager->addThread(threadInfo5);
#if SIMULATOR
	m_pManager->addThread(threadInfo6);
#endif
	printf("match:%X\n", pNotify->IsNotify());
 	printf("Finish:%X\n", pFinish->IsFinish());

	m_pManager->setParallelNum(0, 7);
	m_pManager->run();

	signal(SIGINT, handle_sigint);

	//------------------------------------------
	//Init status to IDLE
	gRecv.nReceiveStatus = 0x0000 + 0x00; //IDLE
	//------------------------------------------

	//--------------------------------------------
	//1.Mount removable disk and raid array
	//2. Check removable disk status and report to GUI
	//3. Check raid array status and report to GUI
	mke2fs fs;
	while(1)
	{
	bool bMountRD = fs.MountDisk(DISK_REMOVEABLE);
	bool bMountRaid = fs.MountDisk(DISK_RAID);

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
		if(bMountRaid == true && bMountRD == true)
			break;
	}
	//--------------------------------------------

	int nAutoDelCounter = 0;
	USB usb;
	while(1)
	{
		sleep(5);

		//------------------------------------------
		if(usb.USB_Mount())
		{
			if(usb.ReadyUpdate())
			{
				//////////////////////////////////////////////////////////////////////////
				// TODO: Tell UI the update file is ready!
				//////////////////////////////////////////////////////////////////////////
			}
		}
		//------------------------------------------

		//------------------------------------------
		//If tuner configuration change, call Zapto.
		if (bTunerChange)
		{
			pTuner->Zapto(gTuner);
			bTunerChange = 0;
		}
		//------------------------------------------

		//-------------------------------------------------------
		//Use counter to make sure start auto delete previous DCP
		//Timeout is 300 seconds.
		//After deleted, timer stop.
		//Delete files what it's not in RunPathList
		if(pPat->IsPmtReady() && nAutoDelCounter <= 60)
		{
			nAutoDelCounter++;
			if(nAutoDelCounter == 60)
			{
				ContentOperation co;
				co.AutoDelete(0, gRunPathList);
			}
		}
		//-------------------------------------------------------

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
			if (pNetComm->IsConnect())
				gRecv.strExtend = "REMOTE:1|";
			else
				gRecv.strExtend = "REMOTE:0|";
		}
		//----------------------------------------------------


		//----------------------------------------------------
		nRate = (gRecv.nReceiveLength - sRecv) * 8 / 1000 / 1000 / 5;
		sRecv = gRecv.nReceiveLength;
// 		if(nRate < 80)
// 			nRate = 80;
		if((gRecv.nReceiveStatus & 0xffff) == 1)
		{
			char ss[20];
			sprintf(ss, "RATE:%d|", nRate);
			gRecv.strExtend += ss;
		}
		//----------------------------------------------------


		//===========================================================
		//Process Notify from DVB
		if(pNotify->IsNotify())
		{
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
		}
		}
		//===========================================================


		//-----------------------------------------------------------
		//Process START from DVB
 		if(pStart->IsStart() && ((gRecv.nReceiveStatus & 0xffff) != 11))
		{
			pPat->Start();
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

					//Get task start time for heart beat
					if((gRecv.nReceiveStatus & 0xffff0000) == 0x010000)
		{
						if(pNetComm)
					pNetComm->StartRecvTask();
		}
					//Get Round start time for heart beat
					if(pNetComm)
				pNetComm->StartRoundRecv();
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
		}
		//------------START-----------------------------------------------



		//===================================================================================
		//Process FINISH from DVB
		if(pFinish->IsFinish() && ((gRecv.nReceiveStatus & 0xffff) != 11))
		{
			//-----------------------------------------------------------
			//Set status to data received and lost analysis
			if (!(gRecv.nReceiveSegment == gRecv.nTotalSegment && gRecv.nTotalSegment != 0))
			{
				printf("set state 2\n");
			gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 2; //STOP
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
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
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
					gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
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
				gRecv.nReceiveStatus  = (gRecv.nReceiveStatus & 0xffff0000) + 11;
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

			sprintf(m_log, "[CineCast] Received Cancel from satellite, restart PAT Thread.");
			gLog->Write(LOG_SYSTEM, m_log);
		}
		//-----------------------------------------------------------------------------------


		//===================================================================================
		//If task finished, reset PAT and round counter for next task.
		if((gRecv.nReceiveStatus & 0xffff) == 11)
		{
// 			gRecv.nReceiveStatus = 11;
			pPat->Reset();
			sprintf(m_log, "[CineCast] Task finished, restart PAT Thread.");
			gLog->Write(LOG_SYSTEM, m_log);
			if(gShutdownAfterFinish)
			{
				gLog->Write(LOG_SYSTEM, "[CineCast] Finish!!! Power-off...");
				system("/bin/sync");
				system("/sbin/init 0");
			}
			else
			{
				gLog->Write(LOG_SYSTEM, "[CineCast] Finish!!! Keep Power-on...");
			}
		}
		//===================================================================================

	}
	return 0;
}