#include "FilmDataProcess.h"
#include "DataProcessThread.h"
#include "../netcomm/NetCommThread.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <log/Log.h>
#include <netcomm/message.h>

namespace fs = boost::filesystem;
using namespace brunt;

extern ILog* gLog;
NetCommThread *pNetComm;

FilmDataThread::FilmDataThread(): m_ReciveLength(0), m_status(STOP),
m_pZtBuf(0), m_pFilter(0), m_pFilmFile(NULL), m_nSegBasic(0), 
m_ReciveSegment(0), m_LostSegment(0), m_CRCError(0), m_TotalSegment(0), 
m_freeMutex(0), m_dataMutex(0), m_ztPos(-1)
{
	m_pDataThread = NULL;
	m_pPmtDescriptor = NULL;
// 	m_pObserver = NULL;
	m_strFileName.clear();
	m_strZtFileName.clear();
	m_pFilter = new Filter;
	//pLog = CreateLog();
// 	m_pManager = brunt::createThreadManager();
}

FilmDataThread::~FilmDataThread()
{
	Stop();

	usleep(100000);
	UpdateFile();
	usleep(100000);

	if(m_pZtFilmFile)
		fclose(m_pZtFilmFile);
	if(m_pFilmFile)
		fclose(m_pFilmFile);

	if (m_pZtBuf)
	{
		delete[] m_pZtBuf;
		m_pZtBuf = NULL;
	}

	if (m_pFilter)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}
	if (m_sLostBuf.m_buf)
	{
		delete[] m_sLostBuf.m_buf;
	}
	FreeBufPool();
	//ReleaseLog(pLog);
}

bool FilmDataThread::Init(void *param1, void *param2)
{
	m_pPmtDescriptor = (struct PmtDescriptor*)param1;

#if 0
	m_nSegBasic = (uint32)param2;
#endif

	//Create Save FileName
	m_strFileName = STORAGE_PATH;
	for(int i = 0; i< m_pPmtDescriptor->subfolderCountDescriptor->SubfolderCount; i++)
	{
		m_strFileName += m_pPmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[i].SubfolderName;
		if(m_strFileName[m_strFileName.size() - 1] == '\\')
		{
			m_strFileName.resize(m_strFileName.size()-1);
			m_strFileName += "/";
		}
		fs::create_directories(m_strFileName);
	}

// 	m_pPmtDescriptor->fileDescriptor->FileName
// 		[m_pPmtDescriptor->fileDescriptor->DescriptorLength - 10] = '\0';

	m_strFileName += m_pPmtDescriptor->fileDescriptor->FileName;

	//Create lost report filename
	m_strZtFileName = m_strFileName + ".zt";

	//Check lost file exist, and read it into buffer 
	int64 sec_nums = (m_pPmtDescriptor->fileDescriptor->FileLength + 
		m_pPmtDescriptor->fileDescriptor->SegmentLength - 1) /
		m_pPmtDescriptor->fileDescriptor->SegmentLength;
	
	m_TotalSegment = sec_nums;

	sec_nums = (sec_nums + 7) / 8;


	if (m_pZtBuf != NULL)
	{
		delete[] m_pZtBuf;
	}

	m_nZtBufSize = sec_nums;

	m_pZtBuf = new uint8[m_nZtBufSize];
	m_sLostBuf.m_size = m_nZtBufSize
		+ 4 + 4 + 8 + 4 + 4 + 4 + 4;
	//ID + count + receive bytes + status + reserved + msg length + msg buffer + crc32
	m_sLostBuf.m_buf = new uint8[m_sLostBuf.m_size + 100];

	memset(m_pZtBuf, 0, m_nZtBufSize);
	memset(m_sLostBuf.m_buf, 0, m_sLostBuf.m_size);

	FILE *fp;
	if((fp = fopen(m_strZtFileName.c_str(), "rb")) > 0)
	{
		fread(m_pZtBuf, 1, sec_nums, fp);
		fclose(fp);
		m_pZtFilmFile = fopen(m_strZtFileName.c_str(), "rb+");
		UpdateInfo();
	}
	else
	{
		m_pZtFilmFile = fopen(m_strZtFileName.c_str(), "wb+");
	}

	//Check the Film file exist, if not existed, create a new one
	//otherwise, reopen film file with append mode.
	if((m_pFilmFile = fopen(m_strFileName.c_str(), "rb")) <= 0)
	{
		//syslog(LOG_INFO|LOG_USER, "Create Film file: %s\n",
		//	m_strFileName.c_str());
		m_pFilmFile = fopen(m_strFileName.c_str(), "wb+");
		if(m_pFilmFile > 0)
		{
			if(fseek(m_pFilmFile, m_pPmtDescriptor->fileDescriptor->FileLength - 1, SEEK_SET) == 0)
			{
				char a = 0;

				fwrite(&a, 1, 1, m_pFilmFile);
				fflush(m_pFilmFile);
			}
		}
	}
	else
	{
		fclose(m_pFilmFile);
		//syslog(LOG_INFO|LOG_USER, "Reopen film file: %s\n",
		//	m_strFileName.c_str());
		m_pFilmFile = fopen(m_strFileName.c_str(), "rb+");
	}

	AllocBufPool();

	return Start();
}

