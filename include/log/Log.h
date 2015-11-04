// Log.h : Log DLL 的主头文件
//

#ifndef V15PC_LOG_
#define V15PC_LOG_


#pragma once

#ifndef __AFXWIN_H__
	//#error include 'stdafx.h' before including this file for PCH
#endif

#ifdef WIN32
#include "resource.h"		// 主符号
#endif
#include "LogStruct.h"

#ifdef _WIN32
#ifdef V15PC_LOG
#define V15PC_LOG_API __declspec(dllexport)
#else
#define V15PC_LOG_API __declspec(dllimport)
#endif

#else 
#define V15PC_LOG_API
#endif


class V15PC_LOG_API ILog
{
public:
	virtual int Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore) = 0;
	virtual int Export(TLogQueryResultArray & contextList) = 0;
	virtual int Query(LOGTYPE type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result) = 0;
	virtual int Write(LOGTYPE type, const char *text) = 0;
};


	
	
V15PC_LOG_API ILog* CreateLog();
V15PC_LOG_API void ReleaseLog(ILog* pLog);

#endif


/*
class CLogApp : public CWinApp
{
public:
	CLogApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
*/