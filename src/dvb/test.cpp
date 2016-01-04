#include "SatelliteRecv.h"
#include "NotifyDataProcess.h"
#include "StartDataProcess.h"
#include "FinishDataProcess.h"
#include "CancelDataProcess.h"
#include "PATDataProcess.h"
#include "../netcomm/NetCommThread.h"

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

#include "ini.h"

using namespace brunt;

brunt::IThreadManager* m_pManager;

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;

NotifyDataThread *pNotify;
StartDataThread *pStart;
FinishDataThread *pFinish;
CancelDataThread *pCancel;
PATDataThread* pPat;
GuiServer* guiServer;
ILog* gLog;
NetCommThread *pNetComm;

static void handle_sigint(int sig)
{
	//usleep(10000);
	printf("stop 1\n");
	pNotify->Stop();
	printf("stop 2\n");
	pStart->Stop();
	printf("stop 3\n");
	pFinish->Stop();
	printf("stop 4\n");
	pCancel->Stop();
	printf("stop 5\n");
	pPat->Stop();
	printf("stop 6\n");	
	guiServer->Stop();
	printf("stop 7\n");	
	pNetComm->Stop();
	sleep(1);


#if 0
	printf("delete 1\n");
	delete pNotify;
	printf("delete 2\n");
	delete pStart;
	printf("delete 3\n");
	delete pFinish;
	printf("delete 5\n");
	delete pPat;
	printf("delete 4\n");
	delete pCancel;
	printf("delete 6\n");
	delete guiServer;
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
	bool bRoundCount = false;

	gLog = CreateLog();

	sprintf(m_log, "[System] CineCast Started.");
	gLog->Write(LOG_SYSTEM, m_log);

#ifdef CAPTURE
	m_pManager = createThreadManager();
#endif

	ITuner *pTuner = CreateTuner();
	
	SatelliteConfOperation sco;
	sco.ReadConfig();

	printf("%s %s %s %s %s\n", gTuner.strDelSys.c_str(),
	    gTuner.strFec.c_str(), gTuner.strModulation.c_str(),
	    gTuner.strRollOff.c_str(), gTuner.strPolVert.c_str());

	pTuner->SetTunerConf(gTuner);
	pTuner->Zapto(gTuner);

#ifdef CAPTURE
	pStart = new StartDataThread;
	pNotify = new NotifyDataThread;
	pFinish = new FinishDataThread;
	pCancel = new CancelDataThread;
	pPat = new PATDataThread;
	guiServer = new GuiServer;
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

	m_pManager->addThread(threadInfo); 
	m_pManager->addThread(threadInfo1);
 	m_pManager->addThread(threadInfo2);
	m_pManager->addThread(threadInfo3);
	m_pManager->addThread(threadInfo4);
	m_pManager->addThread(threadInfo5);

	printf("match:%X\n", pNotify->IsNotify());
 	printf("Finish:%X\n", pFinish->IsFinish());

	m_pManager->setParallelNum(0, 7);
	m_pManager->run();

	signal(SIGINT, handle_sigint);

	gRecv.nReceiveStatus = 0x0000 + 0x00; //IDLE

	while(1)
	{
		sleep(5);
		//If tuner configuration change, call Zapto.
		if (bTunerChange)
		{
			pTuner->Zapto(gTuner);
			bTunerChange = 0;
		}

		//Get satellite status and log it.
		gInfo = pTuner->GetTunerInfo();
		sprintf(m_log, "[Satellite] status=%02X, agc=%3u%%, snr=%3u%%, ber=%d, unc=%d, lock=%02X",
			gInfo.nStatus,
			gInfo.nAGC,
			gInfo.nSNR,
			gInfo.nBER,
			gInfo.nUNC,
			gInfo.nLock);
		gLog->Write(LOG_SYSTEM, m_log);

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

 		if(pStart->IsStart())
		{
			if(bRoundCount == false)
		{
				bRoundCount = true;
				gRecv.nReceiveStatus += 0x010000;
				if(gRecv.nReceiveStatus == 0x010000)
		{
					pNetComm->StartRecvTask();
		}
				pNetComm->StartRoundRecv();
				sprintf(m_log, "[DataReceiving] Round=%d %s FilmID=%04X",
					gRecv.nReceiveStatus >> 8,
			gRecv.strFilmName.c_str(),
					gRecv.nFileID);
				gLog->Write(LOG_SYSTEM, m_log);
			}
 			gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 0x01;
		}

		if(pFinish->IsFinish())
		{
			gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 2; //STOP
			gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 3; //STOP
			
			//This function process lost file, and send lost file to Network Center.
			pPat->GetLostSegment();
			//=============================

			gRecv.nLostSegment = pPat->LostSegment();
			gRecv.nReceiveLength = pPat->ReciveLength();
			gRecv.nReceiveSegment = pPat->ReciveSegment();
			gRecv.nTotalSegment = pPat->TotalSegment();
			if(bRoundCount)
			{
				bRoundCount = false;
// 				gRecv.nReceiveSegment = gRecv.nTotalSegment - gRecv.nLostSegment;
				sprintf(m_log, "[DataReceived] Status=%d, Round=%d, CRCErr=%d, TotalSeg=%d, LostSeg=%d, ReceivedSeg=%d, FilmID=%04X",
					gRecv.nReceiveStatus & 0x0000ffff,
					gRecv.nReceiveStatus >> 16,
					gRecv.nCrcErrorSegment,
					gRecv.nTotalSegment,
					gRecv.nLostSegment,
					gRecv.nReceiveSegment,
					gRecv.nFileID);
				gLog->Write(LOG_SYSTEM, m_log);
			}

			//TODO lost report
			//gRecv.nReceiveStatus = 5;
			//write_output();
			//break;
	}
	}
#endif
	return 0;
}