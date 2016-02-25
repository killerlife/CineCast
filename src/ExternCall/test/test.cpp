// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

typedef unsigned long long uint64;
typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

class RaidDetailParser
{
public:
	void DetailParser(std::string strDetail);
	std::string GetRaidLevel(){ return strRaidLevel; };
	std::string GetState(){ return strState; };
	int GetRaidDevices(){ return nRaidDevices; };
	int GetActiveDevices(){ return nActiveDevices; };
	int GetWorkingDevices(){ return nWorkingDevices; };
	int GetFailedDevices(){ return nFailedDevice; };
	uint64 GetArraySize(){ return nArraySize; };
	uint64 GetUsedSize(){ return nUsedSize; };
	std::vector<std::string>& GetDevicesState(){ return strDevState; };

private:
	std::string strRaidLevel;
	std::string strState;
	int nRaidDevices;
	int nActiveDevices;
	int nWorkingDevices;
	int nFailedDevice;
	uint64 nArraySize;
	uint64 nUsedSize;
	std::vector<std::string> strDevState;
};

void RaidDetailParser::DetailParser(std::string strDetail)
{
	strRaidLevel = "";
	strState = "";
	nRaidDevices = nActiveDevices = nWorkingDevices = nFailedDevice = 0;
	nArraySize = nUsedSize = 0;
	strDevState.clear();
	size_t pos;
	if ((pos = strDetail.find("Raid Level")) != std::string::npos)
	{
		char buf[50];
		sscanf(strDetail.c_str() + pos + 13, "%s", buf);
		strRaidLevel = buf;
		if((pos = strDetail.find("State : ")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 8, "%s", buf);
			strState = buf;
		}
		if((pos = strDetail.find("Array Size")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 13, "%lld", &nArraySize);
		}
		if((pos = strDetail.find("Used Dev Size")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 16, "%lld", &nUsedSize);
		}
		if((pos = strDetail.find("Raid Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 15, "%d", &nRaidDevices);
		}
		if ((pos = strDetail.find("Active Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 17, "%d", &nActiveDevices);
		}
		if ((pos = strDetail.find("Working Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 18, "%d", &nWorkingDevices);
		}
		if ((pos = strDetail.find("Failed Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 17, "%d", &nFailedDevice);
		}
		if ((pos = strDetail.find("Number")) != std::string::npos)
		{
			int i = 0;
			for(i = 0;; i++)
			{
				if(strDetail.at(pos + i) == '\n')
					break;
			}
			i++;
			pos += i;
			for (i = 0; i < nRaidDevices; i++)
			{
				int n, major, minor, raid;
				char state1[20], state2[20], dev[40];
				int j = 0;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &n);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &major);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &minor);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &raid);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", state1);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", state2);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", dev);
				for(j = 0;; j++)
				{
					if(strDetail.at(pos + j) == '\n')
						break;
				}
				pos += (j+1);
				std::string sState = dev;
				sState += "|";
				sState +=state1;
				sState += " ";
				sState += state2;
				strDevState.push_back(sState);
			}
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp = fopen("r:/test.res", "rb");
	char buf[2048];
	std::string m_strRes;
	if(fp > 0)
	{
		fread(buf, 2048, 1, fp);
		m_strRes = buf;

	}
	RaidDetailParser rdp;
	rdp.DetailParser(m_strRes);

	return 0;
}

