#include "NotifyDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
//#include <log/Log.h>
#include <syslog.h>

#if SIMULATOR
extern char SimDataBuf[10][4096];
extern int SimBufPos;
#endif

NotifyDataThread gNotify;
NotifyDataThread* CreateNotify()
{
	return &gNotify;
}

void ReleaseNotify(NotifyDataThread* pNotify)
{
}

uint32 gDebugID = 201603024;

NotifyDataThread::NotifyDataThread():m_status(0), bMatch(false), m_machineId(0), m_pid(0), m_filmId(0)
{
	m_pFilter = new Filter;
	pDebugCmd = GetDebugCommand();
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
#if SIMULATOR
extern void print_hex(char* buf, int len);
#endif

void NotifyDataThread::doit()
{
// 	pLog->Write(LOG_DVB, "[Notify Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[Notify Descriptor] Run");
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
// 					DPRINTF("NOTIFY\n");
				char *pos = &SimDataBuf[SimBufPos?(SimBufPos-1)%10:9][0];
				if((*(uint16*)pos) == 0x1ff)
				{
// 					DPRINTF("NOTIFY\n");
// 					print_hex(pos+2, 25);
					memcpy(m_buffer, pos + 2, getBits((uint8*)pos+2, 12, 12) + 10);
					*(uint16*)pos = 0x00;
					{
						uint16 len = getBits(m_buffer, 12, 12);
//  						DPRINTF("len %d\n", len);
						uint32 crc = calc_crc32(m_buffer, len - 1) & 0xffffffff;
						uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
							(*((m_buffer + len)) << 16) |
							(*((m_buffer + len + 1)) << 8) |
							(*((m_buffer + len + 2)))) & 0xffffffff;

// 						DPRINTF("CRC %08x %08x\n", crc, crc1);

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
// 								DPRINTF("[Notify Descriptor] Match\n");
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
// 										DPRINTF("[Notify Descriptor] TRUE\n");
										break;
									}
								}
								pdata += 5;
							}
							if (type >= 4 && bFind == false)
							{
								bMatch = true;
// 								DPRINTF("[Notify Descriptor] TRUE\n");
								break;
							}
						}
						else
							DPRINTF("Notify CRC error\n");
					}
				}
				else
					usleep(1000);
			}
			else
#endif
			if (m_pFilter->ReadFilter(m_buffer, count))
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
			}
			else
			{
#if SIMULATOR
				if((*pDebugCmd) == D_NOTIFY)
				{
					m_filmId = gDebugID;
					bMatch = true;
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
