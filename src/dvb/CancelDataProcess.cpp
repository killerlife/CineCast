#include "CancelDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <log/Log.h>
#include <syslog.h>

CancelDataThread::CancelDataThread():m_bCancel(false), m_status(0), m_pid(0)
{
	m_pFilter = new Filter;
// 	pLog = CreateLog();
}

CancelDataThread::~CancelDataThread()
{
	DPRINTF("[CancelDataThread] Destroy Stop\n");
	Stop();

	DPRINTF("[CancelDataThread] Destroy Delete Filter\n");
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
// 	ReleaseLog(pLog);
}

bool CancelDataThread::Init(void *param1, void *param2)
{
	m_pid = *(uint16*)param1;
	return Start();
}

bool CancelDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_pid, 0x94);
	m_status = RUN;
	return true;
}

bool CancelDataThread::Stop()
{
	if(m_status == STOP)
		return true;
	m_status = STOP;
	m_pFilter->Stop();
	return true;
}

void CancelDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
#endif

	//pLog->Write(LOG_DVB, "[Cancel Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[Cancel Descriptor] Run");

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
			fp = fopen("cancel", "rb");
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
					m_bCancel = true;
				}
#ifdef DEBUG
				else
				{
					printf("crc no match\n");
				}
#endif
			}
			break;
		case STOP:
#ifdef DEBUG
			printf("stop cancel\n");
#endif
			return;
		case IDLE:
			break;
		}
	}
}

