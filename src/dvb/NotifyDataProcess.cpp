#include "NotifyDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
//#include <log/Log.h>
#include <syslog.h>

NotifyDataThread gNotify;
NotifyDataThread* CreateNotify()
{
	return &gNotify;
}

void ReleaseNotify(NotifyDataThread* pNotify)
{
}

NotifyDataThread::NotifyDataThread():m_status(0), bMatch(false), m_machineId(0), m_pid(0), m_filmId(0)
{
	m_pFilter = new Filter;
// 	pLog = CreateLog();
}

NotifyDataThread::~NotifyDataThread()
{
	Stop();
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
// 	ReleaseLog(pLog);
}

bool NotifyDataThread::Init(void *param1, void *param2)
{
	m_pid = *(uint16*)param1;
	m_machineId = *(uint32*)param2;
	return Start();
}

bool NotifyDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_pid, 0x91);
	m_status = RUN;
	return true;
}

bool NotifyDataThread::Stop()
{
	if(m_status == STOP)
		return true;
	m_status = STOP;
	m_pFilter->Stop();
	CActiveThread::stop();
	return true;
}

void NotifyDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
#endif

// 	pLog->Write(LOG_DVB, "[Notify Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[Notify Descriptor] Run");
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
			fp = fopen("notify", "rb");
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
    				//DPRINTF("[Notify Descriptor] Get Data\n");
				//do crc32 check
				uint16 len = getBits(m_buffer, 12, 12);
				uint32 crc = calc_crc32(m_buffer, len - 1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2)))) & 0xffffffff;

				if (crc == crc1)
				{
					uint8 *pdata = m_buffer+10;
					uint8 type = *pdata++;
					m_filmId = getBits(m_buffer + 5, 0, 32);
					if(type < 2)
					{
						bMatch = true;
						break;
					}
					bool bFind = false;
					pdata++;
					uint32 cnt = getBits(pdata, 0, 32);
					pdata += 4;
					if(m_machineId == 0)
					{
					    bFind = true;
					    bMatch = true;
					    DPRINTF("[Notify Descriptor] Match\n");
					    break;
					}
					for (uint32 i = 0; i < cnt; i++)
					{
						uint32 id = getBits(pdata, 0, 32);

						if (id == m_machineId)
						{
							bFind = true;
							if(type < 4)
							{
								bMatch = true;
								break;
							}
						}
						pdata += 5;
					}
					if (type >= 4 && bFind == false)
					{
						bMatch = true;
						break;
					}
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
			return;
		case IDLE:
			break;
		}
	}
}
