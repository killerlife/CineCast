#ifndef _TA_NEWV8_IBREAKPOINT_IMPORT_H__
#define _TA_NEWV8_IBREAKPOINT_IMPORT_H__
#include <string>
#include <vector>
#ifdef _WIN32
#ifdef NEWV8_IBREAKPOINT_IMPORT_EXPORTS
#define NEWV8_IBREAKPOINT_IMPORT_API __declspec(dllexport)
#else
#define NEWV8_IBREAKPOINT_IMPORT_API __declspec(dllimport)
#endif

#else 
#define NEWV8_IBREAKPOINT_IMPORT_API
#endif



#include <string>

struct CBreakPointImportData
{
	std::string    sID;              // 节目ID
	int                nType;        //  节目类型
	unsigned int  nProSize;    // 已经导入的节目字节数
	std::string    sFilepath;   //  全路径
	
	CBreakPointImportData(const char* id = ""):sID(id),nProSize(0),sFilepath("")
	{

	}
};


class IBreakPointImport
{
public:
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool write(const CBreakPointImportData& breakPoint) = 0;
	virtual bool read(CBreakPointImportData& breakPoint) = 0;
	virtual bool del() = 0;
	virtual void release() = 0;	
};

IBreakPointImport* createBreakPointImport();

void releaseBreakPointImport(IBreakPointImport *pImport);

#endif
