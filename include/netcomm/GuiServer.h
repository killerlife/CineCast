#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"

class GuiThread;

class GuiServer: public brunt::CActiveThread
{
public:
	GuiServer();
	virtual ~GuiServer();
	bool Init();
	bool Start();
	bool Stop();
	int GetStatus() {return m_status;};

private:
	virtual void doit();

private:
	int m_status;
	CZSocket m_SrvSocket;
	std::list<GuiThread*> m_listClient;
	GuiThread *m_pThread;
};

class GuiThread: public brunt::CActiveThread
{
public:
	GuiThread();
	virtual ~GuiThread();
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

	bool ErrorProtocol(char* buf);
	bool S_GetSatellite(char* buf);
	bool S_GetReceive(char* buf);
	bool C_GetTuner(char* buf);
	bool C_SetTuner(char* buf);
	bool N_GetConfig(char* buf);
	bool N_SetConfig(char* buf);
	bool N_GetRemote(char* buf);
	bool N_SetRemote(char* buf);

private:
	int m_status;
	bool bStop;
};