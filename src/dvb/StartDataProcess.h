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
	struct StartDescriptor* GetStartDescriptor(){ return (StartDescriptor*)m_pStartDescriptor; };
	struct FileDescriptor* GetFileDescriptor(){ return (FileDescriptor*)m_pFileDescriptor; };
	int GetStatus() { return m_status; };
	std::string GetFilmName() {return m_filmName;};
	std::string GetUUID() {return m_uuid;};
	std::string GetCreator() {return m_creator;};
	std::string GetIssuer() {return m_issuer;};
	std::string GetIssueDate() {return m_issueDate;};
	bool IsStart();

private:
	virtual void doit();

private:
	int m_status;
	uint16 m_pid;
	volatile struct StartDescriptor* m_pStartDescriptor;
	volatile struct FileDescriptor* m_pFileDescriptor;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	std::string m_filmName, m_uuid, m_creator, m_issuer, m_issueDate;
	bool bStart;
//	ILog* pLog;
};
StartDataThread* CreateStart();
void ReleaseStart(StartDataThread* pStart);
