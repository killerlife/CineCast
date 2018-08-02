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
	std::string m_strTaskTime;		// 任务开始时间 [7/31/2018 jaontolt]
	std::string m_strRoundTime;		// 本轮开始时间 [7/31/2018 jaontolt]
	std::string m_strAureoLamNS;	// 光环HTTP SERVICE地址 [7/31/2018 jaontolt]
	bool bExit;						// 线程退出标志 [7/31/2018 jaontolt]
	uint64 nRecvLength;				// 已接收长度，用于计数接收速率 [7/31/2018 jaontolt]
	int m_logCount;					// 日志计数器，防止保存过多撑爆硬盘 [8/1/2018 jaontolt]
};

AureoLamThread* CreateAureoLam();
void ReleaseAureoLam(AureoLamThread* pAL);


#endif