#ifndef _BASE_OPERATION_H_
#define _BASE_OPERATION_H_

#include <list>
#include "UiProtocal.h"
#include <dvb/mytype.h>
#include <stdio.h>

#include "../content/IContent.h"
#include "thread/activeThread/activeThreadManager_i.h"

class NetOperation
{
public:
	NetOperation();
	virtual ~NetOperation();
	
	std::list<NETWORK_CONF>& GetNetConfig();
	bool SetNetConfig(std::list<NETWORK_CONF>& m_listNetConf);

	REMOTE_CONF& GetRemoteConfig();
	bool SetRemoteConfig(REMOTE_CONF &m_remoteConf);

private:
	std::list<NETWORK_CONF> m_list;
	REMOTE_CONF m_rc;

private:
	int calcmask(std::string mask);
	int calccount(int n);
	std::string prefix2mask(int n);
};

class SatelliteConfOperation
{
public:
	SatelliteConfOperation();
	virtual ~SatelliteConfOperation();
	bool ReadConfig();
	bool WriteConfig();
};

class ContentOperation
{
public:
	ContentOperation();
	virtual ~ContentOperation();
	
	std::vector<InfoData>& GetContentList(int src);
	int UpdateProgramList(std::vector<int> srcList);
	bool IsProgramListReady(int src);
	uint64 GetAvalibleSpace(int src);
	uint64 GetTotalSpace(int src);

private:
	std::vector<InfoData> m_Content;
	std::vector<InfoData> m_UsbContent;
};

class mke2fs: public brunt::CActiveThread
{
public:
	mke2fs();
	virtual ~mke2fs();
	bool FormatDisk();
	char* GetOutput();
	std::string& GetSOutput();
	uint8 CheckStatus();

private:
	virtual void doit();

private:
	FILE *fp;
	char out[80];
	std::string sout;
	uint8 m_Status;
};

class System
{
public:
	System(){};
	virtual ~System(){};
	void Reboot();
	void Shutdown();
};



//new class
class USB
{
public:
	USB(){};
	virtual ~USB(){};
	void USB_Mount();
	void USB_UnMount();
	void USB_UpdateSpace();
};




//扫描整个目录统计大小
long long scan_dir(char *dir, int depth);     // 定义目录扫描函数  
int cpfile(char *path_dst,char *path_src,long long totalsize,int* complete_percent);
int cpdir(char *target_dir,char *source_dir,long long totalsize,int* complete_percent);
int copy_dir(char *dir_dst,char *dir_src,int* complete_percent,int* flags);

#endif _BASE_OPERATION_H_