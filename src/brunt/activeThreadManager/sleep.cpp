#ifdef  _WIN32
#include "windows.h"
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include "sleep.h"
#include <iostream>

namespace brunt
{

void sleep(unsigned int ms)
{
	#ifdef  _WIN32
	Sleep(ms);
	#else
	usleep(ms*1000);	
	#endif
}
unsigned int getTickCount()
{
#ifdef _WIN32
	unsigned int time=GetTickCount();
	
#else
	struct timeval	tv;
	gettimeofday(&tv, 0);
	unsigned int time= tv.tv_sec*1000 + tv.tv_usec/1000;
#endif
	return time;
}
}
