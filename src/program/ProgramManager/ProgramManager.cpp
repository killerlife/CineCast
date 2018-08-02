// ProgramManager.cpp : 定义 DLL 应用程序的入口点。
//
#include "ProgramManager.h"
#include "activeThread.h"
#ifdef DEBUGPRINT
#include "brunt/debugprint.h"
#else
#define DPRINT(arg...)
#endif
#include <stdio.h>
#include <stdlib.h>
#include "config/config.h"
#ifdef LICENSE
#include "license/ILicense.h"
#endif
#include <sstream>
#include <string>
#include <vector>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
//#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

using namespace std;
using namespace brunt;


#ifdef SIZEOF_ARY
#undef SIZEOF_ARY
#endif//SIZEOF_ARY

#define SIZEOF_ARY(a)	(sizeof(a)/sizeof(a[0]))

class CProgramManager: public IProgramManager, public CActiveThread
{
public:
	IProgramQuery* getIProgramQuery(int nSource);
	IProgramImport* getIProgramImport();
	// update programs' info of some source 
	virtual int update(const vector<int>& updateSrcList);
	// the program data whether is ready
	virtual bool isReady(int nSource);
	// get error message
	virtual int getLastError(string* msg);

	virtual int getProgramFileList(int nSrc, int type, std::vector<InfoData>& ProgramList);

	CProgramManager();
	virtual ~CProgramManager();

protected:
	struct TQueryInfo{
		string root;
		IProgramQuery* query;
		TQueryInfo():query(NULL){};
	};

	virtual void init();
	virtual void doit();
	virtual void release();

	void loadInfo(int nSrc);
	std::string getProgramPath(const std::string& programId);
	inline const char* getProgramSrc(int src);
	void cleanThread();

	TQueryInfo m_queryInfoList[PST_SIZEOF];
// 	std::vector<struct InfoData> m_programInfoList[PST_SIZEOF];	// 0: hdisk , 1: USB
	boost::mutex m_mutexQueryList;
	bool m_infoValidMark[PST_SIZEOF];	// map of m_programInfoList for data validation
	std::vector<int> m_updateSrcList;		// loadInfo() parameters
//	bool m_updateOk;
   	int m_error; 
};


IProgramManager* getIProgramManager()
{
	static CProgramManager _manager;
	return &_manager;
}

#ifdef _WIN32
#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif//_WIN32



CProgramManager::CProgramManager()
{
//	m_updateOk = true;
	m_error = 0;
	memset(m_infoValidMark, false, sizeof(m_infoValidMark));
}

CProgramManager::~CProgramManager()
{
	cleanThread();
}

IProgramQuery* CProgramManager::getIProgramQuery(int nSource)
{
	if(nSource>=0 && nSource<PST_SIZEOF)
		return m_queryInfoList[nSource].query;
	return NULL;
}

IProgramImport* CProgramManager::getIProgramImport()
{
	return NULL;
}

// update programs' info of some source 
int CProgramManager::update(const vector<int>& updateSrcList)
{
	if(!(status() == thread_stopped || status() == thread_ready ))
	{
		DPRINT("The thread had not been ready, status = %d.", status());
//		DPRINT((DP_Error, "CProgramManager", "The thread had not been ready, status = %d.", status()));
		return -1;
	}

	if(isStopped())
	{
		cleanThread();
	}

	m_updateSrcList = updateSrcList;
	if(!start())
	{
		DPRINT("Start thread failed, status = %d.", status());
//		DPRINT((DP_Error, "CProgramManager", "Start thread failed, status = %d.", status()));
		return -1;
	}

	for(int i=0; i<m_updateSrcList.size(); i++)
	{
		if(m_updateSrcList[i]>=0 && m_updateSrcList[i]<PST_SIZEOF)
			m_infoValidMark[m_updateSrcList[i]] = false;
	}

	return 0;
}

// the program data whether is ready
bool CProgramManager::isReady(int nSource)
{
	if(nSource>=0 && nSource<PST_SIZEOF)
		return m_infoValidMark[nSource];
	return false;
}

