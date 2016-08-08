#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include "demux.h"
#include "SatelliteRecv.h"
#include "PMTDataProcess.h"
#include <thread/activeThread/activeThreadManager_i.h>

//class ILog;

class PATDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	PATDataThread();
	virtual ~PATDataThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Reset(bool bFinish = true);
	bool Stop();
	void Clear();
	uint64 ReciveLength();
	uint64 FileLength();
	uint64 ReciveSegment();
	uint64 LostSegment();
	uint64 CRCError();
	uint64 TotalSegment();
	
	int GetStatus() { return m_status; };

	uint64 GetLostSegment(); //Use to calc lost segment/receive segment, and generate lost info struct
	
	bool UnzipSubtitle(); //find and unzip subtitle zip file

	bool SendLostReport();
	bool SendLostReport(uint32 filmId);

	bool SaveData(char* fn, char* pData, uint32 segNum, uint32 len);

	bool IsPmtReady();
	bool IsPat();
	bool RoundCleanCounter();

private:
	virtual void doit();

private:
	int m_status;
	std::list<uint16> m_pmtIdList;
	std::list<PMTDataThread*> m_pmtList;
	uint16 m_PatId;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	brunt::IThreadManager* m_pManager;
	std::string m_strReportFileList;
	uint32 m_FilmId;
	volatile bool m_bPat;
	bool bIdle;
	uint32* pDebugCmd;
	uint64 nLostSegment;
	uint64 nTotalSegment;
	uint64 nCrc;
	uint64 nReceiveSegment;
	uint64 nFileLength;
	uint64 nReceiveLength;
// 	ILog *pLog;
};

bool *CreateFilmDataFlag();
