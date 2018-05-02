#pragma once
#include "LogStruct.h"

class ILogFileOperation
{
public:
	virtual int IsDirectory(const char* path) = 0;
	virtual int CreateDirectory(const char* path) = 0;
	virtual int DeleteDirectory(const char* path) = 0;
	
	virtual int OpenFile(const char* file) = 0;
	virtual int CloseFile() = 0;

	virtual int Write(const char* file,const int type, const char* context) = 0;
	virtual int Read(const int type, TLogQueryResultArray & result) = 0;
};


class CLogFileOperation : public ILogFileOperation
{
public:
	CLogFileOperation(void);
	~CLogFileOperation(void);

public:
	int IsDirectory(const char* path);
	int CreateDirectory(const char* path);
	int DeleteDirectory(const char* path);

	int OpenFile(const char* file);
	int CloseFile();

	int Write(const char* file,const int type, const char* context);
	int Read(const int type, TLogQueryResultArray & result);

private:
	FILE* m_pfile;

};