int CProgramManager::getLastError(string* msg)
{
	if(msg)
	{
//	switch(m_error)
//	{
//	case :
//		*msg = "";
//		break;
//	}
	}
	return m_error;
}

std::string FormatTime(long mstime, bool bSec = false);

std::string FormatTime(long mstime, bool bSec)
{
	if (bSec)
	{
		long ms = 0;
		long second = mstime%60;
		mstime /= 60;
		long mintue = mstime%60;
		mstime /= 60;
		long hour = mstime;
		std::string csText;
		char sTemp[20];
		sprintf(sTemp, "%02d:%02d:%02d.%03d", hour, mintue, second, ms);
		csText = sTemp;
		return csText;
	}
	else
	{
		long ms = mstime%1000;
		mstime /= 1000;
		long second = mstime%60;
		mstime /= 60;
		long mintue = mstime%60;
		mstime /= 60;
		long hour = mstime;
		std::string csText;
		char sTemp[20];
		sprintf(sTemp, "%02d:%02d:%02d.%03d", hour, mintue, second, ms);
		csText = sTemp;
		return csText;
	}
}

void CProgramManager::init()
{
//	m_updateOk = false;
	return CActiveThread::init();
}

void CProgramManager::doit()
{
	for(int i=0; i<m_updateSrcList.size() && status()!=thread_stoping; i++)
	{
		assert(m_updateSrcList[i]>=0 && m_updateSrcList[i]<PST_SIZEOF);
		if(m_updateSrcList[i]>=0 && m_updateSrcList[i]<PST_SIZEOF)
		{
			loadInfo(m_updateSrcList[i]);
		}
	}
}

void CProgramManager::release()
{
//	m_updateOk = true;
	return CActiveThread::release();
}

void CProgramManager::cleanThread()
{
	CActiveThread::stop();
}

//
void CProgramManager::loadInfo(int nSrc)
{
	if(nSrc<0 || nSrc>=PST_SIZEOF)
		return;

	IProgramQuery* p = createProgramQuery();
	if(!p) return;

	string root;
	if(nSrc==PST_HDD)
		root = "/storage";//getConfig().getProgramRootPath();
	else
		root = "/media/usb";//getConfig().getUsbRootPath();

	if(p->open(root, true)!=0)
	{
		DPRINT("IProgramQuery->open(%s) failed.", root.c_str());
//		DPRINT(DP_Error, "CProgramManager", "IProgramQuery->open(%s) failed.", root.c_str());
//		releaseProgramQuery(p);
//		return;
	}

	boost::mutex::scoped_lock lock(m_mutexQueryList);
	if(m_queryInfoList[nSrc].query)
		releaseProgramQuery(m_queryInfoList[nSrc].query);
	m_queryInfoList[nSrc].query = p;
	m_queryInfoList[nSrc].root = root;
	m_infoValidMark[nSrc] = true;
}

