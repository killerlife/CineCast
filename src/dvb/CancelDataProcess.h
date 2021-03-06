﻿#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include "demux.h"
#include "SatelliteRecv.h"

// class ILog;

class CancelDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	CancelDataThread();
	virtual ~CancelDataThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Stop();
	bool IsCancel() { return m_bCancel; };
	void ClearCancel();
	int GetStatus() { return m_status; };
	uint64 ReciveLength(){ return 0; };
	uint64 FileLength(){return 0;};

private:
	virtual void doit();

private:
	bool m_bCancel;
	int m_status;
	uint16 m_pid;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	uint32* pDebugCmd;
	//ILog* pLog;
};

CancelDataThread* CreateCancel();
void ReleaseCancel(CancelDataThread* pCancel);