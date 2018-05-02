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
#include <unistd.h>
#include <sys/stat.h>
#include <netcomm/message.h>
#include "ini.h"
#include "../externcall/ExternCall.h"

extern ILog* gLog;
extern NetCommThread *pNetComm;
extern RECEIVE_INFO gRecv;

extern char strDemux[1024];

bool gFilmDataFlag = false;
#if SIMULATOR
extern char SimDataBuf[10][4096];
extern int SimBufPos;
#endif

bool *CreateFilmDataFlag()
{
	return &gFilmDataFlag;
}

PATDataThread::PATDataThread():
	m_status(0),
	m_PatId(0xfe),
	m_pManager(NULL),
	bIdle(false),
	nLostSegment(0),
	nTotalSegment(0),
	nCrc(0),
	nReceiveSegment(0),
	nFileLength(0),
	nReceiveLength(0),
	bReset(false)
{
	m_pFilter = new Filter;
	pDebugCmd = GetDebugCommand();
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
// 	m_pFilter->SetStrDevName("/dev/dvb/adapter0/demux0");
	m_pFilter->SetStrDevName(strDemux);
	m_pFilter->SetFilterID(m_PatId, 0x00);
	return true;
	//Use Start function to start thread.
	//return Start();
}

bool PATDataThread::Start()
{
	if(m_status != RUN)
	{
		nLostSegment = 0;
		nTotalSegment = 0;
		nCrc = 0;
		nReceiveSegment = 0;
		nFileLength = 0;
		nReceiveLength = 0;
	m_status = RUN;
	if(gLog)
		gLog->Write(LOG_DVB, "[PATDataThread] Start: Run.");
		bReset = false;
#if 0 //This patch will make the receive stop after one round finished

		//////////////////////////////////////////////////////////////////////////
		// PreCreate PMT thread
		if(m_pManager == NULL)
			m_pManager = brunt::createThreadManager();
		if(m_pmtIdList.empty())
		{
			for(int i = 0x21; i<0x26; i++)
			{
				m_pmtIdList.push_back(i);

				char str[200];
				sprintf(str, "[PAT Descriptor] Create PMT Descriptor: %d", i);
				//pLog->Write(LOG_DVB, str);
				//syslog(LOG_INFO|LOG_USER, "[PAT Descriptor] Create PMT Descriptor: %d", pmtId); 
				PMTDataThread *thread = new PMTDataThread;
				thread->Init(&i, NULL);
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
		//////////////////////////////////////////////////////////////////////////
#endif
	}
	return true;
}

bool PATDataThread::Reset(bool bFinish)
{
	m_status = IDLE;
	if(gLog && bReset == false)
		gLog->Write(LOG_DVB, "[PATDataThread] Reset: Processing.");

	std::list<PMTDataThread*>::iterator itor;
	int i = 0;
	char str[50];
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor, i++)
	{
		PMTDataThread *pmtThread = *itor;
		pmtThread->Stop();
		if(gLog && bReset == false)
		{
			sprintf(str, "[PATDataThread] Reset: %d:%d PMT stopped", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
		if(bFinish)
		{
		pmtThread->FinishDCP();
			if(gLog && bReset == false)
		{
			sprintf(str, "[PATDataThread] Reset: %d Finish DCP", i);
			gLog->Write(LOG_DVB, str);
		}
		}

		delete pmtThread;
		if(gLog && bReset == false)
		{
			sprintf(str, "[PATDataThread] Reset: %d:%d PMT deleted", i, m_pmtList.size());
			gLog->Write(LOG_DVB, str);
		}
	}
	m_pmtList.clear();
	m_pmtIdList.clear();
	if(gLog && bReset == false)
		gLog->Write(LOG_DVB, "[PATDataThread] Reset: All Clear.");
	while(bIdle == false)
	{
		usleep(2000);
	}
	bReset = true;
	//Disable for change function to reset
	//m_status = RUN;
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
#if SIMULATOR
extern void print_hex(char* buf, int len);
#endif

void PATDataThread::doit()
{
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
#if SIMULATOR
			if((*pDebugCmd) == D_SIMULATOR)
			{
// 				DPRINTF("PAT SIM\n");
				char *pos = &SimDataBuf[SimBufPos?(SimBufPos-1)%10:9][0];
				if((*(uint16*)pos) == 0xfe)
				{
					uint16 len = getBits((uint8*)pos+2, 12, 12);
// 					DPRINTF("len %d\n", len);
// 					print_hex(pos+2, len + 3);
					memcpy(m_buffer, pos + 2, len + 10);
					*(uint16*)pos = 0x00;
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
// 								DPRINTF("PN %d\n", program_number);
								if (program_number != 0)
								{
									uint16 pmtId = ((pbuf[2] & 0x1f) << 8) |
										pbuf[3];
// 									DPRINTF("pmtID %04x\n", pmtId);
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
										if(pmtId != 0x20)
										{
											break;
										}

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
						else
						{
							DPRINTF("CRC  %08x %08x error\n", crc, crc1);
						}
					}
				}
				usleep(1000);
			}
			else
#endif
			if (m_pFilter->ReadFilter(m_buffer, count))
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
			else
			{
#if SIMULATOR
				//--------------------------------------------------------
				//                Network Simulator
				if(*(pDebugCmd) == D_PAT)
				{
					m_bPat = true;
					std::list<uint16>::iterator itor;
					bool bFind = false;
					uint16 pmtId = 0x20;
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
						brunt::ThreadRunPolicy policy;
						policy.order = 0;
						policy.policy = brunt::POLICY_ERROR_EXIT;

						brunt::CThreadRunInfo threadInfo(thread, policy);
						m_pManager->addThread(threadInfo); 
						m_pManager->setParallelNum(0, m_pmtIdList.size());
						m_pManager->run();
						m_pmtList.push_back(thread);
					}
					*pDebugCmd = 0;
				}
				//--------------------------------------------------------
#endif
			}
			break;
		case STOP:
			bIdle = false;
			if(m_pManager != NULL)
			{
			m_pManager->terminate();
			brunt::releaseThreadManager(m_pManager);
			m_pManager = NULL;
				if(gLog)
					gLog->Write(LOG_DVB, "[PATDataThread] Stop: Release ThreadManager.");
			}
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

void PATDataThread::Clear()
{
	nLostSegment = nTotalSegment = nCrc = nReceiveSegment = nFileLength = nReceiveLength = 0;
}

uint64 PATDataThread::ReciveLength()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nReceiveLength = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nReceiveLength += (*itor)->ReciveLength();
	}
	}
	return nReceiveLength;
}

uint64 PATDataThread::FileLength()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nFileLength = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nFileLength += (*itor)->FileLength();
	}
	}
	return nFileLength;
}

