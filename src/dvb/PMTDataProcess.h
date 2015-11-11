#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include "demux.h"
#include "SatelliteRecv.h"
#include "FilmDataProcess.h"
#include <thread/activeThread/activeThreadManager_i.h>

//class ILog;

class PMTDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	PMTDataThread();
	virtual ~PMTDataThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Stop();
	uint64 ReciveLength();
	uint64 FileLength();
	int GetStatus() { return m_status; };

	uint64 ReciveSegment();
	uint64 LostSegment();
	uint64 CRCError();
	uint64 TotalSegment();

	uint64 GetLostSegment();

private:
	virtual void doit();

private:
	int m_status;
	std::list<struct PmtDescriptor*> m_pmtList;
	std::list<FilmDataThread*> m_filmList;
	uint16 m_PmtId;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	brunt::IThreadManager* m_pManager;
	struct ProgramDescriptor m_programDescriptor;
	bool bThreadStart;
// 	ILog *pLog;
};