#include <unistd.h>
#include <stdio.h>
#include <netcomm/UiProtocal.h>
#include <log/Log.h>
#include "../netcomm/NetCommThread.h"
#include "ini.h"

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;
ILog* gLog;

using namespace brunt;

int main(int argc, char *argv[])
{
	gLog = CreateLog();

	NetCommThread nc;
	nc.Start();

	nc.StartRecvTask();
	nc.StartRoundRecv();
	while(1)
	{
		sleep(1);
		nc.HeartBreat();
	}
	ReleaseLog(gLog);
	return 0;
}