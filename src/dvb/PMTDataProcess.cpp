#include "PMTDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
//#include <log/Log.h>

PMTDataThread::PMTDataThread():m_status(0), m_PmtId(0), m_pManager(NULL), bThreadStart(false)
{
	m_pFilter = new Filter;
	//pLog = CreateLog();
}

PMTDataThread::~PMTDataThread()
{
	Stop();
	if(m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
	//ReleaseLog(pLog);
}

bool PMTDataThread::Init(void *param1, void *param2)
{
	m_PmtId = *(uint16 *)param1;

	return Start();
}

bool PMTDataThread::Start()
{
	Stop();
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_PmtId, 0x02);
	m_status = RUN;
	return true;
}

bool PMTDataThread::Stop()
{
	m_status = STOP;
	m_pFilter->Stop();
#ifdef DEBUG
	printf("PMT stop\n");
#endif

	std::list<FilmDataThread*>::iterator itor;
	for (itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread *filmThread = *itor;
#ifdef DEBUG
		printf("Film Thread stop\n");
#endif // DEBUG
		filmThread->Stop();
		while(!filmThread->isStop()) usleep(100000);
		filmThread->stop();
		delete filmThread;
	}
	m_filmList.clear();

	std::list<struct PmtDescriptor*>::iterator itr;
	for (itr = m_pmtList.begin(); itr != m_pmtList.end(); ++itr)
	{
		struct PmtDescriptor* pmt = *itr;
		delete pmt;
	}
	m_pmtList.clear();

	return true;
}

