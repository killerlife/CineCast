#include "PATDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
//#include <log/Log.h>

PATDataThread::PATDataThread():m_status(0), m_PatId(0xfe), m_pManager(NULL)
{
	m_pFilter = new Filter;
	//pLog = CreateLog();
}

PATDataThread::~PATDataThread()
{
	Stop();
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
	//ReleaseLog(pLog);
}

bool PATDataThread::Init(void *param1, void *param2)
{
	m_PatId = *(uint16*)param1;
	return Start();
}

bool PATDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_PatId, 0x00);
	m_status = RUN;
	return true;
}

bool PATDataThread::Stop()
{
	m_status = STOP;
	m_pFilter->Stop();
#ifdef DEBUG
	printf("PAT stop\n");
#endif

	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		PMTDataThread *pmtThread = *itor;
		pmtThread->Stop();
		delete pmtThread;
	}
	m_pmtList.clear();
	m_pmtIdList.clear();

	return true;
}

void PATDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
	printf("start pat thread manager\n");
#endif
	
	//pLog->Write(LOG_DVB, "[PAT Descriptor] Run");
	//syslog(LOG_INFO|LOG_USER, "[PAT Descriptor] Run");

#if 1
	m_pManager = brunt::createThreadManager();
#endif // 0
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
			fp = fopen("pat", "rb");
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
				//DPRINTF("[PAT Descriptor] Get Data\n");
				//do crc32 check
				uint16 len = getBits(m_buffer, 12, 12);
				uint32 crc = calc_crc32(m_buffer, len -1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2))))  & 0xffffffff;
				if (crc == crc1)
				{
					uint8 *pbuf = m_buffer + 8;
					len -= 9;
					uint16 program_number;
					while(len > 0)
					{
						program_number = (pbuf[0] << 8) |
							pbuf[1];
						if (program_number != 0)
						{
							uint16 pmtId = ((pbuf[2] & 0x1f) << 8) |
								pbuf[3];
							std::list<uint16>::iterator itor;
							bool bFind = false;
							for (itor = m_pmtIdList.begin(); itor != m_pmtIdList.end(); ++itor)
							{
								uint16 id = *itor;
								if(id == pmtId)
								{
									bFind = true;
									break;
								}
							}
							if (!bFind)
							{
								m_pmtIdList.push_back(pmtId);
								
								char str[200];
								sprintf(str, "[PAT Descriptor] Create PMT Descriptor: %d", pmtId);
								//pLog->Write(LOG_DVB, str);
								//syslog(LOG_INFO|LOG_USER, "[PAT Descriptor] Create PMT Descriptor: %d", pmtId); 

								PMTDataThread *thread = new PMTDataThread;
								thread->Init(&pmtId, NULL);
#if 1
								brunt::ThreadRunPolicy policy;
								policy.order = 0;
								policy.policy = brunt::POLICY_ERROR_EXIT;

								brunt::CThreadRunInfo threadInfo(thread, policy);
								m_pManager->addThread(threadInfo); 
								m_pManager->setParallelNum(0, m_pmtIdList.size());
								m_pManager->run();
#endif
								m_pmtList.push_back(thread);
							}
						}
						pbuf += 4;
						len -= 4;
					}
				}
			}
			break;
		case STOP:
#if 1
			m_pManager->terminate();
			brunt::releaseThreadManager(m_pManager);
			m_pManager = NULL;
#endif // 0
			return;
		case IDLE:
			break;
		}
	}
}

uint64 PATDataThread::ReciveLength()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->ReciveLength();
	}
	return length;
}

uint64 PATDataThread::FileLength()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->FileLength();
	}
	return length;
}

uint64 PATDataThread::ReciveSegment()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->ReciveSegment();
	}
	return length;
}

uint64 PATDataThread::LostSegment()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->LostSegment();
	}
	return length;
}

uint64 PATDataThread::CRCError()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->CRCError();
	}
	return length;
}

uint64 PATDataThread::TotalSegment()
{
	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		length += (*itor)->TotalSegment();
	}
	return length;
}

void PATDataThread::GetLostSegment()
{
	std::list<PMTDataThread*>::iterator itor;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		(*itor)->GetLostSegment();
	}
}

