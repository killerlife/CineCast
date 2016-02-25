#include "ExternCall.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
#if 0
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
#else
	FILE *fp = fopen("test.res", "rb");
	char buf[300];
	std::string m_strRes;
	if(fp > 0)
	{
		fread(buf, 300, 1, fp);
		m_strRes = buf;

	}
#endif

	return 0;
}
