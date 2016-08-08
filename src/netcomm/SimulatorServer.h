#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"
#include "../dvb/mytype.h"

class SimulatorThread;

class SimulatorServer: public brunt::CActiveThread
{
public:
	SimulatorServer();
	virtual ~SimulatorServer();
	bool Init();
	bool Start();
	bool Stop();
	int GetStatus() {return m_status;};

private:
	virtual void doit();

private:
	int m_status;
	CZSocket m_SrvSocket;
	std::list<SimulatorThread*> m_listClient;
	SimulatorThread *m_pThread;
};

class SimulatorThread: public brunt::CActiveThread
{
public:
	SimulatorThread();
	virtual ~SimulatorThread();
	bool Init();
	bool Start();
	bool Stop();
	int GetStatus() {return m_status;};
	bool isStop() {return bStop;};

	bool Read(char* buf, int rSize, int& nRead);
	bool Write(char* buf, int wSize, int& nWrite);

	bool UiProcessFilter();

public:
	CZSocket m_socket;

private:
	virtual void doit();

	bool NotifyProc(char* buf);
	bool StartProc(char* buf);
	bool FinishProc(char* buf);
	bool CancelProc(char* buf);
	bool PatProc(char* buf);
	bool PmtProc(char* buf);
	bool DataProc(char* buf);
	bool ErrorProtocol(char* buf);
		
private:
	uint32* pDebugCmd;
	int m_status;
	bool bStop;
};

SimulatorServer* CreateSimulatorServer();
void ReleaseSimulatorServer(SimulatorServer* pGui);
