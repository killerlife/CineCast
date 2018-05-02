#ifndef _BASE_OPERATION_H_
#define _BASE_OPERATION_H_

#include <list>
#include "UiProtocal.h"
#include <dvb/mytype.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>

#include "../content/IContent.h"
#include "thread/activeThread/activeThreadManager_i.h"

#pragma pack(1)
class NetOperation
{
public:
	NetOperation();
	virtual ~NetOperation();
	
	std::list<NETWORK_CONF>& GetNetConfig();
	bool SetNetConfig(std::list<NETWORK_CONF>& m_listNetConf);

	REMOTE_CONF& GetRemoteConfig();
	bool SetRemoteConfig(REMOTE_CONF &m_remoteConf);

	std::list<NETWORK_STATUS>& GetNetStatus();

private:
	std::list<NETWORK_CONF> m_list;
	std::list<NETWORK_STATUS> m_slist;

	REMOTE_CONF m_rc;

private:
	int calcmask(std::string mask);
	int calccount(int n);
	std::string prefix2mask(int n);
	NETWORK_CONF GetNetConfig(std::string dev);
	REMOTE_CONF GetDns(std::string dev);
	NETWORK_STATUS GetNetStatus(std::string dev);
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
	bool AutoDelete(int src, std::vector<std::string>&runList);

private:
	void UpdateDirList(int src);
	void FindDir(std::string dir);

private:
	std::vector<InfoData> m_Content;
	std::vector<InfoData> m_UsbContent;
	std::vector<InfoData> m_RaidContent;
	std::vector<std::string>m_dir;
	std::vector<int> m_srcList;
};

typedef enum{
	DISK_REMOVEABLE = 0,
	DISK_RAID,
}DISK_TYPE;

class mke2fs: public brunt::CActiveThread
{
public:
	mke2fs();
	virtual ~mke2fs();
	bool FormatDisk(DISK_TYPE type);
	char* GetOutput();
	std::string& GetSOutput();
	uint8 CheckStatus();
	bool MountDisk(DISK_TYPE type);

private:
	virtual void doit();

private:
	FILE *fp;
	char out[1024];
	std::string sout;
	uint8 m_Status;
	DISK_TYPE m_type;
};

class System
{
public:
	System(){};
	virtual ~System(){};
	void Reboot();
	void Shutdown();

	//DateTime format
	//"YYYY-MM-DD hh:mm:ss"
	void SetDateTime(char* stime);
};



//new class
class USB
{
public:
	USB(){};
	virtual ~USB(){};
	bool USB_Mount();
	bool USB_UnMount();
	void USB_UpdateSpace();

private:
	void find_dir(std::string dir);

private:
	std::string strMounted;
	std::vector<std::string>m_dir;
};




//扫描整个目录统计大小
long long scan_dir(char *dir, int depth);     // 定义目录扫描函数  
int cpfile(char *path_dst,char *path_src,long long totalsize,int* complete_percent);
int cpdir(char *target_dir,char *source_dir,long long totalsize,int* complete_percent);
int copy_dir(char *dir_dst,char *dir_src,int* complete_percent,int* flags);

typedef struct _ENCODEBLOCK_
{
	uint64 EncodeBegin;
	uint64 EncodeEnd;
}ENCODEBLOCK;

typedef struct _MD5BLOCK_
{
	uint64 Md5Begin;
	uint64 Md5End;
	char md5[16];
	_MD5BLOCK_()
	{
		memset(md5, 0, 16);
	}
}MD5BLOCK;

typedef struct _ENCODEFILE_
{
	std::string Name;
	uint16 Id;
	bool Encode;
	bool Md5;
	std::string SubPath;
	std::vector<ENCODEBLOCK> EncodeBlockList;
	std::vector<MD5BLOCK> Md5BlockList;
public:
	void ENCODEFILE()
	{
		Encode = false;
		Md5 = false;
	}
	~_ENCODEFILE_()
	{
		EncodeBlockList.clear();
		Md5BlockList.clear();
	};
	void clear()
	{
		EncodeBlockList.clear();
		Md5BlockList.clear();
	};
}ENCODEFILE;

typedef struct _ENCODEFILELIST_
{
	std::string Name;
	uint32 Id;
	std::string ChallengeCode;
	std::vector<ENCODEFILE> EncodeFileList;
public:
	void clear()
	{
		for(int i = 0; i < EncodeFileList.size(); i++)
		{
			EncodeFileList[i].clear();
		}
		EncodeFileList.clear();
	}
} ENCODEFILELIST;

class Md5Class
{
public:
	Md5Class();
	virtual ~Md5Class(){};
	bool Md5Parser(char *buf);
	bool GetMd5(FILE* fp, MD5BLOCK mb);
	bool Md5Verify(uint32 filmId);

	bool bMd5Success;
	uint64 nRollBackLen;

private:
	void SaveResult(uint32 filmId);
	ENCODEFILELIST efl;
};

Md5Class* CreateMd5Class();
void ReleaseMd5Class(Md5Class* pMd5);

class RaidDetailParser
{
public:
	void RunRaidManager();
	std::string GetRaidLevel(){ return strRaidLevel; };
	std::string GetState(){ return strState; };
	int GetRaidDevices(){ return nRaidDevices; };
	int GetActiveDevices(){ return nActiveDevices; };
	int GetWorkingDevices(){ return nWorkingDevices; };
	int GetFailedDevices(){ return nFailedDevice; };
	uint64 GetArraySize(){ return nArraySize; };
	uint64 GetUsedSize(){ return nUsedSize; };
	std::vector<std::string>& GetDevicesState(){ return strDevState; };

private:
	void DetailParser(std::string strDetail);

private:
	std::string strRaidLevel;
	std::string strState;
	int nRaidDevices;
	int nActiveDevices;
	int nWorkingDevices;
	int nFailedDevice;
	uint64 nArraySize;
	uint64 nUsedSize;
	std::vector<std::string> strDevState;
};

#endif _BASE_OPERATION_H_