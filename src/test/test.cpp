#include "../content/IContent.h"
#include "thread/activeThread/activeThreadManager_i.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>


int main(int argc, char **argv)
{
	vector<int> srcList;
	vector<InfoData> m_movieList;

	srcList.push_back(0);
	printf("update success\n");
	if(getIContentManager()->update(srcList)==0)
	{
		printf("update success\n");
		//wait update status
		while(1)
		{
			sleep(5);
			printf("check ready\n");
			if(getIContentManager()->isReady(0) == true)
			{
				m_movieList.clear();
				int nRet = getIContentManager()->getProgramFileList(0, 0, m_movieList);
				InfoData info;
				std::vector<InfoData>::iterator itor;
				for(itor = m_movieList.begin();itor!=m_movieList.end(); ++itor)
				{
					info = *itor;
					for (int i = 0; i < 15; i++)
					{
						printf("%s\n", info.pData[i].c_str());
					}
// 					printf("Name: %s\n", info.pData[1].c_str());
// 					printf("Format: %s\n", info.pData[4].c_str());
// 					printf("Length: %s\n", info.pData[6].c_str());
// 					printf("Resolution: %s\n", info.pData[7].c_str());
				}
			}
			else
				printf("not ready\n");
		}
	}
	else
		printf("programs update failed.\n");
	return 0;
}