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
	bool Stop();
	uint64 ReciveLength();
	uint64 FileLength();
	uint64 ReciveSegment();
	uint64 LostSegment();
	uint64 CRCError();
	uint64 TotalSegment();
	
	int GetStatus() { return m_status; };

	uint64 GetLostSegment();

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
// 	ILog *pLog;
};