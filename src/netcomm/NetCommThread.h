#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <dvb/demux.h>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"
#include "message.h"

enum NET_RUN_STATUS
{
	NET_IDEL = 0,
	NET_RUN,
	NET_LOGIN,
	NET_AUTH,
	NET_READ,
	NET_STOP,
};

class HeartThread;

class NetCommThread: public brunt::CActiveThread
{
public:
	NetCommThread();
	virtual ~NetCommThread();
	bool Init(bool bLeonis = false);
	bool Start();
	bool Stop();
	int GetStatus() { return m_status; };
	bool GetMD5File(uint32 filmId);
	bool ReportLost(char* buf, int nSize, int nLeoSize);
	bool HeartBreat();
	void StartRecvTask();
	void StartRoundRecv();
	bool LogUpload(uint32 nBegin, uint32 nEnd, bool bLeonis = false);
	bool PkgSendConfirm(uint32 filmId);
	bool IsConnect(){ return bConnected;};

private:
	virtual void doit();
	bool Login();
	bool Auth();
	inline void GetMutex();
	inline void ReleaseMutex();
	void GetHardKey(const std::string hardkey);
	void cleanThread();
	bool RecvFilter();
	bool DecryptRep(uint32 filmId);
	bool UpdateRep(uint8* sn);

private:
	int m_status;
	CZSocket m_loginSocket;
	CZSocket m_authSocket;
	struct L_LOGIN_REQ m_loginReq;
	struct R_LOGIN_REP m_loginRep; 
	uint32 m_nMachineId;
	uint8 m_hardKey[16];
	struct R_AUTH_REP m_authRep;
	int m_mutex;
	struct R_MD5_KEY_REP m_md5Rep;
	bool bMd5Rep;
	bool bConnected;
	std::string m_strTaskTime;
	std::string m_strRoundTime;
	bool bLeonisCfg;
	std::string m_strPkgFileName;
	bool bPkgSendStart;
	uint32 nFilmId;
	std::string m_strUpdateFile;
	uint64 nRecvLength;
	HeartThread* pHeartThread;
};