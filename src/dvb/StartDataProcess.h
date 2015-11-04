#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include "demux.h"
#include "SatelliteRecv.h"

//class ILog;

class StartDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	StartDataThread();
	virtual ~StartDataThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Stop();
	uint64 ReciveLength(){ return 0; };
	uint64 FileLength(){return 0;};
	struct StartDescriptor* GetStartDescriptor(){ return m_pStartDescriptor; };
	struct FileDescriptor* GetFileDescriptor(){ return m_pFileDescriptor; };
	int GetStatus() { return m_status; };

private:
	virtual void doit();

private:
	int m_status;
	uint16 m_pid;
	struct StartDescriptor* m_pStartDescriptor;
	struct FileDescriptor* m_pFileDescriptor;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
//	ILog* pLog;
};