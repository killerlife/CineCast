#include "ExternCall.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	IExternCall* pEC = CreateExternCall();
	pEC->RunCommand("mdadm -D /dev/md0");
	while(1)
	{
		if(pEC->IsFinish())
		{
			printf("%s\n", pEC->GetOutput().c_str());
			FILE *fp = fopen("test.res", "wb");
			if(fp > 0)
			{
				fwrite(pEC->GetOutput().c_str(), pEC->GetOutput().size(), 1, fp);
				fclose(fp);
			}
			break;
		}
	}
	return 0;
}