bool FilmDataThread::Start()
{
	Stop();

	//syslog(LOG_INFO|LOG_USER, "Init Data Thread\n");

// 	brunt::ThreadRunPolicy policy;
// 	policy.order = 0;
// 	policy.policy = brunt::POLICY_ERROR_EXIT;
// 	brunt::CThreadRunInfo threadInfo(m_pDataThread, policy);
// 	m_pManager->addThread(threadInfo); 
// 	m_pManager->setParallelNum(0, 1);
// 	m_pManager->run();

	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_pPmtDescriptor->ElementaryPid, 0x80);
	m_status = RUN;
	bStop = false;
	return true;
}

bool FilmDataThread::Stop()
{
	if(m_status == STOP)
		return true;

	//syslog(LOG_INFO|LOG_USER, "Film Data Thread[%llx] stop\n", (uint32)this);
#if 0
	if (m_pDataThread)
	{
		m_pDataThread->m_status = STOP;
		m_pDataThread->Stop();
		m_pDataThread->stop();
		while(!m_pDataThread->isStop()) usleep(10000);
		delete m_pDataThread;
		m_pDataThread = NULL;
	}

	usleep(100000);
#endif
	m_pFilter->Stop();
	m_status = STOP;

	return true;
}

void FilmDataThread::doit()
{
#if 1
	char str[200];
	sprintf(str, "[FilmData] Run 0x%x", m_pPmtDescriptor->ElementaryPid);
	//pLog->Write(LOG_DVB, str);

#if 0
	m_pDataThread = new DataProcessThread;

	m_pDataThread->Init((void*) this, NULL);
	m_pDataThread->start();
#endif // 0

#ifdef USE_SIM
	FILE *fp;
	int cc = 0;
	while(1)
#else
	while(1)
#endif
	{
		switch (m_status)
		{
		case RUN:
			{
				#if 0
				struct FilmDataBuf* pBuf;
				while(1) 
				{
					if(m_status == STOP)
					{
						bStop = true;
						return;
					}
					pBuf = GetFreeBuf();
					if (pBuf == NULL)
						usleep(1);
					else
					{
						if (m_status == STOP)
						{
							PushIntoFreeBuf(pBuf);
							bStop = true;
							return;
						}
// 						DPRINTF("Get Free Buf\n");
						break;
					}
				}
				#endif
#ifdef USE_SIM
				char fn[20];
				sprintf(fn, "data-23");
				//sprintf(fn, "data-%X", m_pPmtDescriptor->ElementaryPid);
// 				DPRINTF("open: %s\n", fn);

				fp = fopen(fn, "rb");
				if(fp <= 0)
				{
					PushIntoFreeBuf(pBuf);
// 					printf("open file error\n");
// 					m_status = STOP;
					break;
				}
				pBuf->count = fread(pBuf->pBuf, 1, m_allocSize, fp);
				fclose(fp);
				pushIntoDataBuf(pBuf);
#else
//				DPRINTF("[Film Data] Ready read 0x%x\n", m_pPmtDescriptor->ElementaryPid);
#if 1
				uint8 buff[4096];
				uint16 count = 4096;
				if (m_pFilter->ReadFilter(buff, count))
				{
				#if 1
					uint16 len = getBits(buff, 12, 12);
					uint32 crc = calc_crc32(buff, len - 1) & 0xffffffff;
					uint32 crc1 = ((*((buff + len - 1)) << 24)|
					(*((buff + len)) << 16) |
					(*((buff + len + 1)) << 8) |
					(*((buff + len + 2))))  & 0xffffffff;

					//do crc32 check
					if (crc == crc1)
					//if(count != m_pPmtDescriptor->fileDescriptor->SegmentLength)
					{
						//Get segment number and construct file position
						uint16 w_size;
						uint32 seg_num = getBits(buff + 11, 0, 32);
						uint16 pl  = getBits(buff + 1, 4, 12);
						uint16 pll = *(buff + 4);
						w_size = pl - pll - 6;
						filmId = getBits(buff + 5, 0, 32);
						#if 0
						if(pos == 0)
						    pos = seg_num;
						else
						{
						    if(++pos != seg_num)
						    {
							miss_count += seg_num - pos + 1;
							//DPRINTF("[DataProcess] miss segment %ld\n", miss_count);
						    }
						    
						}
						#endif
						uint64 pos = (uint64) seg_num * m_pPmtDescriptor->fileDescriptor->SegmentLength;
	
						//write data to file
						if (m_pFilmFile > 0)
						{	
							#if 0
 							DPRINTF("-------------------------------------------------->\nwrite %llX to %s\n%llX %X\n",
 								pos,
 								pFilmThread->m_pPmtDescriptor->fileDescriptor->FileName,
 								pFilmThread->m_pPmtDescriptor->fileDescriptor->FileLength,
 								seg_num);
							#endif
							if(!haveSegment(seg_num))
							{
							WriteFile(pos,
								buff + 15,
								w_size);
  							UpdateZtMem(seg_num);
  						
							//this only for test
							//UpdateZtFile();
							//
							m_ReciveSegment++;//= w_size;
							m_ReciveLength += w_size;
						}
					}
					}
					else
						m_CRCError++;
				#else
					m_ReciveSegment++;//= w_size;
					//m_ReciveLength += w_size;
				#endif
				}
#else
				pBuf->count = 4096;
				if (m_pFilter->ReadFilter(pBuf->pBuf, pBuf->count))
				{
					//DPRINTF("[Film Data] push into data 0x%x\n", m_pPmtDescriptor->ElementaryPid);
					//m_pDataThread->start();
					pushIntoDataBuf(pBuf);
					//PushIntoFreeBuf(pBuf);
					m_ReciveLength++;
				}
				else
					PushIntoFreeBuf(pBuf);
#endif
#endif
			}		
			break;
		case STOP:
			DPRINTF("enter stop %x\n", this);
			UpdateFile();
			bStop = true;
// 			DPRINTF("exit stop\n");
// 			m_pManager->terminate();
// 			brunt::releaseThreadManager(m_pManager);
// 			m_pManager = NULL;
			return;
		case IDLE:
			break;
		}
	}
#endif
}

