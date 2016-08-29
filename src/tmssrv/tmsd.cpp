#include <unistd.h>
#include <stdio.h>
#include <netcomm/UiProtocal.h>
#include <log/Log.h>
#include "tmssrv.h"
#include "../netcomm/NetCommThread.h"
#include "../dvb/PATDataProcess.h"

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;
ILog* gLog;
char *gMd5 = NULL;
NetCommThread *pNetComm;
std::vector<std::string> gRunPathList;
PATDataThread* pPat;
bool gShutdownAfterFinish;

ITmsServer *tms = 0;

int main(int argc, char *argv[])
{
#if 0
	pid_t fpid;
	fpid = fork();
	if(fpid < 0)
		printf("[Theater Manager System Daemon] fork error\n");
	else if(fpid == 0)
	{
#endif
		gLog = CreateLog();
	tms = getITmsServer();
	tms->Start(TMSSRV_PORT);

	while(1)
		sleep(1);
#if 0
	}
#endif
	ReleaseLog(gLog);
	return 0;
}