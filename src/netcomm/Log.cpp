// Log.cpp :  DLL ĳʼ̡
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
#include "ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

class CLog : public ILog
{
public:
	CLog();
	int Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore);
	int Export(TLogQueryResultArray & contextList);
	int Query(LOGTYPE type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result);
	int Write(LOGTYPE type, const char *text);

private:
	CLogFileOperation m_LogFileOp;
	std::string logroot, usbroot;
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
//#define LOGROOT	getConfig().getLogPath()
//#define USBROOT	getConfig().getLogOutPath()
#define LOGROOT "../log"
#define USBROOT "/media/usb"

using namespace brunt;

CLog::CLog()
{
	ICMyini* ini = createMyini();
	logroot = LOGROOT;
	usbroot = USBROOT;
	if (ini)
	{
		if(ini->load("/etc/CineCast/config.cfg"))
		{
			std::string tmp;
			if(ini->read(" ", "logroot", tmp))
				logroot = tmp;
			if(ini->read(" ", "usbroot", tmp))
				usbroot = tmp;
		}
	}
}

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
	string path = usbroot;
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
	string path = logroot;

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
		sprintf(buft,"%s/%04d-%02d/%02d.log",path.c_str(),year, month, day++);
		if (m_LogFileOp.OpenFile(buft)){
			m_LogFileOp.Read(type, result);
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

//  printf("run to CLog::Query()\n");
//  printf("run to CLog::Query()-end\n");
// 	printf("result.size()=%d\n",result.size());
	
// 	for(int i = 0; i < result.size(); i++)
// 	{
// 		printf("result:%s %s", result[i].time.c_str(), result[i].text.c_str());
// 	}
	
	return 0;
	//return result;
}


int CLog::Write(LOGTYPE type, const char *text)
{
	time_t timep;
	time	(&timep);
	tm* p = localtime(&timep);
	char buft[MAX_PATH] = {0};
	string path = logroot;

	sprintf(buft,"%s/%04d-%02d",path.c_str(),(1900+p->tm_year),(1+p->tm_mon));
	//printf("%s\n", buft);
	if (!m_LogFileOp.IsDirectory(buft)){
		if (!m_LogFileOp.CreateDirectory(buft))
		{
			return 0;
		}
	}
	//printf("%s\n", buft);

	memset(buft,0,MAX_PATH);
	sprintf(buft,"%s/%04d-%02d/%02d.log",path.c_str(),(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
	return m_LogFileOp.Write(buft,type,text); 
}




//
//	ע⣡
//
//		 DLL ̬ӵ MFC
//		DLLӴ DLL 
//		 MFC κκںǰ
//		 AFX_MANAGE_STATE ꡣ
//
//		:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// ˴Ϊͨ
//		}
//
//		˺κ MFC 
//		ÿʮҪζ
//		Ϊеĵһ
//		֣ж
//		ΪǵĹ캯 MFC
//		DLL á
//
//		йϸϢ
//		 MFC ˵ 33  58
//

// CLogApp
/*
BEGIN_MESSAGE_MAP(CLogApp, CWinApp)
END_MESSAGE_MAP()


// CLogApp 

CLogApp::CLogApp()
{
	// TODO: ڴ˴ӹ룬
	// Ҫĳʼ InitInstance 
}


// Ψһһ CLogApp 

CLogApp theApp;


// CLogApp ʼ

BOOL CLogApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

*/


