int CProgramManager::getProgramFileList(int nSrc, int type, std::vector<InfoData>& ProgramList)
{
	string sType ;
	if(type==0){
		sType = "Movie" ;
	}else if(type==1){
		sType = "BusinessAd" ;
	}else{
		sType = "PublicAd" ;
	}

	if(nSrc<0 || nSrc>=PST_SIZEOF)
		return -1;

	boost::mutex::scoped_lock lock(m_mutexQueryList);
	
	//Maybe doesn't need to clear
	ProgramList.clear();

	int count = 0;

	IProgramQuery* p = m_queryInfoList[nSrc].query;
	if (p == NULL)
	{
		DPRINT("Program query == NULL\n");
		return -1;
	}
	

	DPRINT("program count %d\n", p->getProgramCount());
	for (int i = 0; i < p->getProgramCount(); i++)
	{
		TProgramInfo info;
		if (p->getProgramInfo(i, info) < 0)
		{
			DPRINT("get program info failed\n");
			continue;
		}
// 		if ((info.type.compare(sType)))
// 			continue;

		InfoData viewInfo;
		viewInfo.pData[0] = info.id;
		viewInfo.pData[1] = info.title;
#ifndef INDIA
		viewInfo.pData[2] = "Movie";
#else
		viewInfo.pData[2] = info.contentType;
#endif
		viewInfo.pData[3] = "N/A";
		viewInfo.pData[4] = info.type;
		viewInfo.pData[5] = "DCP";
		long a, b;
		sscanf(info.duration.c_str(), "%ld.%03d", &a, &b);
		viewInfo.pData[6] = FormatTime(a*1000+b, false);
		viewInfo.pData[7] = "1080i60";
		viewInfo.pData[8] = info.issuer;
		viewInfo.pData[9] = info.issueDate;
		viewInfo.pData[10] = "";
		viewInfo.pData[11] = info.country;
		viewInfo.pData[12] = "";
		viewInfo.pData[13] = info.language;
		viewInfo.pData[14] = info.rootpath;//programList[i];
		viewInfo.pData[15] = "NOENC";
		for (int c = 0; c < info.videoInfo.size(); c++)
		{
			if(info.videoInfo.at(c).keyIdStr != "")
			{
				viewInfo.pData[15] = "ENC";
				break;
			}
		}
		
		// license
		viewInfo.pData[10] = "N/A";
		#ifdef LICENSE
		ILicenseManager* licmgr = getLicenseManager();
		ILicenseCtrl* licensectrl = licmgr->queryILicenseCtrl(info.id);
		if(licensectrl)
		{
			viewInfo.pData[3] = licensectrl->checkType()?(licensectrl->getType()==LST_KDM?"KDM":"DMS"):"U";
			std::vector<TLicenseInfo> infos;
			if(licensectrl->getInfo(infos) == 0)
			{
				TLicenseInfo info_lic = infos.at(0);
				viewInfo.pData[10] = info_lic.startdate.GetString() + " - " + info_lic.enddate.GetString();
			}
			licensectrl->Release();
		}
		#endif

		ProgramList.push_back(viewInfo);
		count++;
	}
	return 0;

#if 0
	if(m_programInfoList[nSrc].size()<=0){
		return m_programInfoList[nSrc] ;
	}

	m_programInfoList[nSrc].clear();

	std::vector<std::string> programList;
	for(int i = 0; i < p->getProgramCount(); ++i)
	{
		TProgramInfo info;
		if(p->getProgramInfo(i, info)<0)
			continue;

		InfoData viewInfo;
		viewInfo.pData[0] = info.id;
		viewInfo.pData[1] = info.title;
		viewInfo.pData[2] = "Movie";
		viewInfo.pData[3] = "";
		viewInfo.pData[4] = "MPEG2";
		viewInfo.pData[5] = info.type;
		viewInfo.pData[6] = FormatTime(atoi(info.duration.c_str()));
		viewInfo.pData[7] = "1080i60";
		viewInfo.pData[8] = info.issuer;
		viewInfo.pData[9] = info.issueDate;
		viewInfo.pData[10] = "";
		viewInfo.pData[11] = info.country;
		viewInfo.pData[12] = "";
		viewInfo.pData[13] = info.language;
		viewInfo.pData[14] = info.rootpath;//programList[i];

		// license
		#ifdef LICENSE
		ILicenseManager* licmgr = getLicenseManager();
		ILicenseCtrl* licensectrl = licmgr->queryILicenseCtrl(info.id);
		if(licensectrl)
		{
			viewInfo.pData[3] = licensectrl->checkType()?(licensectrl->getType()==LST_KDM?"KDM":"DMS"):"U";
			licensectrl->Release();
		}
		#endif

		m_programInfoList[nSrc].push_back(viewInfo);
	}
	std::vector<InfoData>::iterator ite = m_programInfoList[nSrc].begin();
	for(unsigned int i = 0; ite != m_programInfoList[nSrc].end(); ++i, ++ite)
	{
		InfoData ptmp = *ite ;
		if((ptmp.pData[2].compare(sType))){
			m_programInfoList[nSrc].erase(ite);
			if(ite == m_programInfoList[nSrc].end()){
				break ;
			}
		}
	}
	return m_programInfoList[nSrc];
#endif
	return 0;
}