#if 0
void PMTDataThread::doit()
{
#ifdef DEBUG
	FILE *fp;
	printf("start pmt thread manager\n");
#endif
#if 0
	m_pManager = brunt::createThreadManager();
#endif // 0

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef DEBUG
// 			printf("again\n");
			fp = fopen("pmt", "rb");
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
				uint32 crc = calc_crc32(m_buffer, len -1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2)))) & 0xffffffff;
				if (crc == crc1)
				{
					uint8 *pbuf = m_buffer+12;
					m_programDescriptor.DescriptorTag = *pbuf++;
					m_programDescriptor.DescriptorLength = *pbuf++;
					m_programDescriptor.FilmId = getBits(pbuf, 0, 32);
					pbuf += 4;
					m_programDescriptor.Version = *pbuf++;
					m_programDescriptor.Type = *pbuf++;
					m_programDescriptor.FileNumber = *pbuf++;

					for(int i = 0; i < m_programDescriptor.FileNumber; i++)
					{
#ifdef DEBUG
						printf("new pmtdescriptor\n");
#endif // DEBUG
						struct PmtDescriptor *pPmtDescriptor = 
							new struct PmtDescriptor;
						pPmtDescriptor->StreamType = *pbuf++;
						pPmtDescriptor->ElementaryPid = ((pbuf[0] & 0x1f) << 8) | pbuf[1];
						pbuf += 4;
						pPmtDescriptor->subfolderCountDescriptor = 
							new struct SubfolderCountDescriptor;
						pPmtDescriptor->subfolderCountDescriptor->DescriptorTag = *pbuf++;
						pPmtDescriptor->subfolderCountDescriptor->DescriptorLength = *pbuf++;
						pPmtDescriptor->subfolderCountDescriptor->SubfolderCount = *pbuf++;

// 						printf("Stream Type: %X ElementPID: %X SubfolderCount: %d\n",
// 							pPmtDescriptor->StreamType, pPmtDescriptor->ElementaryPid,
// 							pPmtDescriptor->subfolderCountDescriptor->SubfolderCount);

						if (pPmtDescriptor->subfolderCountDescriptor->SubfolderCount > 0)
						{
#ifdef DEBUG
							printf("new subfolder\n");
#endif // DEBUG
							pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor = 
								new struct SubfolderDescriptor[pPmtDescriptor->subfolderCountDescriptor->SubfolderCount];
							for (int j = 0; j < pPmtDescriptor->subfolderCountDescriptor->SubfolderCount; j++)
							{
								pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorTag = *pbuf++;
								pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength = *pbuf++;
#ifdef DEBUG
								printf("new subfolder name\n");
#endif // DEBUG
								pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName = 
									new char[pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength];
								memcpy(pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName,
									pbuf,
									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength);
								pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName
									[pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength] = '\0';
// 								printf("%X %d\n",
// 									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorTag,
// 									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength);
// 								printf("%s\n", pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName);
								
								pbuf += pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength;
							}
						}
#ifdef DEBUG
						printf("new filedescriptor\n");
#endif // DEBUG

						pPmtDescriptor->fileDescriptor = new struct FileDescriptor;
						pPmtDescriptor->fileDescriptor->DescriptorTag = *pbuf++;
						pPmtDescriptor->fileDescriptor->DescriptorLength = *pbuf++;
						pPmtDescriptor->fileDescriptor->FileLength = ((uint64)*pbuf << 32) |
							((uint64)*(pbuf + 1) << 24) | ((uint64)*(pbuf + 2) << 16) |
							((uint64)*(pbuf + 3) << 8) | (uint64)*(pbuf + 4);
						pbuf +=5;
						pPmtDescriptor->fileDescriptor->SegmentLength = getBits(pbuf, 0, 32);
						pbuf += 4;
						pPmtDescriptor->fileDescriptor->Option = *pbuf++;
#ifdef DEBUG
						printf("new filename\n");
#endif // DEBUG
						pPmtDescriptor->fileDescriptor->FileName = 
							new char[pPmtDescriptor->fileDescriptor->DescriptorLength - 10];
						memcpy(pPmtDescriptor->fileDescriptor->FileName,
							pbuf,
							pPmtDescriptor->fileDescriptor->DescriptorLength - 10);
						pPmtDescriptor->fileDescriptor->FileName
							[pPmtDescriptor->fileDescriptor->DescriptorLength - 10] = '\0';
						pbuf += (pPmtDescriptor->fileDescriptor->DescriptorLength - 10);

// 						printf("File Len:%lld SegLen: %d\n", 
// 							pPmtDescriptor->fileDescriptor->FileLength,
// 							pPmtDescriptor->fileDescriptor->SegmentLength);
// 						printf("FileName: %s\n",
// 							pPmtDescriptor->fileDescriptor->FileName);

						//search pmt list for current pmt
						std::list<struct PmtDescriptor*>::iterator itor;
						bool bFind = false;
						for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
						{
							struct PmtDescriptor* pPmt = *itor;
							if (pPmt->ElementaryPid == pPmtDescriptor->ElementaryPid)
							{
								bFind = true;
								break;
							}
						}
						if(bFind)
						{
#if 0
							delete[] pPmtDescriptor->fileDescriptor->FileName;
							delete pPmtDescriptor->fileDescriptor;
							if(pPmtDescriptor->subfolderCountDescriptor->SubfolderCount > 0)
							{
								for (int i = 0; i < pPmtDescriptor->subfolderCountDescriptor->SubfolderCount; i++)
								{
									delete[] pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[i].SubfolderName;
								}
								delete[] pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor;
							}
#endif
#ifdef DEBUG
							printf("delete %x %x\n", pPmtDescriptor, pPmtDescriptor->ElementaryPid);
#endif // DEBUG
							delete pPmtDescriptor;
							pPmtDescriptor = NULL;
						}
						else
						{
#ifdef DEBUG
							printf("push pmt descriptor %X\n", pPmtDescriptor->ElementaryPid);
#endif // DEBUG
							m_pmtList.push_back(pPmtDescriptor);
// 							FilmDataThread *pFilmDataThread = new FilmDataThread;
// 							pFilmDataThread->Init((void*) pPmtDescriptor, NULL);
// 							m_filmList.push_back(pFilmDataThread);
						}
					}
				}
#ifdef DEBUG
				else
				{
					printf("pmt crc error\n");
				}
#endif
			}
			break;
		case STOP:
