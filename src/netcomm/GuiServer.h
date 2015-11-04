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

	bool S_GetSatellite(char* buf);

private:
	int m_status;
	bool bStop;
};