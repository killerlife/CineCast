#ifdef WIN32
#include "StdAfx.h"
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "LogFileOperation.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif


CLogFileOperation::CLogFileOperation(void)
{
}

CLogFileOperation::~CLogFileOperation(void)
{
}


int CLogFileOperation::IsDirectory(const char* path)
{
#ifdef WIN32
	if(GetFileAttributes(path) == -1) 
		return 0;
	else
		return 1;
#else
	struct stat buf;
	if(stat(path, &buf) != 0)
		return 0;
	if (buf.st_mode & S_IFDIR)
	{
		return 1;
	}
	return 0;
#endif
}


int CLogFileOperation::CreateDirectory(const char* path)
{
#ifdef WIN32
	CString strDir(path);
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;
	CString strTemp;
	bool bSuccess = false;

	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{
			vPath.push_back(strTemp);
			strTemp.AppendChar(_T('\\'));
		}
	}

	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		bSuccess = ::CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
#else
	std::string strDir(path);
	if (strDir.at(strDir.length()-1)!='/')
	{
		strDir += '/';
	}

	std::vector<std::string> vPath;
	std::string strTemp;
	bool bSuccess = false;

	for (int i=0;i<strDir.length();++i)
	{
		if (strDir.at(i) != '/') 
		{
			strTemp += strDir.at(i);
		}
		else 
		{
			vPath.push_back(strTemp);
			strTemp += '/';
		}
	}

	std::vector<std::string>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		bSuccess = mkdir((char*)(*vIter).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) ? true : false;    
	}

	return bSuccess;
#endif
}

int CLogFileOperation::DeleteDirectory(const char* path)
{
#ifdef WIN32
	CFileFind tempFind;
	char tempFileFind[200];
	sprintf(tempFileFind,"%s\\*.*",path);
	BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
	while(IsFinded)
	{
		IsFinded=(BOOL)tempFind.FindNextFile();
		if(!tempFind.IsDots())
		{
			char foundFileName[200];
			strcpy(foundFileName,tempFind.GetFileName().GetBuffer(200));
			if(tempFind.IsDirectory())
			{
				char tempDir[200];
				sprintf(tempDir,"%s\\%s",path,foundFileName);
				DeleteDirectory(tempDir);
			}
			else
			{
				char tempFileName[200];
				sprintf(tempFileName,"%s\\%s",path,foundFileName);
				DeleteFile(tempFileName);
			}
		}
	}
	tempFind.Close();
	if(!RemoveDirectory(path))
	{
		::MessageBox(0,"ɾĿ¼ʧܣ","Ϣ",MB_OK);
		return FALSE;
	}
	return TRUE;
#else
	DIR *d;
	struct dirent *file;
	struct stat buf;
	if(!(d = opendir(path)))
		return FALSE;
	while((file = readdir(d)) != NULL)
	{
		if (strncmp(file->d_name, ".", 1) == 0)
			continue;
		stat(file->d_name, &buf);
		if (!(buf.st_mode & S_IFDIR))
		{
			remove(file->d_name);
		}
	}
	if (rmdir(path) == 0)
	{
		return TRUE;
	}
	return FALSE;
#endif
}

int CLogFileOperation::OpenFile(const char* file)
{
	m_pfile = fopen(file,"r");
	if (m_pfile == NULL)
		return 0;
	return 1;
}

int CLogFileOperation::CloseFile()
{
	if (m_pfile != NULL){
		fclose(m_pfile);
	}
	return 0;
}

static int mLogMutex = 0;
int CLogFileOperation::Write(const char* file,const int type, const char* context)
{
	time_t timep;
	tm* p;
	time	(&timep);
	p = localtime(&timep);
	char buft[MAX_PATH] = {0};

	while(mLogMutex) usleep(1);
	mLogMutex = 1;
	
	FILE* pf = fopen(file,"a+");
	if (NULL == pf){
		mLogMutex = 0;
		return 0;
	}

	fprintf(pf,"%d %04d%02d%02d-%02d:%02d:%02d %s\n",
		type,
		(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday),
		p->tm_hour,p->tm_min,p->tm_sec,
		context);


	if (NULL != pf){
		fclose(pf);
	}
	mLogMutex = 0;
	return 1;
}

std::string str_LOG;//NEW
std::string g_LogAll;

int CLogFileOperation::Read(const int type, TLogQueryResultArray & result)
{
	char* pfile = NULL;
	char* pMove = NULL;
	char buf[MAX_PATH] = {0};
	char str[MAX_PATH] = {0};

	//LOGQUERYRESULT res; //don't know why, it cause code dump
	
	//用任意字符填充 只要有push函数就会出错
	/*
	res.time="time time time time time";
	res.text="text text text text text";
	result.push_back(res);  //new
	*/
	
	//NEW
	str_LOG="";  //全局字符串初始化
	g_LogAll="";
	char sType[6];
	
	while ((pfile = fgets(buf,MAX_PATH,m_pfile)))
	{
#if 0
		res.type = (LOGTYPE)atoi(buf);
		pfile = strstr(buf," ");
		pMove = strstr(++pfile," ");
		memcpy(str,pfile,pMove - pfile);
		res.time = str;
		res.text = ++pMove;

		if ((LOGTYPE)type == LOG_ALL || (LOGTYPE)type == res.type)
		{	
			//res.push_back(res);   //OLD
			//NEW
			printf("Read:%s %s", res.time.c_str(), res.text.c_str());	
			str_LOG+=res.time.c_str();
			str_LOG+="  ";   //时间与日志内容加空格间隙
			str_LOG+=res.text.c_str();
			sprintf(sType, "%d ", res.type);
			g_LogAll += sType;
			g_LogAll += res.time.c_str();
			g_LogAll += " ";
			g_LogAll += res.text.c_str();
	}
#else
		if(buf[0] != 0x0a)
		{
		//printf("befor atoi %02x %02x %02x\n", buf[0], buf[1], buf[2]);	
		int log_type = (LOGTYPE)atoi(buf);
		//printf("befor atoi %d\n", log_type);	
		pfile = strstr(buf," ");
		//printf("befor ++pfile\n");	
		pMove = strstr(++pfile," ");	
		//printf("befor copy\n");	
		memcpy(str,pfile,pMove - pfile);
		//printf("after copy\n");	
		//str[pMove - pfile] = 0;
		//printf("%d %s %s\n", log_type, str, pMove);
		#if 1
		if ((LOGTYPE)type == LOG_ALL || (LOGTYPE)type == log_type)
		{
			str_LOG += str;
			str_LOG += "  ";
			str_LOG += ++pMove;
			sprintf(sType, "%d ", log_type);
			g_LogAll += sType;
			g_LogAll += str;
			g_LogAll += " ";
			g_LogAll += pMove;
		}
#endif
	}
#endif
	}
		
	//NEW
	//printf("str_LOG=%s", str_LOG.c_str());	

	return 0;
}