bool FilmDataThread::haveSegment(uint32 nSegNum)
{
	int p1 = nSegNum/8;
	int p2 = nSegNum%8;
	if(p1 < m_nZtBufSize)
	{
		if(m_pZtBuf[p1] & (1<<p2))
			return true;
	}
	return false;
}

void FilmDataThread::UpdateZtMem(uint32 nSegNum)
{
	int p1 = nSegNum/8;
	int p2 = nSegNum%8;
	if(p1 < m_nZtBufSize)
		m_pZtBuf[p1] |= (1<<p2); 
	if(m_ztPos == -1)
	{
		m_ztPos = p1;
	}
	else
	{
		if((m_ztPos + 1025) < p1)
		{
			if(m_pZtFilmFile > 0)
			{
				fseek(m_pZtFilmFile, m_ztPos, SEEK_SET);
				fwrite((m_pZtBuf + m_ztPos),
					1024,
					1,
					m_pZtFilmFile);
			}
			m_ztPos += 1024;
		}
	}
// 	DPRINTF("ZT POS: %X %X\n", p1, p2);
}

void FilmDataThread::UpdateFile()
{
	//flush film file and save it
	if(m_pFilmFile > 0)
	{
		fflush(m_pFilmFile);
		//m_pFilmFile = 0;
	}

	//flush zt file and save it
	UpdateZtFile();
	if(m_pZtFilmFile > 0)
	{
	    fflush(m_pZtFilmFile);
	    //m_pZtFilmFile = 0;
	}

	return;
}

