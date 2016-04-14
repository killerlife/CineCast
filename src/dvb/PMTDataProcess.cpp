#include "PMTDataProcess.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
//#include <log/Log.h>
#include <time.h>

extern RECEIVE_INFO gRecv;

PMTDataThread::PMTDataThread():
m_status(0), m_PmtId(0), 
m_pManager(NULL), bThreadStart(false),
m_mutex(0)
{
	m_pFilter = new Filter;
	pDebugCmd = GetDebugCommand();
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
	if (m_status == STOP)
		return true;

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

#include <log/Log.h>
#include <log/LogStruct.h>
extern ILog* gLog;

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
	if(gLog)
		gLog->Write(LOG_DVB, str);
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
				m_mutex = 1;
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
							delete pPmtDescriptor;
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

							m_pmtList.push_back(pPmtDescriptor);
#if 1
							sprintf(str, "[PMT Descriptor] Create FilmData 0x%x", pPmtDescriptor->ElementaryPid);
							//pLog->Write(LOG_DVB, str);
							//syslog(LOG_INFO|LOG_USER, str);
							FilmDataThread *pFilmDataThread = new FilmDataThread;
							pFilmDataThread->Init((void*) pPmtDescriptor, NULL);
							//pFilmDataThread->Start();

							if(pFilmDataThread->status() == brunt::thread_ready)
							{
								pFilmDataThread->start();
								DPRINTF("%s\n", pPmtDescriptor->fileDescriptor->FileName);
							}

							m_filmList.push_back(pFilmDataThread);
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
				m_mutex = 0;
			}
			else
			{
#if 1
				if((*pDebugCmd) == D_PMT)
				{
					{
						for(int i = 0; i < 10; i++)
						{
							struct PmtDescriptor *pPmtDescriptor = 
								new struct PmtDescriptor;
							pPmtDescriptor->StreamType = 0;
							pPmtDescriptor->ElementaryPid = 0x21+i;

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
								delete pPmtDescriptor;
							}
							else
							{
								pPmtDescriptor->subfolderCountDescriptor = 
									new struct SubfolderCountDescriptor;
								pPmtDescriptor->subfolderCountDescriptor->DescriptorTag = 0;
								pPmtDescriptor->subfolderCountDescriptor->DescriptorLength = 1;
								pPmtDescriptor->subfolderCountDescriptor->SubfolderCount = 1;

								if (pPmtDescriptor->subfolderCountDescriptor->SubfolderCount > 0)
								{
									pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor = 
										new struct SubfolderDescriptor[pPmtDescriptor->subfolderCountDescriptor->SubfolderCount];
									for (int j = 0; j < pPmtDescriptor->subfolderCountDescriptor->SubfolderCount; j++)
									{
										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorTag = 0;
										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength = 1;

										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName = 
											new char[10];
										memcpy(pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName,
											"20160324\\",
											9);;
										pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName
											[9] = '\0';
									}
								}

								pPmtDescriptor->fileDescriptor = new struct FileDescriptor;
								pPmtDescriptor->fileDescriptor->DescriptorTag = 0;
								pPmtDescriptor->fileDescriptor->DescriptorLength = 1;
								pPmtDescriptor->fileDescriptor->FileLength = 1000000000;
								pPmtDescriptor->fileDescriptor->SegmentLength = 532;
								pPmtDescriptor->fileDescriptor->Option = 0;

								pPmtDescriptor->fileDescriptor->FileName = 
									new char[100];
								sprintf(pPmtDescriptor->fileDescriptor->FileName, "LEONIS_TEST%d.test", i);

								m_pmtList.push_back(pPmtDescriptor);
#if 1
								sprintf(str, "[PMT Descriptor] Create FilmData 0x%x", pPmtDescriptor->ElementaryPid);
								//pLog->Write(LOG_DVB, str);
								//syslog(LOG_INFO|LOG_USER, str);
								FilmDataThread *pFilmDataThread = new FilmDataThread;
								pFilmDataThread->Init((void*) pPmtDescriptor, NULL);
								//pFilmDataThread->Start();

								if(pFilmDataThread->status() == brunt::thread_ready)
								{
									pFilmDataThread->start();
									DPRINTF("%s\n", pPmtDescriptor->fileDescriptor->FileName);
								}

								m_filmList.push_back(pFilmDataThread);
#endif
							}
						}
					}
					*pDebugCmd = 0;
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

uint64 PMTDataThread::ReciveLength()
{
	uint64 length = 0;
	if(m_mutex == 1)
		return length;
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
	if(m_mutex == 1)
		return length;
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
	if(m_mutex == 1)
		return length;
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
// 	if(m_mutex == 1)
// 		return length;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->LostSegment();
	}
	return length;
}

bool PMTDataThread::RoundCleanCounter()
{
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		thread->RoundCleanCounter();
	}
	return true;
}

