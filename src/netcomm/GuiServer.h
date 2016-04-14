#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <thread/activeThread/activeThreadManager_i.h>
#include "zSocket.h"
#include "../dvb/mytype.h"

//һ߳,ļר
class CopyThread: public brunt::CActiveThread
{
public:
	CopyThread();
	virtual ~CopyThread();
	bool Init();
	bool Start();
	bool Stop();
	int GetStatus() {return m_status;};
	bool isStop() {return bStop;};
private:
	virtual void doit();
	int copy_dir(char *dir_dst,char *dir_src,int* complete_percent,int* copy_flag);
private:
	int m_status;
	bool bStop;
public:
	char path_src[512];        //浽棬ָ·ѾʧЧ
	char path_dst[512];
	int* complete_percent;     //ɰٷֱ
    int* copy_flag;            //״̬
};

class GuiThread;
class ContentOperation;
class mke2fs;

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

	bool ErrorProtocol(char* buf);
	bool S_GetSatellite(char* buf);
	bool S_GetReceive(char* buf);
	
	bool C_GetTuner(char* buf);
	bool C_SetTuner(char* buf);
	
	bool N_GetConfig(char* buf);
	bool N_SetConfig(char* buf);
	bool N_GetRemote(char* buf);
	bool N_SetRemote(char* buf);

	//bool M_GetContent(char* buf);
	//bool M_GetDiskInfo(char* buf);


    bool M_UpdateProgramList_HDD(char* buf);          //UpdateProgramListˢӲб���Ա? 
    bool M_IsProgramListReady_HDD(char* buf);         //IsProgramListReadyѯǷ׼

    bool M_UpdateProgramList_USB(char* buf);         
    bool M_IsProgramListReady_USB(char* buf); 

    bool M_GetContent_HDD(char* buf);     //rename
	bool M_GetDiskInfo_HDD(char* buf);    //rename
    //new
	bool M_GetContent_USB(char* buf);     //rename
	bool M_GetDiskInfo_USB(char* buf);    //rename
	
	
	bool S_Reboot(char* buf);
	bool S_Shutdown(char* buf);

	bool S_FormatDisk(char* buf);
	bool S_FormatResult(char* buf);
	bool S_FormatStatus(char* buf);


	//new
	bool S_USB_Mount(char* buf);
	bool S_USB_UnMount(char* buf);

	bool M_DeleteDir(char* buf);  //ɾӰƬĿ¼

	bool M_CopyDir_HDD_TO_USB(char* buf);
	bool setCopyProgress(char* buf);    //ؿ

	bool S_Get_TMS(char* buf);

	bool UnknowFunction(char* buf);

	bool R_GetRaidInfo(char* buf);
	bool M_UpdateProgramList_RAID(char* buf);          //UpdateProgramListˢӲб���Ա? 
	bool M_IsProgramListReady_RAID(char* buf);         //IsProgramListReadyѯǷ׼
	bool M_GetContent_RAID(char* buf);     //rename
	bool M_GetDiskInfo_RAID(char* buf);    //rename

	bool R_SetDebugCmd(char* buf);

private:
	uint32* pDebugCmd;
	int m_status;
	bool bStop;
	ContentOperation* m_Content;
	mke2fs *m_mkfs;
	std::string m_sFormatRes;
    CopyThread*  copyThread;    //ļָ߳
};

GuiServer* CreateGuiServer();
void ReleaseGuiServer(GuiServer* pGui);