void FilmDataThread::UpdateZtFile()
{
	//flush zt file and save it
	if (m_pZtFilmFile > 0)
	{
// 		DPRINTF("Update file:%d %s\n", m_nZtBufSize, m_strZtFileName.c_str());
// 		DPRINTF("FL:%lld SL:%ld\n", 
// 			m_pPmtDescriptor->fileDescriptor->FileLength,
// 			m_pPmtDescriptor->fileDescriptor->SegmentLength);
		fseek(m_pZtFilmFile, 0, SEEK_SET);
		fwrite(m_pZtBuf, m_nZtBufSize, 1, m_pZtFilmFile);
		//fclose(m_pZtFilmFile);
		//m_pZtFilmFile = 0;
	}
	return;
}

uint64 FilmDataThread::ReciveLength()
{
	return m_ReciveLength;
}

uint64 FilmDataThread::ReciveSegment()
{
	return m_ReciveSegment;
}

uint64 FilmDataThread::LostSegment()
{
	return m_LostSegment;
}

uint64 FilmDataThread::CRCError()
{
	return m_CRCError;
}

uint64 FilmDataThread::TotalSegment()
{
	return m_TotalSegment;
}

uint64 FilmDataThread::FileLength()
{
	if(m_pPmtDescriptor != NULL)
	return m_pPmtDescriptor->fileDescriptor->FileLength;
	return 0;
}

void FilmDataThread::WriteFile(uint64 pos, uint8 *pbuf, uint16 size)
{
	if (m_pFilmFile > 0)
	{
		fseek(m_pFilmFile, pos, SEEK_SET);
		fwrite(pbuf,
			size,
			1,
			m_pFilmFile);
	}
}

void FilmDataThread::AllocBufPool()
{
	m_allocSize = 4096;//m_pPmtDescriptor->fileDescriptor->SegmentLength + 22;
	for (int i = 0; i < 10; i++)
	{
		struct FilmDataBuf* pBuf = new struct FilmDataBuf;
		pBuf->pBuf = new uint8[m_allocSize];
		pBuf->count = m_allocSize;
		m_bufFreePool.push_back(pBuf);
	}
}

void FilmDataThread::FreeBufPool()
{
	std::list<struct FilmDataBuf*>::iterator pos;
	for (pos = m_bufFreePool.begin(); pos != m_bufFreePool.end(); ++pos)
	{
		struct FilmDataBuf* pBuf = *pos;
		delete pBuf;
	}

	for (pos = m_bufDataPool.begin(); pos != m_bufDataPool.end(); ++pos)
	{
		struct FilmDataBuf* pBuf = *pos;
		delete pBuf;
	}

	m_bufFreePool.clear();
	m_bufDataPool.clear();
}

struct FilmDataBuf* FilmDataThread::GetFreeBuf()
{
	struct FilmDataBuf* pBuf;
	if (m_bufFreePool.empty())
	{
		m_freeMutex = 0;
		//DPRINTF("No Free buf 0x%x\n", this);
		return NULL;
	}
	pBuf = m_bufFreePool.front();
	while(m_freeMutex);
	m_freeMutex = 1;
	m_bufFreePool.pop_front();
	m_freeMutex = 0;
	return pBuf;
}

struct FilmDataBuf* FilmDataThread::GetDataBuf()
{
	struct FilmDataBuf* pBuf;
	if (m_bufDataPool.empty())
	{
		m_dataMutex = 0;
		//DPRINTF("Data buf empty 0x%x\n", this);
		return NULL;
	}
	pBuf = m_bufDataPool.front();
	while(m_dataMutex);
	m_dataMutex = 1;
	m_bufDataPool.pop_front();
	m_dataMutex = 0;
	return pBuf;
}

void FilmDataThread::PushIntoFreeBuf(struct FilmDataBuf* pBuf)
{
	while(m_freeMutex);
	m_freeMutex = 1;
	m_bufFreePool.push_back(pBuf);
	m_freeMutex = 0;
}

void FilmDataThread::pushIntoDataBuf(struct FilmDataBuf* pBuf)
{
	while(m_dataMutex);
	m_dataMutex = 1;
	m_bufDataPool.push_back(pBuf);
	m_dataMutex = 0;
}

