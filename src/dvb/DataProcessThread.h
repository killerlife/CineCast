#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include "demux.h"
#include "SatelliteRecv.h"
#include <stdio.h>
#include <stdlib.h>

class FilmDataThread;
//class ILog;

class DataProcessThread: public brunt::CActiveThread
{
public:
	DataProcessThread();
	virtual ~DataProcessThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Stop();
	int GetStatus() { return m_status; };
	bool isStop() { return bStop; };

public:
	volatile int m_status;

private:
	virtual void doit();

private:
	FilmDataThread* pFilmThread;
	int32 m_id;
	bool bStop;
// 	ILog *pLog;
};