#if 0
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
#else
void PMTDataThread::doit()
{
#ifdef USE_SIM
	FILE *fp;
	printf("start pmt thread manager\n");
#endif
#if 0
	m_pManager = brunt::createThreadManager();
#endif // 0

	char str[200];
	sprintf(str, "[PMT Descriptor] Run 0x%x", this);
	//pLog->Write(LOG_DVB, str);
	//syslog(LOG_INFO|LOG_USER, "[PMT Descriptor] Run 0x%x", this);
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
#ifdef USE_SIM
// 			printf("again\n");
			fp = fopen("pmt", "rb");
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
				//DPRINTF("[PMT Descriptor] Get Data\n");
				//do crc32 check
				uint16 len = getBits(m_buffer, 12, 12);
				uint32 crc = calc_crc32(m_buffer, len -1) & 0xffffffff;
				uint32 crc1 = ((*((m_buffer + len - 1)) << 24)|
					(*((m_buffer + len)) << 16) |
					(*((m_buffer + len + 1)) << 8) |
					(*((m_buffer + len + 2)))) & 0xffffffff;
				if (crc == crc1)
				{
					uint8 *pbuf = m_buffer+12;
					m_programDescriptor.DescriptorTag = *pbuf++;
					m_programDescriptor.DescriptorLength = *pbuf++;
					m_programDescriptor.FilmId = getBits(pbuf, 0, 32);
					pbuf += 4;
					m_programDescriptor.Version = *pbuf++;
					m_programDescriptor.Type = *pbuf++;
					m_programDescriptor.FileNumber = *pbuf++;

					for(int i = 0; i < m_programDescriptor.FileNumber; i++)
					{
#if 0
						printf("new pmtdescriptor\n");
#endif // DEBUG
						struct PmtDescriptor *pPmtDescriptor = 
							new struct PmtDescriptor;
						pPmtDescriptor->StreamType = *pbuf++;
						pPmtDescriptor->ElementaryPid = ((pbuf[0] & 0x1f) << 8) | pbuf[1];
						pbuf += 4;

						//search pmt list for current pmt
						std::list<struct PmtDescriptor*>::iterator itor;
						bool bFind = false;
						for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
						{
							struct PmtDescriptor* pPmt = *itor;
							if (pPmt->ElementaryPid == pPmtDescriptor->ElementaryPid)
							{
								bFind = true;
								break;
							}
						}
						if(bFind)
						{
							len = getBits(pbuf - 2, 4, 12);
							pbuf += len;
#if 0
							printf("delete %x %x len: %d\n", pPmtDescriptor, pPmtDescriptor->ElementaryPid, len);
#endif // DEBUG
							delete pPmtDescriptor;

#if 0
							//Check and start thread
							if(!bThreadStart)
							{
								m_pmtList.sort(PmtDescriptor());
								DPRINTF("Create Film Thread\n");
								std::list<struct PmtDescriptor*>::iterator pos;
								uint32 start_seg = 0;
								for(pos = m_pmtList.begin(); pos != m_pmtList.end(); ++pos)
								{
									struct PmtDescriptor* pPmt = *pos;
									DPRINTF("new Film Thread\n");
									FilmDataThread *pFilmDataThread = new FilmDataThread;
									DPRINTF("Init Film Thread\n");
									pFilmDataThread->Init((void*) pPmt, (void*)start_seg);
									DPRINTF("Init done\n");
									start_seg += 
										((pPmt->fileDescriptor->FileLength + 
										pPmt->fileDescriptor->SegmentLength - 1)
										/ pPmt->fileDescriptor->SegmentLength);
									DPRINTF("start seg: %X\n", start_seg);

									brunt::ThreadRunPolicy policy;
									policy.order = 0;
									policy.policy = brunt::POLICY_ERROR_EXIT;

									DPRINTF("Thread Info\n");
									brunt::CThreadRunInfo threadInfo(pFilmDataThread, policy);
									DPRINTF("add Thread Info\n");
									m_pManager->addThread(threadInfo); 
									DPRINTF("push thread list\n");

									m_filmList.push_back(pFilmDataThread);
								}
								DPRINTF("set parallel number\n");
								m_pManager->setParallelNum(0, m_pmtList.size());
								DPRINTF("run\n");
								m_pManager->run();
								bThreadStart = true;
							}
#endif
						}
						else
						{
							pPmtDescriptor->subfolderCountDescriptor = 
								new struct SubfolderCountDescriptor;
							pPmtDescriptor->subfolderCountDescriptor->DescriptorTag = *pbuf++;
							pPmtDescriptor->subfolderCountDescriptor->DescriptorLength = *pbuf++;
							pPmtDescriptor->subfolderCountDescriptor->SubfolderCount = *pbuf++;

							if (pPmtDescriptor->subfolderCountDescriptor->SubfolderCount > 0)
							{
#if 0
								printf("new subfolder\n");
#endif // DEBUG
								pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor = 
									new struct SubfolderDescriptor[pPmtDescriptor->subfolderCountDescriptor->SubfolderCount];
								for (int j = 0; j < pPmtDescriptor->subfolderCountDescriptor->SubfolderCount; j++)
								{
									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorTag = *pbuf++;
									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength = *pbuf++;

									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName = 
										new char[pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength + 1];
									memcpy(pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName,
										pbuf,
										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength);
									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName
										[pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength] = '\0';

// 									DPRINTF("%s\n", 
// 										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName);

									pbuf += pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength;
								}
							}

							pPmtDescriptor->fileDescriptor = new struct FileDescriptor;
							pPmtDescriptor->fileDescriptor->DescriptorTag = *pbuf++;
							pPmtDescriptor->fileDescriptor->DescriptorLength = *pbuf++;
							pPmtDescriptor->fileDescriptor->FileLength = ((uint64)*pbuf << 32) |
								((uint64)*(pbuf + 1) << 24) | ((uint64)*(pbuf + 2) << 16) |
								((uint64)*(pbuf + 3) << 8) | (uint64)*(pbuf + 4);
							pbuf +=5;
							pPmtDescriptor->fileDescriptor->SegmentLength = getBits(pbuf, 0, 32);
							pbuf += 4;
							pPmtDescriptor->fileDescriptor->Option = *pbuf++;

							pPmtDescriptor->fileDescriptor->FileName = 
								new char[pPmtDescriptor->fileDescriptor->DescriptorLength - 9];
							memcpy(pPmtDescriptor->fileDescriptor->FileName,
								pbuf,
								pPmtDescriptor->fileDescriptor->DescriptorLength - 10);
							pPmtDescriptor->fileDescriptor->FileName
								[pPmtDescriptor->fileDescriptor->DescriptorLength - 10] = '\0';
							pbuf += (pPmtDescriptor->fileDescriptor->DescriptorLength - 10);

// 							DPRINTF("%s\n", pPmtDescriptor->fileDescriptor->FileName);

							m_pmtList.push_back(pPmtDescriptor);
#if 1
							sprintf(str, "[PMT Descriptor] Create FilmData 0x%x", pPmtDescriptor->ElementaryPid);
							//pLog->Write(LOG_DVB, str);
							//syslog(LOG_INFO|LOG_USER, str);
							FilmDataThread *pFilmDataThread = new FilmDataThread;
							pFilmDataThread->Init((void*) pPmtDescriptor, NULL);
							//pFilmDataThread->Start();

							m_filmList.push_back(pFilmDataThread);

// 							brunt::ThreadRunPolicy policy;
// 							policy.order = 0;
// 							policy.policy = brunt::POLICY_ERROR_EXIT;

// 							brunt::CThreadRunInfo threadInfo(pFilmDataThread, policy);
// 							m_pManager->addThread(threadInfo); 
							if(pFilmDataThread->status() == brunt::thread_ready)
							{
								pFilmDataThread->start();
								DPRINTF("%s\n", pPmtDescriptor->fileDescriptor->FileName);
							}

// 							m_pManager->setParallelNum(0, m_pmtList.size());
							//syslog(LOG_INFO|LOG_USER, "Thread count:%d\n", m_pmtList.size());
// 							m_pManager->run();
#endif
						}
					}
				}
#ifdef DEBUG
				else
				{
					printf("pmt crc error\n");
				}
#endif
			}
			break;
		case STOP:
#if 0
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
#endif

uint64 PMTDataThread::ReciveLength()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->ReciveLength();
	}
	return length;
}

uint64 PMTDataThread::FileLength()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->FileLength();
	}
	return length;
}

uint64 PMTDataThread::ReciveSegment()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->ReciveSegment();
	}
	return length;
}
uint64 PMTDataThread::LostSegment()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->LostSegment();
	}
	return length;
}
uint64 PMTDataThread::CRCError()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->CRCError();
	}
	return length;
}

uint64 PMTDataThread::TotalSegment()
{
	uint64 length = 0;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->TotalSegment();
	}
	return length;
}

void PMTDataThread::GetLostSegment()
{
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		struct LostBuf * pLostBuf = thread->GetLostSegment();
		//TODO: send lost report to remote
		//--------------------------------
	}
}
