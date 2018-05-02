#include "CancelDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <log/Log.h>
#include <syslog.h>

CancelDataThread gCancel;
#if SIMULATOR
extern char SimDataBuf[10][4096];
extern int SimBufPos;
#endif
extern char strDemux[1024];

CancelDataThread* CreateCancel()
{
	return &gCancel;
}

void ReleaseCancel(CancelDataThread* pCancel)
{
}

CancelDataThread::CancelDataThread():m_bCancel(false), m_status(0), m_pid(0)
{
	m_pFilter = new Filter;
	pDebugCmd = GetDebugCommand();
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
// 	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetStrDevName(strDemux);
	m_pFilter->SetFilterID(m_pid, 0x94);
	m_status = RUN;
	m_bCancel = false;
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
	//pLog->Write(LOG_DVB, "[Cancel Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[Cancel Descriptor] Run");

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#if SIMULATOR
			if((*pDebugCmd) == D_SIMULATOR)
			{
				char *pos = &SimDataBuf[SimBufPos?(SimBufPos-1)%10:9][0];
				uint16 len = getBits(m_buffer, 12, 12);
				if((*(uint16*)pos) == 0x4ff)
				{
					memcpy(m_buffer, pos + 2, len + 10);
					*(uint16*)pos = 0x00;
					{
						//do crc32 check
						uint32 crc = calc_crc32(m_buffer, len - 1) & 0xffffffff;
						uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
							(*((m_buffer + len)) << 16) |
							(*((m_buffer + len + 1)) << 8) |
							(*((m_buffer + len + 2)))) & 0xffffffff;
// 						DPRINTF("Cancel\n");
						*pDebugCmd = 0;
						m_bCancel = true;
						if (crc == crc1)
						{
							m_bCancel = true;
						}
						else
						{
							DPRINTF("Cancel CRC error\n");
						}
					}
				}
				else
					usleep(1000);
			}
else
#endif
			if (m_pFilter->ReadFilter(m_buffer, count))
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
			}
			else
			{
#if SIMULATOR
				//--------------------------------------------------------
				//                Network Simulator
				if ((*pDebugCmd) == D_CANCEL)
				{
					m_bCancel = true;
					*pDebugCmd = 0;
				}
				//--------------------------------------------------------
#endif
			}
			break;
		case STOP:
			return;
		case IDLE:
			break;
		}
	}
}

void CancelDataThread::ClearCancel()
{
    m_bCancel = false;
}