struct LostBuf* FilmDataThread::GetLostSegment()
{
	int64 sec_nums = (m_pPmtDescriptor->fileDescriptor->FileLength + 
		m_pPmtDescriptor->fileDescriptor->SegmentLength - 1) /
		m_pPmtDescriptor->fileDescriptor->SegmentLength;
	
	UpdateFile();
	uint64 ReceiveBytes = 0;
	uint32 LostSegments = 0;
	uint32 ReceiveSegments = 0;
	uint32 nPos = 28;
	int i;
	for (i = 0; i < m_nZtBufSize - 1; i++)
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for(int j = 0; j < 8; j++)
		{
			if(ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
			{
				LostSegments++;
			}
			ch >>=1;
		}
	}
	int64 nums = sec_nums%8;
	if(nums == 0)
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for (int j = 0; j < 8; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
			{
				LostSegments++;
			}
			ch >>= 1;
		}
	}
	else
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for(int j = 0; j < nums; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
				LostSegments++;
			ch >>= 1;
		}
	}
	m_ReciveLength = ReceiveBytes;
	m_ReciveSegment = ReceiveSegments;
	m_LostSegment = LostSegments;
	L_LOST_INFO *info = (L_LOST_INFO*)m_sLostBuf.m_buf;
	info->filmID = filmId;
	info->lostNum = LostSegments;
	info->receivedByteCount = ReceiveBytes;
	info->recvState = 5;
	info->lostLength = m_sLostBuf.m_size;
	char str[512];
	if (gLog)
	{
		sprintf(str, "[FilmData] FilmID=%04X, PID=%04X, %s"
			, filmId
			, m_pPmtDescriptor->ElementaryPid
			, m_strFileName.c_str());
		gLog->Write(LOG_DVB, str);
		sprintf(str, "[FilmData] TotalSeg=%d, LostSeg=%d, CRCSeg=%d, SegLen=%d",
			m_TotalSegment,
			m_LostSegment,
			m_CRCError,
			m_pPmtDescriptor->fileDescriptor->SegmentLength);
		gLog->Write(LOG_DVB, str);
	}
	if(pNetComm)
	{
		//TODO: send lost report to remote
		sprintf(str, "%s.lost", m_strZtFileName.c_str());
		FILE *fp = fopen(str, "rb");
		if(fp)
		{
			fwrite((char*)info, m_sLostBuf.m_size, 1, fp);
			fclose(fp);
		}
		//--------------------------------
		pNetComm->ReportLost((char*)info, m_sLostBuf.m_size);
	}
	return &m_sLostBuf;
}
void FilmDataThread::UpdateInfo()
{
	int64 sec_nums = (m_pPmtDescriptor->fileDescriptor->FileLength + 
		m_pPmtDescriptor->fileDescriptor->SegmentLength - 1) /
		m_pPmtDescriptor->fileDescriptor->SegmentLength;

	uint64 ReceiveBytes = 0;
	uint32 LostSegments = 0;
	uint32 ReceiveSegments = 0;
	uint32 nPos = 28;
	int i;
	for (i = 0; i < m_nZtBufSize - 1; i++)
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for(int j = 0; j < 8; j++)
		{
			if(ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
			{
				LostSegments++;
			}
			ch >>=1;
		}
	}
	int64 nums = sec_nums%8;
	if(nums == 0)
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for (int j = 0; j < 8; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
			{
				LostSegments++;
			}
			ch >>= 1;
		}
	}
	else
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for(int j = 0; j < nums; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += m_pPmtDescriptor->fileDescriptor->SegmentLength;
				ReceiveSegments++;
			}
			else
				LostSegments++;
			ch >>= 1;
		}
	}
	m_ReciveLength = ReceiveBytes;
	m_ReciveSegment = ReceiveSegments;
	m_LostSegment = LostSegments;
	char str[512];
	if (gLog)
	{
		sprintf(str, "[FilmData] UpdateInfo: FilmID=%04X, PID=%04X, %s"
			, filmId
			, m_pPmtDescriptor->ElementaryPid
			, m_strFileName.c_str());
		gLog->Write(LOG_DVB, str);
		sprintf(str, "[FilmData] TotalSeg=%d, LostSeg=%d, CRCSeg=%d, SegLen=%d",
			m_TotalSegment,
			m_LostSegment,
			m_CRCError,
			m_pPmtDescriptor->fileDescriptor->SegmentLength);
		gLog->Write(LOG_DVB, str);
	}
}