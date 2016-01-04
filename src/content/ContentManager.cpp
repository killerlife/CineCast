#include "IContent.h"
#include "IContentQuery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <dvb/mytype.h>

#include <thread/activeThread/activeThread.h>
#include <thread/activeThread/activeThreadManager_i.h>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
//#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include <log/Log.h>

using namespace std;
using namespace brunt;

class ContentManager: public IContentManager, public CActiveThread
{
public:
	virtual int update(const vector<int>& updateSrcList);
	virtual bool isReady(int nSource);
	virtual int getLastError(string*msg);
	virtual int getProgramFileList(int nSrc, int type, std::vector<InfoData>& programList);

	ContentManager();
	virtual ~ContentManager();

protected:
	struct TQueryInfo{
		string root;
		IContentQuery* query;
		TQueryInfo():query(NULL){};
	};

	virtual void init();
	virtual void doit();
	virtual void release();
	
	virtual void loadInfo(int nSrc);
	void cleanThread();
	std::string FormatTime(long mstime, bool bSec = false);

	TQueryInfo m_queryInfoList[PST_SIZEOF];
	boost::mutex m_mutexQueryList;
	bool m_infoValidMark[PST_SIZEOF];
	std::vector<int> m_updateSrcList;
	int m_error;
	ILog* pLog;
};


IContentManager* getIContentManager()
{
	static ContentManager _cManager;
	return &_cManager;
}

ContentManager::ContentManager()
{
	pLog = CreateLog();
	m_error = 0;
	memset(m_infoValidMark, false, sizeof(m_infoValidMark));
}

ContentManager::~ContentManager()
{
	cleanThread();
	if (pLog)
	{
		ReleaseLog(pLog);
	}
}

int ContentManager::update(const vector<int>& updateSrcList)
{
	char str[512];
	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[ContentManager] The thread had not been ready, status = %d.", status());
		DPRINTF("%s\n", str);
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return -1;
	}

	if (isStopped())
	{
		cleanThread();
	}

	m_updateSrcList = updateSrcList;
	if(!start())
	{
		sprintf(str, "[ContentManager] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return -1; 
	}

	for(int i=0; i<m_updateSrcList.size(); i++)
	{
		if(m_updateSrcList[i]>=0 && m_updateSrcList[i]<PST_SIZEOF)
			m_infoValidMark[m_updateSrcList[i]] = false;
	}

	return 0; 
}

bool ContentManager::isReady(int nSource)
{
	if(nSource>=0 && nSource<PST_SIZEOF)
		return m_infoValidMark[nSource];
	return false;
}

int ContentManager::getLastError(string* msg)
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

void ContentManager::init()
{
	return CActiveThread::init();
}

void ContentManager::doit()
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

void ContentManager::release()
{
	//	m_updateOk = true;
	return CActiveThread::release();
}

void ContentManager::cleanThread()
{
	CActiveThread::stop();
}

int ContentManager::getProgramFileList(int nSrc, int type, std::vector<InfoData>& ProgramList)
{
	char str[512];

	if(nSrc<0 || nSrc>=PST_SIZEOF)
	{
		sprintf(str, "[ContentManager] Src not in list.");
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}

		return -1;
	}

	boost::mutex::scoped_lock lock(m_mutexQueryList);

	//Maybe doesn't need to clear
	ProgramList.clear();

	int count = 0;

	IContentQuery* p = m_queryInfoList[nSrc].query;
	if (p == NULL)
	{
		sprintf(str, "[ContentManager] Can not get IContentQuery in %d.", nSrc);
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return -1;
	}


	for (int i = 0; i < p->getProgramCount(); i++)
	{
		TContentInfo info;
		if (p->getProgramInfo(i, info) < 0)
			continue;
		// 		if ((info.type.compare(sType)))
		// 			continue;

		InfoData viewInfo;
		viewInfo.pData[0] = info.id;
		viewInfo.pData[1] = info.fileName;
		viewInfo.pData[2] = info.progress;
		viewInfo.pData[3] = info.status;
		viewInfo.pData[4] = info.type;
		viewInfo.pData[5] = info.fileLength;
		long a, b;
		sscanf(info.duration.c_str(), "%ld.%03d", &a, &b);
		viewInfo.pData[6] = FormatTime(a*1000+b, false);
		viewInfo.pData[7] = info.stereoScopic;
		viewInfo.pData[8] = info.issuer;
		viewInfo.pData[9] = info.issueDate;
		viewInfo.pData[10] = info.country;
		viewInfo.pData[11] = info.receiveSegment;
		viewInfo.pData[12] = info.totalSegment;
		viewInfo.pData[13] = info.dateTime;
		viewInfo.pData[14] = info.rootpath;//programList[i];
		viewInfo.pData[15] = info.creator;

		ProgramList.push_back(viewInfo);
		count++;
	}
	return 0;
}

std::string ContentManager::FormatTime(long mstime, bool bSec)
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

void ContentManager::loadInfo(int nSrc)
{
	char str[512];
	if(nSrc<0 || nSrc>=PST_SIZEOF)
	{
		sprintf(str, "[ContentManager] Src not in list.");
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return;
	}

	IContentQuery* p = createContentQuery();
	if(!p) return;

	string root;
	if(nSrc==PST_HDD)
		root = "/storage";//getConfig().getProgramRootPath();
	else if(nSrc == PST_USB)
		root = "/media/usb";//getConfig().getUsbRootPath();
	else
		root = "/raid";

	if(p->open(root, true)!=0)
	{
		sprintf(str, "[ContentManager] IContentQuery->open(%s) failed.", root.c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		DPRINTF("%s\n", str);
		//		DPRINT(DP_Error, "CProgramManager", "IProgramQuery->open(%s) failed.", root.c_str());
		//		releaseProgramQuery(p);
		//		return;
	}

	boost::mutex::scoped_lock lock(m_mutexQueryList);
	if(m_queryInfoList[nSrc].query)
		releaseContentQuery(m_queryInfoList[nSrc].query);
	m_queryInfoList[nSrc].query = p;
	m_queryInfoList[nSrc].root = root;
	m_infoValidMark[nSrc] = true;
}
