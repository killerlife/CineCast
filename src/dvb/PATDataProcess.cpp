#include "PATDataProcess.h"
#include "../netcomm/NetCommThread.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <log/Log.h>
#include <sys/stat.h>
#include <netcomm/message.h>
#include "ini.h"

extern ILog* gLog;
extern NetCommThread *pNetComm;
extern RECEIVE_INFO gRecv;
bool gFilmDataFlag = false;

bool *CreateFilmDataFlag()
{
	return &gFilmDataFlag;
}

PATDataThread::PATDataThread():m_status(0), m_PatId(0xfe), m_pManager(NULL), bIdle(false)
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
	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetFilterID(m_PatId, 0x00);
	return Start();
}

bool PATDataThread::Start()
{
	m_status = RUN;
	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Start: Run.");
	return true;
}

bool PATDataThread::Restart()
{
	m_status = IDLE;
	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Restart: Processing.");

	std::list<PMTDataThread*>::iterator itor;
	int i = 0;
	char str[50];
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor, i++)
	{
		PMTDataThread *pmtThread = *itor;
		pmtThread->Stop();
		if(gLog)
		{
			sprintf(str, "[PATDataThread] Restart: %d:%d PMT stopped", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
		delete pmtThread;
		if(gLog)
		{
			sprintf(str, "[PATDataThread] Restart: %d:%d PMT deleted", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
	}
	m_pmtList.clear();
	m_pmtIdList.clear();
	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Restart: All Clear.");
	while(bIdle == false)
	{
		usleep(200);
	}
	m_status = RUN;
}
bool PATDataThread::IsPat()
{
    bool bRes = m_bPat;
    m_bPat = false;
    return bRes;
}

bool PATDataThread::Stop()
{
	if(m_status == STOP)
		return true;

	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Stop: Processing.");

	m_status = STOP;

	m_pFilter->Stop();

	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Stop: Filter stopped.");

	std::list<PMTDataThread*>::iterator itor;
	int i = 0;
	char str[50];
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor, i++)
	{
		PMTDataThread *pmtThread = *itor;
		pmtThread->Stop();
		if(gLog)
		{
			sprintf(str, "[PATDataThread] Stop: %d:%d PMT stopped", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
		delete pmtThread;
		if(gLog)
		{
			sprintf(str, "[PATDataThread] Stop: %d:%d PMT deleted", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
	}
	m_pmtList.clear();
	m_pmtIdList.clear();
	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Stop: All Clear.");

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

	while(1)
	{
		switch(m_status)
		{
		case RUN:
			uint16 count;
			count = 4096;
			bIdle = false;
			if(m_pManager == NULL)
				m_pManager = brunt::createThreadManager();

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
					
					m_bPat = true;
					
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
			bIdle = false;
			if(m_pManager != NULL)
			{
			m_pManager->terminate();
			brunt::releaseThreadManager(m_pManager);
			m_pManager = NULL;
				if(gLog)
					gLog->Write(LOG_DVB, "[PATDataThread] Stop: Release ThreadManager.");
			}
#endif // 0
			return;
		case IDLE:
			if(m_pManager != NULL)
			{
				m_pManager->terminate();
				brunt::releaseThreadManager(m_pManager);
				m_pManager = NULL;
				if(gLog)
					gLog->Write(LOG_DVB, "[PATDataThread] IDLE: Release ThreadManager.");
			}
			bIdle = true;
			usleep(200);
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

uint64 PATDataThread::GetLostSegment()
{
	std::list<PMTDataThread*>::iterator itor;
	uint64 nLostCount = 0;
	m_strReportFileList = "";
	bool bFound = false;
	uint32 id = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		nLostCount +=(*itor)->GetLostSegment();
		m_strReportFileList += (*itor)->GetReportFileList();
		m_strReportFileList += " ";
		if((id = (*itor)->GetFilmId()) != 0)
		{
		    bFound = true;
		    m_FilmId = id;
		}
		//m_FilmId = (*itor)->GetFilmId();
	}
	if(!bFound)
	    m_FilmId = gRecv.nFileID;
	return nLostCount;
}

bool PATDataThread::UnzipSubtitle()
{
	std::list<PMTDataThread*>::iterator itor;
	for(itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		if((*itor)->UnzipSubtitle())
			return true;
	}
	return false;
}

using namespace brunt;

bool PATDataThread::SendLostReport()
{
	char str[200], str1[200];

	sprintf(str, "/tmp/lost%ld.zip ", m_FilmId);
	std::string cmd = "rm -rf ";
	cmd = cmd + str;
	system(cmd.c_str());
	
	//Construct zip command and zip all zt file to one archive file
	if(m_strReportFileList.find(".zt") == std::string::npos)
	{
	    return SendLostReport(m_FilmId);
	}
	cmd = "zip ";
	cmd = cmd + str + m_strReportFileList;
	system(cmd.c_str());

	struct stat mstat;
	sprintf(str, "/tmp/lost%ld.zip", m_FilmId);
	if (stat(str, &mstat) == -1)
	{
		sprintf(str1, "[PATDataThread] Unable stat %s.", str);
		if(gLog)
			gLog->Write(LOG_ERROR, str1);
		DPRINTF("%s\n", str1);
		return false;
	}

	char *buf = new char[mstat.st_size + 100];
	L_LOST_INFO *info = (L_LOST_INFO*) buf;
	char *pos = buf + 28;//(char*)&info->pLost;
	
	sprintf(str, "/tmp/lost%ld.zip", m_FilmId);
	FILE *fp = fopen(str, "rb");
	if(fp <= 0)
	{
		sprintf(str1, "[PATDataThread] Unable open %s.", str);
		if(gLog)
			gLog->Write(LOG_ERROR, str1);
		DPRINTF("%s\n", str1);
		delete[] buf;
		return false;
	}
	fread(pos, mstat.st_size, 1, fp);
	info->filmID = m_FilmId;
	info->lostNum = LostSegment();
	info->receivedByteCount = ReciveLength();
	info->recvState = 5;
	info->lostLength = mstat.st_size;
	info->reserved = 0;

	ICMyini *ini = createMyini();
	if(ini)
	{
	    sprintf(str, "%ld", info->filmID);
	    ini->write(" ", "FILMID", str);
	    sprintf(str, "%lld", info->lostNum);
	    ini->write(" ", "LOST", str);
	    sprintf(str, "%lld", info->receivedByteCount);
	    ini->write(" ", "RECVBYTE", str);
	    sprintf(str, "%ld", info->recvState);
	    ini->write(" ", "STATUS", str);
	    sprintf(str, "/tmp/lost%ld.info", m_FilmId);
	    ini->save(str);
	    releaseMyini(ini);
	}
	
	if(pNetComm)
	{
		pNetComm->ReportLost(buf, 
			28 + mstat.st_size + sizeof(uint32),
			28 + mstat.st_size + sizeof(uint32));
	}

	delete[] buf;
	return true;
}

extern RECEIVE_INFO gRecv;

bool PATDataThread::SendLostReport(uint32 filmId)
{
	char str[200], str1[200];

	//Construct zip command and zip all zt file to one archive file
	sprintf(str, "/tmp/lost%ld.zip", filmId);

	char *buf;
	L_LOST_INFO *info;
	struct stat mstat;
	uint32 zipsize = 0;
	if (stat(str, &mstat) == -1)
	{
		sprintf(str1, "[PATDataThread] SendLostReport(id) Unable stat %s.", str);
		if(gLog)
			gLog->Write(LOG_ERROR, str1);
		DPRINTF("%s\n", str1);
		buf = new char[100];
		
		info = (L_LOST_INFO*) buf;
		char *pos = buf + 28;
	
		info->filmID = filmId;
		info->lostNum = LostSegment();
		info->receivedByteCount = ReciveLength();
		info->recvState = 5;
		info->lostLength = zipsize = 0;
		info->reserved = 0;
	}
	else
	{
	    buf = new char[mstat.st_size + 100];
	    info = (L_LOST_INFO*) buf;
	    char *pos = buf + 28;//(char*)&info->pLost;
	
		sprintf(str, "/tmp/lost%ld.zip", filmId);
		FILE *fp = fopen(str, "rb");
		if(fp <= 0)
		{
			sprintf(str1, "[PATDataThread] SendLostReport(id): Unable open %s.", str);
			if(gLog)
				gLog->Write(LOG_ERROR, str1);
			DPRINTF("%s\n", str1);
			info->filmID = filmId;
			info->lostNum = LostSegment();
			info->receivedByteCount = ReciveLength();
			info->recvState = 5;
			info->lostLength = zipsize = 0;
			info->reserved = 0;
		}
		else
		{
			fread(pos, mstat.st_size, 1, fp);

			sprintf(str, "/tmp/lost%ld.info", filmId);
			ICMyini *ini = createMyini();
			if(ini)
			{
			    if(ini->load(str))
			    {
				std::string tmp;
				if(ini->read(" ", "LOST", tmp))
				    info->lostNum = atol(tmp.c_str());
				else
				    info->lostNum = LostSegment();
				if(ini->read(" ", "RECVBYTE", tmp))
				    info->receivedByteCount = atoll(tmp.c_str());
				else
				    info->receivedByteCount = ReciveLength();
			    }
			    else
			    {
				info->lostNum = LostSegment();
				info->receivedByteCount = ReciveLength();
			    }
			}
			else
			{
				info->lostNum = LostSegment();
				info->receivedByteCount = ReciveLength();
			}
			info->filmID = filmId;
			info->recvState = gRecv.nReceiveStatus & 0xffff;
			info->lostLength = zipsize = mstat.st_size;
			info->reserved = 0;
		}

	}
	
	if(pNetComm)
	{
		pNetComm->ReportLost(buf, 
			28 + zipsize + sizeof(uint32),
			28 + zipsize + sizeof(uint32));
		RoundCleanCounter();
	}

	delete[] buf;
	return true;
}

bool PATDataThread::RoundCleanCounter()
{
	std::list<PMTDataThread*>::iterator itor;
	for(itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		if((*itor)-RoundCleanCounter())
			return true;
	}
	return false;
}

bool PATDataThread::SaveData(char* fn, char* pData, uint32 segNum, uint32 len)
{
	std::list<PMTDataThread*>::iterator itor;
	for(itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		if((*itor)->SaveData(fn, pData, segNum, len))
			return true;
	}
	return false;
}

bool PATDataThread::IsPmtReady()
{
	std::list<PMTDataThread*>::iterator itor;
	if (m_pmtList.size() < 1)
		return false;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
		if(!(*itor)->IsFilmDataReady())
			return false;
	}
	return true;
}