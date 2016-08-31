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
	bool NotifyProc(char* buf);
	bool StartProc(char* buf);
	bool FinishProc(char* buf);
	bool CancelProc(char* buf);
	bool PatProc(char* buf);
	bool PmtProc(char* buf);
	bool DataProc(char* buf);
	bool ErrorProtocol(char* buf);

private:
	int m_status;
	CZSocket m_SrvSocket;
	std::list<SimulatorThread*> m_listClient;
	SimulatorThread *m_pThread;
};

SimulatorServer* CreateSimulatorServer();
void ReleaseSimulatorServer(SimulatorServer* pGui);
