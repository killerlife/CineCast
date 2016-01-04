#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <dvb/demux.h>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"
#include "message.h"
#include <config/config.h>

enum NET_RUN_STATUS
{
	NET_IDEL = 0,
	NET_RUN,
	NET_LOGIN,
	NET_AUTH,
	NET_READ,
	NET_STOP,
};

class NetCommThread: public brunt::CActiveThread
{
public:
	NetCommThread();
	virtual ~NetCommThread();
	bool Init();
	bool Start();
	bool Stop();
	int GetStatus() { return m_status; };
	bool GetMD5File(uint32 filmId);
	bool ReportLost(L_LOST_INFO &lost);

private:
	virtual void doit();
	bool Login();
	bool Auth();
	inline void GetMutex();
	inline void ReleaseMutex();

private:
	int m_status;
	CZSocket m_loginSocket;
	CZSocket m_authSocket;
	IConfig* m_pConfig;
	struct L_LOGIN_REQ m_loginReq;
	struct R_LOGIN_REP m_loginRep; 
	uint8 m_hardKey[16];
	struct R_AUTH_REP m_authRep;
	int m_mutex;
	struct R_MD5_KEY_REP m_md5Rep;
	bool bMd5Rep;
};