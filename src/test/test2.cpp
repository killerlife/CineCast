#if 1
#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>
#include "../netcomm/BaseOperation.h"
#include "../netcomm/NetCommThread.h"
#include "../dvb/PATDataProcess.h"
#include "log/Log.h"

#include <string>

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;
ILog* gLog = NULL;
uint32 gDebugID = 201603024;
char *gMd5 = NULL;
NetCommThread *pNetComm = NULL;
PATDataThread* pPat = NULL;
std::vector<std::string> gRunPathList;

int main(int argc, char **argv)
{
	USB usb;
	usb.USB_Mount();
#if 0
	mke2fs e2fs;

//	if(e2fs.status() == brunt::thread_ready)
//		e2fs.start();
	e2fs.FormatDisk();
	while(1)
	{
		char *s = e2fs.GetOutput();
		if(s[0] == 1)
		{
		//if(!s.empty())
		    //printf("%s\n", s+1);
		    std::string s = e2fs.GetSOutput();
		    printf("%s", s.c_str());
		    s[0] = 0;
		}
		else if(s[0] == -1)
		    break;
	}
#endif
	return 0;
}
#else
#include <stdio.h>
int  main(void)
{
	USB usb;
	usb.USB_Mount();
    return 0;
}

#endif