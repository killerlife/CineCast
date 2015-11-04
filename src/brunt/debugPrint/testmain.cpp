#include <stdio.h>


#include "debugprint.h"

int main(int argc , char* argv[])
{
	char* p  = new char[100];
	snprintf(p, 100, "hello world");
	printf("debugprint testmain:\n");
	
	int i = DP_Fatal;
	char* DP_LEVEL_NAME[] = {
	"DP_All",			// µÈ¼¶×îµÍ£¬µ±ÉèÎªDP_ALLÊ±£¬ËùÓÐÀàÐÍµÄDebugPrintÈ«²¿Êä³ö¡£
	"DP_Info",
	"DP_Debug",
	"DP_Trace",
	"DP_Warning",
	"DP_Error",
	"DP_Fatal"			// µÈ¼¶×î¸ß£¬ 
	};
	CDebugPrint dprint(0, 0);
	dprint.Print(i, "testmain", "print a string directly.\n");
	while(i >=0 )	
	{
		dprint.Print(i, "testmain", "[%d] %s p=%p *p=%s", i, DP_LEVEL_NAME[i], p, p);
//		printf("%s\n", DP_LEVEL_NAME[i]);
		i--;
	}

	delete p;

	return 0;
}