uint64 PMTDataThread::CRCError()
{
	uint64 length = 0;
	if(m_mutex == 1)
		return length;
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
	if(m_mutex == 1)
		return length;
	std::list<FilmDataThread*>::iterator itor;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		length += thread->TotalSegment();
	}
	return length;
}

uint64 PMTDataThread::GetLostSegment()
{
	uint64 nLostCount = 0;
	std::list<FilmDataThread*>::iterator itor;
	std::vector<std::string> dcp;

	if(m_mutex == 1)
		return nLostCount;
	m_strReportFileList = ""; 
	bool bFound = false;
	uint32 id = 0;
	for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		FilmDataThread* thread = *itor;
		m_strReportFileList += thread->GetLostSegment();
		m_strReportFileList += " ";
		nLostCount += thread->LostSegment();
		if((id = thread->GetFilmId()) != 0)
		{
			m_FilmId = id;
			bFound = true;
		}

		std::string assetmap = thread->FindAssetmap();
		if(assetmap != "")
		{
			bool bFound = false;
			for (int j = 0; j < dcp.size(); j++)
			{
				if (assetmap == dcp.at(j))
				{
					bFound = true;
				}
			}
			if (!bFound)
			{
				dcp.push_back(assetmap);
			}
		}
	}
	if (!bFound)
		m_FilmId = gRecv.nFileID;

	for(int i = 0; i < dcp.size(); i++)
	{
		uint64 mFilmLength = 0;
		uint64 mReceLength = 0;
		uint64 mTotaSegment = 0;
		uint64 mReceSegment = 0;
		uint64 mLostSegment = 0;
		uint64 mCrcError = 0;

		for(itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
		{
			FilmDataThread* thread = *itor;
			if(thread->IsSameDCP(dcp.at(i)))
			{
				mFilmLength += thread->FileLength();
				mReceLength += thread->ReciveLength();
				mTotaSegment += thread->TotalSegment();
				mReceSegment += thread->ReciveSegment();
				mLostSegment += thread->LostSegment();
				mCrcError += thread->CRCError();
			}
		}
		std::string fn = dcp.at(i) + ".RecvStatus";
		FILE *fp = fopen(fn.c_str(), "wb");
		if(fp)
		{
			char buf[200];
			std::string name = dcp.at(i);
			if (name.at(name.size() - 1) == '/')
				name.resize(name.size() - 1);
			size_t pos = name.rfind("/");
			if(pos != std::string::npos)
			{
				name = name.c_str() + pos + 1;
			}
			sprintf(buf, "FilmId=%lld\nFilmName=%s\nFilmLength=%lld\n",
				m_FilmId, name.c_str(), mFilmLength);
			fwrite(buf, strlen(buf), 1, fp);
			sprintf(buf, "Creator=%s\nCRCError=%lld\nIssueDate=%d\n",
				gRecv.strCreator.c_str(), mCrcError, gRecv.strIssueDate.c_str());
			fwrite(buf, strlen(buf), 1, fp);
			sprintf(buf, "Issuer=%s\nLostSegment=%lld\nReceiveLength=%lld\n",
				gRecv.strIssuer.c_str(), mLostSegment, mReceLength);
			fwrite(buf, strlen(buf), 1, fp);
			sprintf(buf, "ReceiveStatus=%lld\nTotalSegment=%lld\nReceiveSegment=%lld\n",
				gRecv.nReceiveStatus & 0xffff, mTotaSegment, mReceSegment);
			fwrite(buf, strlen(buf), 1, fp);

			time_t t;
			time(&t);
			struct tm *pTm;
			pTm = localtime(&t);
 			sprintf(buf, "DateTime=%.4d-%.2d-%.2d\n",
				pTm->tm_year + 1900,
				pTm->tm_mon + 1,
				pTm->tm_mday);
 			fwrite(buf, strlen(buf), 1, fp);
			fclose(fp);
		}
	}
	return nLostCount;
}

bool PMTDataThread::UnzipSubtitle()
{
	std::list<FilmDataThread*>::iterator itor;
	if(m_mutex == 1)
		return false;
	for (itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		if((*itor)->UnzipSubtitle())
			return true;
	}
	return false;
}

bool PMTDataThread::SaveData(char* fn, char* pData, uint32 segNum, uint32 len)
{
	std::list<FilmDataThread*>::iterator itor;
	if(m_mutex == 1)
		return false;
	for (itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		if((*itor)->SaveData(fn, pData, segNum, len))
			return true;
	}
	return false;
}

bool PMTDataThread::IsFilmDataReady()
{
	std::list<FilmDataThread*>::iterator itor;
	if (m_filmList.size() < 1)
		return false;
	if(m_mutex == 1)
		return false;
	for (itor = m_filmList.begin(); itor != m_filmList.end(); ++itor)
	{
		if(!(*itor)->IsReady())
			return false;
	}
	return true;
}