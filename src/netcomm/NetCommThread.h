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

//////////////////////////////////////////////////////////////////////////
//Network Communication class:
//////////////////////////////////////////////////////////////////////////
class NetCommThread: public brunt::CActiveThread
{
public:
	NetCommThread();
	virtual ~NetCommThread();
	bool Init(bool bLeonis = false);
	bool Start();
	bool Stop();
	int GetStatus() { return m_status; };
	bool GetMD5File(uint32 filmId); //向节目管理中心平台发送MD5请求
	bool ReportLost(char* buf, int nSize, int nLeoSize); //向节目管理中心平台上报丢包信息
	bool HeartBreat(); //发送心跳包
	void StartRecvTask(); //初始化任务开始时间
	void StartRoundRecv(); //初始化每轮开始时间
	bool LogUpload(uint32 nBegin, uint32 nEnd, bool bLeonis = false); //日志上报
	bool PkgSendConfirm(uint32 filmId); //LEONIS分布式补包丢包上报确认
	bool IsConnect(){ return bConnected;}; //节目管理中心平台是否连接
	bool DecryptRep(); //MD5校验结果上报
	void CloseConnect(); //关闭节目管理中心连接
	void StartConnect(); //连接节目管理中心

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
	bool bPause;
};

NetCommThread* CreateNetComm();
void ReleaseNetComm(NetCommThread* pNet);