#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <dvb/demux.h>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"

enum TMS_NET_RUN_STATUS
{
	TMS_IDEL = 0,
	TMS_RUN,
	TMS_LOGIN,
	TMS_READ,
	TMS_STOP,
};

class CfcTmsThread: public brunt::CActiveThread
{
public:
	CfcTmsThread();
	virtual ~CfcTmsThread();
	bool Init(char *ip = NULL, short port = 10021);
	bool Start();
	bool Stop();
	int GetStatus() { return m_status; };
	void cleanThread();
	bool IsConnect(){ return bConnected;}; //TMS中心平台是否连接

	bool RegSatelliteDownloadTask();
	bool UpdateSatelliteDownloadStatus(int speed, bool cancel);
	bool ImportContentUsbLocalToTms(std::string path);
	bool finishSatelliteDownload(std::string buf);

protected:
	virtual void doit();
	bool RecvFilter();
	bool ProcessUpdateSatelliteDownloadStatus(char* buf);

private:
	int m_status;
	CZSocket m_socket;
	bool bConnected;
	std::string tmsIp;
	std::string strId;
	short tmsPort;
	std::string strFinish;
};

CfcTmsThread* CreateCfcTms();
void ReleaseCfcTms(CfcTmsThread* pCfc);