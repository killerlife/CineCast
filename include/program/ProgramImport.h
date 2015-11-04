// ProgramImport.h : ProgramImport DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	//#error include 'stdafx.h' before including this file for PCH
#endif

// #include "resource.h"		// 主符号


// CProgramImportApp
// 有关此类实现的信息，请参阅 ProgramImport.cpp
//

#ifndef NEW_V8_PROGRAM_IMPORT_H
#define NEW_V8_PROGRAM_IMPORT_H
#include <string>
#include <vector>


#ifdef _WIN32
#ifdef NEWV8PROGRAM_IMPORT_EXPORTS
#define NEWV8PROGRAMIMPORT_API __declspec(dllexport)
#else
#define NEWV8PROGRAMIMPORT_API __declspec(dllimport)
#endif

#else 
#define NEWV8PROGRAMIMPORT_API
#endif

enum IMPORT_STATUS
{
	IMPORT_STATUS_READY   = 0,
	IMPORT_STATUS_WROKING = 1,
	IMPORT_STATUS_STOP    = 2,
	IMPORT_STATUS_END     = 3,
	IMPORT_STATUS_ERROR   = 4
};

struct ImportCheckProgress
{
	int id;
	std::string filename;
	int progress;
	unsigned int filesize; // K
	ImportCheckProgress():id(0), progress(0), filesize(0)
	{
	}
};

struct ImportProgress
{
	std::vector<ImportCheckProgress> progs;
	int totalProgress;	// [0,100]
	bool isEnd;
	ImportProgress():totalProgress(0), isEnd(false)
	{
	}
};


struct filename
{
	std::string m_src;
	std::string m_dest;
};

class NEWV8PROGRAMIMPORT_API IProgramImport 
{
public:
	virtual ~IProgramImport(){}
	virtual int import(const std::string& programPath, const std:: string& lid) = 0;
	virtual const ImportProgress& progress() = 0;
	virtual int getImportStatus() = 0;
	virtual int stop() = 0;
	virtual long long int getProgramSize() = 0;
	virtual int getImportSize(long long int& status) = 0;
	virtual void setProgramType(int ProgramType) = 0; 
	virtual bool isBreakPointImport(std::string& id,unsigned int& nSize,int& nType) = 0;	
	virtual bool DelBreakPointImport() = 0;
	virtual void DelBreakPointImportFile() = 0;
	virtual int getLastError() = 0;
};

NEWV8PROGRAMIMPORT_API IProgramImport* createProgramImport();
NEWV8PROGRAMIMPORT_API void releaseProgramImport(IProgramImport* programImport);

#endif

/*
class CProgramImportApp : public CWinApp
{
public:
	CProgramImportApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
*/