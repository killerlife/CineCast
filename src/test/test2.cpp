#if 1
#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>
#include "../netcomm/BaseOperation.h"

#include <string>

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;

int main(int argc, char **argv)
{
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
	return 0;
}#else
#include <stdio.h>
int  main(void)
{
    FILE *fp;
    
    char buf[80];
    
    fp = popen("mke2fs /dev/sdb1", "r");
    fgets(buf, 80, fp);
    printf("%s", buf);
    fgets(buf, 80, fp);
    printf("%s", buf);
    fgets(buf, 80, fp);
    printf("%s", buf);
    fgets(buf, 80, fp);
    printf("%s", buf);
    pclose(fp);
    return 0;
}

#endif