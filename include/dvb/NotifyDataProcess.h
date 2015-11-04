#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include "demux.h"
#include "SatelliteRecv.h"

class NotifyDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	NotifyDataThread();
	virtual ~NotifyDataThread();

	//param1 is PID of Notify section (uint16)
	//param2 is Machine ID (uint32)
	bool Init(void *param1, void *param2);

	bool Start();
	bool Stop();
	bool IsNotify() { return bMatch; };
	int GetStatus() { return m_status; };
	uint64 ReciveLength(){ return 0; };
	uint64 FileLength(){return 0;};

private:
	virtual void doit();

private:
	int m_status;
	bool bMatch;
	uint16 m_pid;
	uint32 m_machineId;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
};