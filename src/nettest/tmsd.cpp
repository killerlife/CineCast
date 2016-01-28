#include <unistd.h>
#include <stdio.h>
#include <netcomm/UiProtocal.h>
#include <log/Log.h>
#include "../netcomm/NetCommThread.h"
#include "../netcomm/BaseOperation.h"
#include "../dvb/PATDataProcess.h"
#include "ini.h"
#include <stdio.h>      /*fprintf(),stderr,BUFSIZ*/
#include <stdlib.h>     /**/
#include <string.h>     /*stderror()*/
#include <fcntl.h>      /*open(),flag*/
#include <errno.h>      /*errno*/
#include <unistd.h>     /*ssize_t*/
#include <sys/types.h>
#include <sys/stat.h>   /*mode_t*/

TUNER_INFO gInfo;
RECEIVE_INFO gRecv;
TUNER_CONF gTuner;
char bTunerChange = 0;
ILog* gLog;
char* gMd5 = NULL;
NetCommThread *pNetComm = NULL;
PATDataThread* pPat = NULL;

using namespace brunt;
#if 0
struct LostBuf
{
	uint32 m_size;
	uint8 *m_buf;
public:
	LostBuf()
	{
		m_size = 0;
		m_buf = NULL;
	}
	~LostBuf()
	{
		if (m_buf)
		{
			delete[] m_buf;
			m_size = 0;
		}
	}
};

std::string m_strFileName, m_strZtFileName, m_strReportName;
int64 m_nZtBufSize, m_sec_nums;
uint8 *m_pZtBuf = NULL;
FILE *m_pZtFilmFile;
uint64 m_ReciveLength, m_ReciveSegment, m_LostSegment, m_CRCError, m_TotalSegment;

struct LostBuf m_sLostBuf;

bool Init()
{
	//Create Save FileName
	m_strFileName = "/storage/disk/XUN-LONG-JUE-3D-ATMOS_FTR_F_CMN-QMS-EN_125M_51_2K_20151211_HXFILM_OV/XUN-LONG-JUE-3D-ATMOS_FTR_F_CMN-QMS-EN_125M_51_2K_20151211_HXFILM_OV_02.mxf";

	//Create lost report filename
	m_strZtFileName = m_strFileName + ".zt";

	struct stat s;
	int fd = open(m_strFileName.c_str(), O_RDONLY);
	fstat(fd, &s);
	close(fd);

	m_strReportName = m_strZtFileName;
	char* str = new char[m_strReportName.size()];
	memcpy(str, m_strReportName.c_str(), m_strReportName.size());
	char *pos;
	for (int i = 0; i < m_strReportName.size(); i++)
	{
		if(str[i] == '/')
			pos = str+i+1;
	}
	m_strReportName = pos;
	delete[] str;

	//Check lost file exist, and read it into buffer 
	int64 sec_nums = (s.st_size + 532 - 1) / 532;
	m_sec_nums = sec_nums;

	m_TotalSegment = sec_nums;

	sec_nums = (sec_nums + 7) / 8;


	if (m_pZtBuf != NULL)
	{
		delete[] m_pZtBuf;
	}

	m_nZtBufSize = sec_nums;


	m_pZtBuf = new uint8[m_nZtBufSize];
	m_sLostBuf.m_size = m_nZtBufSize
		+ 4 + 4 + 8 + 4 + 4 + 4 + 4 ;
	//ID + count + receive bytes + status + reserved + msg length + msg buffer + crc32
	m_sLostBuf.m_buf = new uint8[m_sLostBuf.m_size + 1024];

	memset(m_pZtBuf, 0, m_nZtBufSize);
	memset(m_sLostBuf.m_buf, 0, m_sLostBuf.m_size + 1024);

	FILE *fp;
	if((fp = fopen(m_strZtFileName.c_str(), "rb")) > 0)
	{
		fread(m_pZtBuf, 1, sec_nums, fp);
		fclose(fp);
	}
	return true;
}

struct LostBuf* GetLostSegment(NetCommThread& nc)
{
	int64 sec_nums = m_sec_nums;

	uint64 ReceiveBytes = 0;
	uint32 LostSegments = 0;
	uint32 ReceiveSegments = 0;

	L_LOST_INFO* pLost = (L_LOST_INFO*)m_sLostBuf.m_buf;
	uint32 nPos = (uint32)((uint8*)&(pLost->pLost) - (m_sLostBuf.m_buf));

	int i;
	for (i = 0; i < m_nZtBufSize - 1; i++)
	{
		uint8 ch;
		ch = m_sLostBuf.m_buf[nPos + i] = m_pZtBuf[i];
		for(int j = 0; j < 8; j++)
		{
			if(ch&0x1)
			{
				ReceiveBytes += 532;
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
				ReceiveBytes += 532;
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
				ReceiveBytes += 532;
				ReceiveSegments++;
			}
			else
				LostSegments++;
			ch >>= 1;
		}
	}

	uint32 nName = m_strReportName.size();
	memcpy(m_sLostBuf.m_buf+nPos + m_nZtBufSize, &nName, sizeof(uint32));
	memcpy(m_sLostBuf.m_buf+nPos + m_nZtBufSize + sizeof(uint32), m_strReportName.c_str(), nName);
	nName += (m_sLostBuf.m_size + sizeof(uint32));

	m_ReciveLength = ReceiveBytes;
	m_ReciveSegment = ReceiveSegments;
	m_LostSegment = LostSegments;
	L_LOST_INFO *info = (L_LOST_INFO*)m_sLostBuf.m_buf;
	info->filmID = 0xABCDEF01;
	info->lostNum = LostSegments;
	info->receivedByteCount = ReceiveBytes;
	info->recvState = 5;
	info->lostLength = m_nZtBufSize;
	info->reserved = 532;
	char str[512];
#if 0
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
#endif
	//if(pNetComm)
	{
		//TODO: send lost report to remote
		//--------------------------------
		nc.ReportLost((char*)info, m_sLostBuf.m_size, nName);
	}
	return &m_sLostBuf;
}
#endif

int main(int argc, char *argv[])
{
#if 0
	Init();
	gLog = CreateLog();

	NetCommThread nc;
	nc.Init(true);
	nc.Start();

	nc.StartRecvTask();
	nc.StartRoundRecv();
/*
	NetCommThread ncleonis;
	ncleonis.Init(true);
	ncleonis.Start();

	ncleonis.StartRecvTask();
	ncleonis.StartRoundRecv();
*/
	while(1)
	{
		sleep(5);
		nc.HeartBreat();
		GetLostSegment(nc);
//		ncleonis.HeartBreat();
	}
	ReleaseLog(gLog);
#else
#if 0
	char buf[2286];
	FILE *fp = fopen("1.xml", "rb");
	if(fp)
	{
		fread(buf, 2286, 1, fp);
		fclose(fp);
	}
	Md5Class* pMd5 = CreateMd5Class();
	gMd5 = buf;
	pMd5->Md5Parser(gMd5);
	if(pMd5->Md5Verify())
		printf("true\n");
	else
		printf("false\n");
#else
	ContentOperation co;
	std::vector<std::string> runList;
	runList.push_back("/storage/20150101/CRIFST/");
//	runList.push_back("/storage/tt/");

	co.AutoDelete(0, runList);
#endif
#endif
	return 0;
}