#include "FilmDataProcess.h"
#include "DataProcessThread.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// #include <log/Log.h>

DataProcessThread::DataProcessThread():m_status(0), pFilmThread(NULL)
{
	//pLog = CreateLog();
}

DataProcessThread::~DataProcessThread()
{
	//Stop();
	//ReleaseLog(pLog);
}

bool DataProcessThread::Init(void *param1, void *param2)
{
	pFilmThread = (FilmDataThread*)param1;
	//m_id = (int32)pFilmThread;
	return Start();
}

bool DataProcessThread::Start()
{
	Stop();
	m_status = RUN;
	//syslog(LOG_INFO|LOG_USER, "Data Process[%llx] Thread Start\n", m_id);
	bStop = false;
	return true;
}

bool DataProcessThread::Stop()
{
	if(m_status == STOP)
		return true;
	DPRINTF("Call Stop\n");
	m_status = STOP;
	//syslog(LOG_INFO|LOG_USER, "Data Process[%llx] Thread Stop\n", m_id);
	usleep(100000);
	return true;
}

void DataProcessThread::doit()
{
#if 1
	char str[200];
	sprintf(str, "[DataProcess] Run 0x%x", this);
	//pLog->Write(LOG_DVB, str);

#ifdef USE_SIM
	uint8 cc = 0;
	while(1)
#else
	while(1)
#endif
	{
		switch(m_status)
		{
		case RUN:
			{
				//DPRINTF("run [%llx] \n", this);
#if 1
				struct FilmDataBuf* pBuf;
				while(1) 
				{
					if (m_status == STOP)
					{
						DPRINTF("Dataprocess stop %x\n", this);

						bStop = true;
						return;
					}
					pBuf = pFilmThread->GetDataBuf();
					if (pBuf == NULL)
						usleep(1);
					else
					{
						if (m_status == STOP)
						{
							pFilmThread->PushIntoFreeBuf(pBuf);

							DPRINTF("Dataprocess stop %x\n", this);

							bStop = true;
							return;
						}
						break;
					}
				}
#if 1
				uint16 len = getBits(pBuf->pBuf, 12, 12);
				uint32 crc = calc_crc32(pBuf->pBuf, len -1) & 0xffffffff;
				uint32 crc1 = ((*((pBuf->pBuf + len - 1)) << 24)|
					(*((pBuf->pBuf + len)) << 16) |
					(*((pBuf->pBuf + len + 1)) << 8) |
					(*((pBuf->pBuf + len + 2))))  & 0xffffffff;

				//do crc32 check
				if (crc == crc1)
				{
					//Get segment number and construct file position
					uint16 w_size;
#ifdef USE_SIM
					uint32 seg_num = cc;
					cc++;
					uint16 pl = getBits(pBuf->pBuf + 1, 4, 12);
					uint16 pll = *(pBuf->pBuf + 4);
					w_size = pl - pll - 6;
#else
					uint32 seg_num = getBits(pBuf->pBuf+11, 0, 32);
					uint16 pl = getBits(pBuf->pBuf + 1, 4, 12);
					uint16 pll = *(pBuf->pBuf + 4);
					w_size = pl - pll - 6;
#endif
					uint64 pos = seg_num * pFilmThread->m_pPmtDescriptor->fileDescriptor->SegmentLength;

					//write data to file
					//if (pFilmThread->m_pFilmFile > 0)
					{
// 						DPRINTF("-------------------------------------------------->\nwrite %llX to %s\n%llX %X\n",
// 							pos,
// 							pFilmThread->m_pPmtDescriptor->fileDescriptor->FileName,
// 							pFilmThread->m_pPmtDescriptor->fileDescriptor->FileLength,
// 							seg_num);
#if 1
						// 							fseek(m_pFilmFile, 0, SEEK_SET);
// 						fseek(pFilmThread->m_pFilmFile, pos, SEEK_SET);
// 						fwrite(pBuf->pBuf + 15,
// 							1,
// 							w_size,
// 							pFilmThread->m_pFilmFile);
						//fflush(m_pFilmFile);
						pFilmThread->WriteFile(pos,
							pBuf->pBuf + 15,
							w_size);
  						pFilmThread->UpdateZtMem(seg_num);

						//this only for test
						//UpdateZtFile();
						//
#endif
						pFilmThread->m_ReciveLength += w_size;
					}
				}
#endif
				pFilmThread->PushIntoFreeBuf(pBuf);
#endif
			}
			break;
		case STOP:
			DPRINTF("stop status %x\n", this);
			bStop = true;
			return;
		case IDLE:
			break;
		}
	}
#endif
}

