#include "ExternCall.h"
#include <thread/activeThread/activeThread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <dvb/mytype.h>

using namespace brunt;

class ExternCall: public IExternCall, public brunt::CActiveThread
{
public:
	ExternCall();
	virtual ~ExternCall();
	std::string GetOutput();
	void SetInput(std::string strInput);
	bool IsFinish();
	void SetCommandLine(std::string strCmdLine);
	bool RunCommand(std::string strCmdLine);
	bool RunCommand();

private:
	virtual void doit();

private:
	char buf[200];
	bool m_bFinish;
	std::string m_strCmd;
	std::string m_strOut;
};

ExternCall gExternCall;

IExternCall* CreateExternCall()
{
	return &gExternCall;
}

void ReleaseExternCall(IExternCall* extCall)
{

}

ExternCall::ExternCall():m_bFinish(true)
{

}

ExternCall::~ExternCall()
{
	CActiveThread::stop();
}

void ExternCall::SetCommandLine(std::string strCmdLine)
{
	m_strCmd = strCmdLine;
}

bool ExternCall::RunCommand(std::string strCmdLine)
{
	m_strCmd = strCmdLine;
	return RunCommand();
}

bool ExternCall::RunCommand()
{
	char str[256];
	if (!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[ExternCall] RunCommand: Thread not ready, status = %d.", status());
		DPRINTF("%s\n", str);
		return false;
	}

	if (isStopped())
	{
		CActiveThread::stop();
	}

	if (!start())
	{
		sprintf(str, "[ExternCall] RunCommand: Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		return false;
	}
	m_bFinish = false;
	return true;
}

void ExternCall::doit()
{
	FILE *fp = popen(m_strCmd.c_str(), "r");
	if (fp <= 0)
	{
		DPRINTF("Open command error\n");
	}
	setvbuf(fp, NULL, _IONBF, 0);
	m_strOut.clear();
	memset(buf, 0, 200);
	char* pOut = NULL;
	while((pOut = fgets(buf, 200, fp)) != NULL)
	{
		std::string s(pOut);
		m_strOut += s;
// 		DPRINTF("%s", m_strOut.c_str());
		memset(buf, 0, 200);
	}
	m_bFinish = true;
	pclose(fp);
}

bool ExternCall::IsFinish()
{
	return m_bFinish;
}

void ExternCall::SetInput(std::string strInput)
{

}

std::string ExternCall::GetOutput()
{
// 	DPRINTF("out:%s\n", m_strOut.c_str());
	return m_strOut;
}