uint64 PATDataThread::ReciveSegment()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nReceiveSegment = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nReceiveSegment += (*itor)->ReciveSegment();
	}
	}
	return nReceiveSegment;
}

uint64 PATDataThread::LostSegment()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nLostSegment = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nLostSegment += (*itor)->LostSegment();
	}
	}
	return nLostSegment;
}

uint64 PATDataThread::CRCError()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nCrc = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nCrc += (*itor)->CRCError();
	}
	}
	return nCrc;
}

uint64 PATDataThread::TotalSegment()
{
// 	uint64 length = 0;
	std::list<PMTDataThread*>::iterator itor;
	if(m_pmtList.size() > 0)
	{
		nTotalSegment = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nTotalSegment += (*itor)->TotalSegment();
		}
	}
	return nTotalSegment;
}

uint64 PATDataThread::GetLostSegment()
{
	std::list<PMTDataThread*>::iterator itor;
// 	uint64 nLostCount = 0;
	m_strReportFileList = "";
	bool bFound = false;
	uint32 id = 0;
	if(m_pmtList.size() > 0)
	{
		nLostSegment = 0;
	for (itor = m_pmtList.begin(); itor != m_pmtList.end(); ++itor)
	{
			nLostSegment +=(*itor)->GetLostSegment();
		m_strReportFileList += (*itor)->GetReportFileList();
		m_strReportFileList += " ";
		if((id = (*itor)->GetFilmId()) != 0)
		{
		    bFound = true;
		    m_FilmId = id;
		}
		//m_FilmId = (*itor)->GetFilmId();
	}
	}
	if(!bFound)
	    m_FilmId = gRecv.nFileID;
	return nLostSegment;
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

	//====================================================================
	//Change work directory to "/storage"
	chdir("/storage");
	//====================================================================

	sprintf(str, "/tmp/lost%ld.zip ", m_FilmId);
	std::string cmd = "/bin/rm -rf ";
	cmd = cmd + str;
#if 0
	//////////////////////////////////////////////////////////////////////////
	// If use SuperDog, it BLOCK system call !!!! [10/10/2017 killerlife]
	// So we use ExternCall class to instead it.
	system(cmd.c_str());
#else
	IExternCall *pEc = CreateExternCall();
	pEc->RunCommand(cmd.c_str());
	while(!pEc->IsFinish())
	{
		usleep(200000);
	}
#endif
	if(gLog)
		gLog->Write(LOG_SYSTEM, cmd.c_str());
	
	//Construct zip command and zip all zt file to one archive file
	if(m_strReportFileList.find(".zt") == std::string::npos)
	{
	    return SendLostReport(m_FilmId);
	}
	cmd = "/bin/zip ";
	cmd = cmd + str + m_strReportFileList;
#if 1
// 	IExternCall *pEc = CreateExternCall();
	pEc->RunCommand(cmd.c_str());
	while(!pEc->IsFinish())
	{
		usleep(200000);
	}
	if(gLog)
		gLog->Write(LOG_SYSTEM, pEc->GetOutput().c_str());
#else
	system(cmd.c_str());
	if(gLog)
		gLog->Write(LOG_SYSTEM, cmd.c_str());
#endif
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