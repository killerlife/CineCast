#include <log/Log.h>
#include <log/LogStruct.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>


int main(int argc, char **argv)
{
	ILog * plog = CreateLog();
	LOGDATETIME timeAfter, timeBefore;
	timeAfter.year = 2015;
	timeAfter.month = 11;
	timeAfter.day = 25;

	timeBefore.year = 2015;
	timeBefore.month = 11;
	timeBefore.day = 27;

	TLogQueryResultArray ra;

	plog->Query(LOG_TMS, &timeAfter, &timeBefore, ra);
	for(int i = 0; i < ra.size(); i++)
	{
		printf("%s %s", ra[i].time.c_str(), ra[i].text.c_str());
	}
	return 0;
}