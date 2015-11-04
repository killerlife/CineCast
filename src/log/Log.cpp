// Log.cpp : 定义 DLL 的初始化例程。
//
#ifdef WIN32
#include "stdafx.h"
#endif
#include "Log.h"
#include "config.h"

#include "LogFileOperation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

class CLog : public ILog
{
public:
	int Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore);
	int Export(TLogQueryResultArray & contextList);
	int Query(LOGTYPE type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result);
	int Write(LOGTYPE type, const char *text);

private:
	CLogFileOperation m_LogFileOp;


};

V15PC_LOG_API ILog* CreateLog()
{
	return new CLog;
}

V15PC_LOG_API void ReleaseLog(ILog* pLog)
{
	if (pLog != NULL)
		delete pLog;
}

//#define LOGROOT		getConfig().getLogRootPath()	
#define LOGROOT	getConfig().getLogPath()
#define USBROOT	getConfig().getLogOutPath()




int CLog::Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore)
{
	return 0;
}

int CLog::Export(TLogQueryResultArray & contextList)
{
#if 0
	time_t timep;
	time	(&timep);
	tm* p = localtime(&timep);
	char buft[MAX_PATH] = {0};
	string path = USBROOT;
	
	sprintf(buft,"%s\\%04d%02d%02d-%02d:%02d:%02d.log",
		path.c_str(),
		(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday),
		p->tm_hour,p->tm_min,p->tm_sec);


	TLogQueryResultArray::iterator oIter;
	LOGQUERYRESULT itemdata;

	char tmp[10];
	char str[10];
	int nNum = 0;
	int nDay = 0;
	int nRet = 0;
	FILE* pf = NULL;
	char* pStr = str + 6;
	for (oIter = contextList.begin(); oIter != contextList.end(); oIter++)
	{
		itemdata = *oIter;
		memset(tmp,0,10);
		memset(str,0,10);
		memcpy(tmp,itemdata.time.c_str(),6);
		memcpy(str,itemdata.time.c_str(),8);

		if (nNum != atoi(tmp))
		{
			nNum = atoi(tmp);
			sprintf(buft,"%s\\%s",path.c_str(),tmp);
			m_LogFileOp.CreateDirectory(buft);
		}

		if (nDay != atoi(pStr))
		{
			nDay = atoi(pStr);
			if (pf != NULL){
				fclose(pf);
				pf = NULL;
			}
			memset(buft,0,MAX_PATH);
			sprintf(buft,"%s\\%s\\%s.log",path.c_str(),tmp,pStr);
			pf = fopen(buft,"a+");
		}

		if (pf != NULL){
			fprintf(pf,"%d %s %s",(int)itemdata.type,itemdata.time.c_str(),itemdata.text.c_str());
		}
	}

	if (pf != NULL){
		fclose(pf);
		pf = NULL;
	}
#else
	char buft[MAX_PATH] = {0};
	string path = USBROOT;
	string start, end;
	start = contextList.at(0).time;
	end = contextList.at(contextList.size() - 1).time;
	start.resize(8);
	end.resize(8);
	sprintf(buft,"%s\\%s-%s.log",
		path.c_str(),
		start.c_str(),
		end.c_str());
	FILE* pf = NULL;
	pf = fopen(buft,"a+");
	TLogQueryResultArray::iterator oIter;
	LOGQUERYRESULT itemdata;
	for (oIter = contextList.begin(); oIter != contextList.end(); oIter++)
	{
		itemdata = *oIter;
		if (pf != NULL){
			fprintf(pf,"%d %s %s\n",(int)itemdata.type,itemdata.time.c_str(),itemdata.text.c_str());
		}
	}
	if (pf != NULL){
		fclose(pf);
		pf = NULL;
	}
#endif
	return 0;
}


int CLog::Query(LOGTYPE type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result)
{
	char buft[MAX_PATH] = {0};
	string path = LOGROOT;

	/*
	sprintf(buft,"%s\\%04d\\%02d",path.c_str(),timeAfter->year,timeAfter->month);
	if (!m_LogFileOp.IsDirectory(buft)){
		return 0;
	}

	memset(buft,0,MAX_PATH);
	sprintf(buft,"%s\\%04d\\%02d",path.c_str(),timeBefore->year,timeBefore->month);
	if (!m_LogFileOp.IsDirectory(buft)){
		return 0;
	}
*/
	int day = timeAfter->day;
	int month = timeAfter->month;
	int year = timeAfter->year;

	while (!((day - 1) == timeBefore->day 
		&& month == timeBefore->month 
		&& year == timeBefore->year))
	{
		memset(buft,0,MAX_PATH);
		sprintf(buft,"%s\\%04d\\%02d\\%02d.log",path.c_str(),year,month,day++);
		if (m_LogFileOp.OpenFile(buft)){		
			m_LogFileOp.Read(type,result);
			m_LogFileOp.CloseFile();
		}
		
		if (day == 31){
			day = 1;
			month++;
			if (month > 12){
				month = 1;
				year++;
				if (year > timeBefore->year){
					break;
				}
			}
		}
	}

	return 0;
}


int CLog::Write(LOGTYPE type, const char *text)
{
	time_t timep;
	time	(&timep);
	tm* p = localtime(&timep);
	char buft[MAX_PATH] = {0};
	string path = LOGROOT;

	sprintf(buft,"%s/%04d-%02d",path.c_str(),(1900+p->tm_year),(1+p->tm_mon));

	if (!m_LogFileOp.IsDirectory(buft)){
		if (!m_LogFileOp.CreateDirectory(buft))
			return 0;
	}

	memset(buft,0,MAX_PATH);
	sprintf(buft,"%s/%04d-%02d/%02d.log",path.c_str(),(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
	return m_LogFileOp.Write(buft,type,text); 
}





























//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CLogApp
/*
BEGIN_MESSAGE_MAP(CLogApp, CWinApp)
END_MESSAGE_MAP()


// CLogApp 构造

CLogApp::CLogApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLogApp 对象

CLogApp theApp;


// CLogApp 初始化

BOOL CLogApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

*/


























