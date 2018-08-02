#ifdef AUREOLAM
#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <dvb/demux.h>
#include <thread/activeThread/activeThreadManager_i.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include "message.h"

class AureoLamThread: public brunt::CActiveThread
{
public:
	AureoLamThread();
	virtual ~AureoLamThread();

	bool Init();
	bool Start();
	bool Stop();
	bool HeartBreat();
	void StartRoundRecv();
	void StartRecvTask();
	void ResetRecvTask();
	void ResetRoundRecv();

private:
	virtual void doit();
	void cleanThread();

private:
	int m_status;
	std::string m_strTaskTime;		// ����ʼʱ�� [7/31/2018 jaontolt]
	std::string m_strRoundTime;		// ���ֿ�ʼʱ�� [7/31/2018 jaontolt]
	std::string m_strAureoLamNS;	// �⻷HTTP SERVICE��ַ [7/31/2018 jaontolt]
	bool bExit;						// �߳��˳���־ [7/31/2018 jaontolt]
	uint64 nRecvLength;				// �ѽ��ճ��ȣ����ڼ����������� [7/31/2018 jaontolt]
	int m_logCount;					// ��־����������ֹ�������ű�Ӳ�� [8/1/2018 jaontolt]
};

AureoLamThread* CreateAureoLam();
void ReleaseAureoLam(AureoLamThread* pAL);


#endif