#include "FinishDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
//#include <log/Log.h>

FinishDataThread gFinish;
FinishDataThread* CreateFinish()
{
	return &gFinish;
}

void ReleaseFinish(FinishDataThread* pFinish)
{

}

FinishDataThread::FinishDataThread():m_bFinish(false), m_status(0), m_pid(0)
{
	m_pFilter = new Filter;
	pDebugCmd = GetDebugCommand();
	//pLog = CreateLog();
}

FinishDataThread::~FinishDataThread()
{
	Stop();
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
// 	ReleaseLog(pLog);
}

bool FinishDataThread::Init(void *param1, void *param2)
{
	m_pid = *(uint16*)param1;
	return Start();
}

bool FinishDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_pid, 0x93);
	m_status = RUN;
	return true;
}

bool FinishDataThread::IsFinish()
{
	bool bRet = m_bFinish;
	m_bFinish = false;
	return bRet;
}

bool FinishDataThread::Stop()
{
	if(m_status == STOP)
		return true;
	m_status = STOP;
	m_pFilter->Stop();
	return true;
}

void FinishDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
#endif

// 	pLog->Write(LOG_DVB, "[Finish Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[Finish Descriptor] Run");

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
			fp = fopen("finish", "rb");
			if(fp <= 0)
			{
				printf("open file error\n");
				m_status = STOP;
				break;
			}
			count = fread(m_buffer, 1, 4096, fp);
			fclose(fp);
#else
			if (m_pFilter->ReadFilter(m_buffer, count))
#endif
			{
				//do crc32 check
				uint16 len = getBits(m_buffer, 12, 12);
				uint32 crc = calc_crc32(m_buffer, len - 1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2)))) & 0xffffffff;
				if (crc == crc1)
				{
					m_bFinish = true;
				}
#ifdef DEBUG
				else
				{
					printf("finish crc error\n");
				}
#endif
			}
			else
			{
#if 1
				if ((*pDebugCmd) == D_FINISH)
				{
					m_bFinish = true;
					*pDebugCmd = 0;
				}
#endif // 0
			}
			break;
		case STOP:
			return;
		case IDLE:
			break;
		}
	}
}

