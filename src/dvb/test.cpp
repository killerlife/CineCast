#include "SatelliteRecv.h"
#include "NotifyDataProcess.h"
#include "StartDataProcess.h"
#include "FinishDataProcess.h"
#include "CancelDataProcess.h"
#include "PATDataProcess.h"

#include "thread/activeThread/activeThreadManager_i.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>
#include <netcomm/GuiServer.h>

using namespace brunt;

brunt::IThreadManager* m_pManager;

TUNER_INFO gInfo;

NotifyDataThread *pNotify;
StartDataThread *pStart;
FinishDataThread *pFinish;
CancelDataThread *pCancel;
PATDataThread* pPat;
GuiServer* guiServer;

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
	guiServer->Stop();
	printf("stop 6\n");	
	sleep(1);

	printf("match:%X\n", pNotify->IsNotify());
#if 1	
	struct StartDescriptor* pSd = pStart->GetStartDescriptor();
	struct FileDescriptor* pFd = pStart->GetFileDescriptor();

	if (pSd != NULL)
	{
		printf("FilmID: %lX DCPLen:%lld \n", pSd->FilmId, pSd->DCPLength);
		printf("File: %s \n", pSd->FileName);
		printf("UUID:%s\n", pSd->UUID);
		printf("ISD:%s\n", pSd->IssueDate);
		printf("IS:%s\n", pSd->Issuer);
		printf("Creator:%s\n", pSd->Creator);
	}

	if (pFd != NULL)
	{
		printf("FL:%lld SL:%ld\n", pFd->FileLength, pFd->SegmentLength);
		printf("File:%s\n", pFd->FileName);
	}
#endif
	printf("Finish:%X\n", pFinish->IsFinish());
	printf("Cancel:%X\n", pCancel->IsCancel());

	printf("delete pNotify thread\n");

	printf("delete 1\n");
	delete pNotify;
	printf("delete 2\n");
	delete pStart;
	printf("delete 3\n");
	delete pFinish;
	printf("delete 4\n");
	delete pCancel;
	printf("delete 5\n");
	delete pPat;
	sleep(1);

	printf("release main\n");
	releaseThreadManager(m_pManager);

	exit(2);
} 

int main(int argc, char **argv)
{
#if 1
#ifdef CAPTURE
	m_pManager = createThreadManager();
#endif

	ITuner *pTuner = CreateTuner();
	TUNER_CONF conf;
	printf("%s %s %s %s %s\n", conf.strDelSys.c_str(),
	    conf.strFec.c_str(), conf.strModulation.c_str(),
	    conf.strRollOff.c_str(), conf.strPolVert.c_str());
	//conf.strModulation = "QAMAUTO";
	//conf.strFec = "auto";
	//conf.strRollOff = "auto";

	pTuner->SetTunerConf(conf);
	pTuner->Zapto(conf);

#ifdef CAPTURE
	pStart = new StartDataThread;
	pNotify = new NotifyDataThread;
	pFinish = new FinishDataThread;
	pCancel = new CancelDataThread;
	pPat = new PATDataThread;
	guiServer = new GuiServer;

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

	while(1)
	{
		sleep(5);
		#if 1
		gInfo = pTuner->GetTunerInfo();
		printf("status = %02X | agc = %3u%% | snr = %3u%% | ber = %d | unc = %d | lock = %02X\n",
			gInfo.nStatus, gInfo.nAGC, gInfo.nSNR, gInfo.nBER, gInfo.nUNC, gInfo.nLock);
		#endif
		printf("FileLength = %lld, RecvLength = %lld TotalSegment = %lld RecvSegment = %lld CRCError %lld\n",
		    pPat->FileLength(), pPat->ReciveLength(), pPat->TotalSegment(), pPat->ReciveSegment(), pPat->CRCError());
		if(pFinish->IsFinish())
		{
			pPat->GetLostSegment();
		}
	}
#endif
#else
	m_pManager = createThreadManager();

	ThreadRunPolicy policy;
	policy.order = 0;
	policy.policy = POLICY_ERROR_EXIT;

	CThreadRunInfo threadInfo(guiServer, policy);
	m_pManager->addThread(threadInfo); 
	m_pManager->setParallelNum(0, 1);
	m_pManager->run();

	while(1)
	{

	}

#endif
	return 0;
}