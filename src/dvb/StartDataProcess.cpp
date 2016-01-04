#include "StartDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
//#include <log/Log.h>
#include <syslog.h>

StartDataThread::StartDataThread():m_status(0), m_pid(0), bStart(false)
{
	m_pStartDescriptor = NULL;
	m_pFileDescriptor = NULL;
	m_pFilter = new Filter;
	//pLog = CreateLog();
}

StartDataThread::~StartDataThread()
{
	Stop();
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
	if (m_pStartDescriptor)
	{
		delete m_pStartDescriptor;
		m_pStartDescriptor = NULL;
	}
	if (m_pFileDescriptor)
	{
		delete m_pFileDescriptor;
		m_pFileDescriptor = NULL;
	}
// 	ReleaseLog(pLog);
}

bool StartDataThread::Init(void *param1, void *param2)
{
	m_pid = *(uint16*)param1;
	return Start();
}

bool StartDataThread::IsStart()
{
	bool bRet = bStart;
	bStart = false;
	return bRet;
}

bool StartDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_pid, 0x92);
	m_status = RUN;
	return true;
}

bool StartDataThread::Stop()
{
	if (m_status == STOP)
	{
		return true;
	}
	m_status = STOP;
	m_pFilter->Stop();
	return true;
}

void StartDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
#endif

	//pLog->Write(LOG_DVB, "[Start Descriptor] Run");
	syslog(LOG_INFO|LOG_USER, "[Start Descriptor] Run");

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
			fp = fopen("start", "rb");
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
				//DPRINTF("[Start Descriptor] Get Data\n");
				//do crc32 check
				uint16 len = getBits(m_buffer, 12, 12);
				uint32 crc = calc_crc32(m_buffer, len -1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2)))) & 0xffffffff;
				if (crc == crc1)
				{
					uint8 *pdata = m_buffer+3;
					if (m_pStartDescriptor)
					{
						delete m_pStartDescriptor;
						m_pStartDescriptor = NULL;
					}
					m_pStartDescriptor = new struct StartDescriptor;

					m_pStartDescriptor->DescriptorTag = *pdata++;
					m_pStartDescriptor->DescriptorLength = *pdata++;
					m_pStartDescriptor->FilmId = getBits(pdata, 0, 32);
					pdata += 4;
					m_pStartDescriptor->Version = *pdata++;
					m_pStartDescriptor->Type = *pdata++;
					m_pStartDescriptor->DCPLength = ((uint64)*pdata << 56)| 
						((uint64)*(pdata+1) << 48) | 
						((uint64)*(pdata+2) << 40) |
						((uint64)*(pdata+3) << 32) |
						((uint64)*(pdata+4) << 24) |
						((uint64)*(pdata+5) << 16) |
						((uint64)*(pdata+6) << 8) |
						((uint64)*(pdata+7)); 
					pdata += 8;
					m_pStartDescriptor->ReservedInfo = ((uint64)*pdata << 56)|
						((uint64)*(pdata+1) << 48) |
						((uint64)*(pdata+2) << 40) |
						((uint64)*(pdata+3) << 32) |
						((uint64)*(pdata+4) << 24) |
						((uint64)*(pdata+5) << 16) |
						((uint64)*(pdata+6) << 8) |
						((uint64)*(pdata+7));
					pdata += 8;
					m_pStartDescriptor->FilmNameLength = *pdata++;

					if (m_pStartDescriptor->FileName != NULL)
					{
						delete[] m_pStartDescriptor->FileName;
						m_pStartDescriptor->FileName = NULL;
					}
					m_pStartDescriptor->FileName = new char[m_pStartDescriptor->FilmNameLength];
					memcpy(m_pStartDescriptor->FileName,
						pdata,
						m_pStartDescriptor->FilmNameLength);
					m_pStartDescriptor->FileName[m_pStartDescriptor->FilmNameLength] = '\0';
					m_filmName = m_pStartDescriptor->FileName;
					pdata += m_pStartDescriptor->FilmNameLength;

					m_pStartDescriptor->UUIDLength = *pdata++;
					if (m_pStartDescriptor->UUID != NULL)
					{
						delete[] m_pStartDescriptor->UUID;
						m_pStartDescriptor->UUID = NULL;
					}
					m_pStartDescriptor->UUID = new char[m_pStartDescriptor->UUIDLength];
					memcpy(m_pStartDescriptor->UUID,
						pdata,
						m_pStartDescriptor->UUIDLength);
					m_pStartDescriptor->UUID[m_pStartDescriptor->UUIDLength] = '\0';
					m_uuid = m_pStartDescriptor->UUID;
					pdata += m_pStartDescriptor->UUIDLength;

					m_pStartDescriptor->IssueDateLength = *pdata++;
					if (m_pStartDescriptor->IssueDate != NULL)
					{
						delete[] m_pStartDescriptor->IssueDate;
						m_pStartDescriptor->IssueDate = NULL;
					}
					m_pStartDescriptor->IssueDate = new char[m_pStartDescriptor->IssueDateLength];
					memcpy(m_pStartDescriptor->IssueDate,
						pdata,
						m_pStartDescriptor->IssueDateLength);
					m_pStartDescriptor->IssueDate[m_pStartDescriptor->IssueDateLength] = '\0';
					m_issueDate = m_pStartDescriptor->IssueDate;
					pdata += m_pStartDescriptor->IssueDateLength;

					m_pStartDescriptor->IssuerLength = *pdata++;
					if (m_pStartDescriptor->Issuer != NULL)
					{
						delete[] m_pStartDescriptor->Issuer;
						m_pStartDescriptor->Issuer = NULL;
					}
					m_pStartDescriptor->Issuer = new char[m_pStartDescriptor->IssuerLength];
					memcpy(m_pStartDescriptor->Issuer,
						pdata,
						m_pStartDescriptor->IssuerLength);
					m_pStartDescriptor->Issuer[m_pStartDescriptor->IssuerLength] = '\0';
					m_issuer = m_pStartDescriptor->Issuer;
					pdata += m_pStartDescriptor->IssuerLength;

					m_pStartDescriptor->CreatorLength = *pdata++;
					if (m_pStartDescriptor->Creator != NULL)
					{
						delete[] m_pStartDescriptor->Creator;
						m_pStartDescriptor->Creator = NULL;
					}
					m_pStartDescriptor->Creator = new char[m_pStartDescriptor->CreatorLength];
					memcpy(m_pStartDescriptor->Creator,
						pdata,
						m_pStartDescriptor->CreatorLength);
					m_pStartDescriptor->Creator[m_pStartDescriptor->CreatorLength] = '\0';
					m_creator = m_pStartDescriptor->Creator;
					pdata += m_pStartDescriptor->CreatorLength;

					if (m_pFileDescriptor != NULL)
					{
						delete m_pFileDescriptor;
						m_pFileDescriptor = NULL;
					}
					m_pFileDescriptor = new struct FileDescriptor;
					m_pFileDescriptor->DescriptorTag = *pdata++;
					m_pFileDescriptor->DescriptorLength = *pdata++;
					m_pFileDescriptor->FileLength = ((uint64)*pdata << 32) | ((uint64)*(pdata+1) << 24) |
						((uint64)*(pdata+2) << 16) | ((uint64)*(pdata+3) << 8) | ((uint64)*(pdata+4));
					pdata += 5;
					m_pFileDescriptor->SegmentLength = getBits((uint8 *)pdata, 0, 32);
					pdata += 4;
					m_pFileDescriptor->Option = *pdata++;

					len = m_pFileDescriptor->DescriptorLength - 10;
					if(m_pFileDescriptor->FileName != NULL)
					{
						delete[] m_pFileDescriptor->FileName;
						m_pFileDescriptor->FileName = NULL;
					}
					m_pFileDescriptor->FileName = new char[len];
					memcpy(m_pFileDescriptor->FileName,
						pdata,
						len);
					m_pFileDescriptor->FileName[len] = '\0';
					
					bStart = true;
				}
			}
			break;
		case STOP:
			return;
		case IDLE:
			break;
		}
	}
}

