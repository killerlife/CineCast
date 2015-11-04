#ifndef NEW_V8_IMPORT_THREAD_H
#define NEW_V8_IMPORT_THREAD_H
#include "ProgramImport.h"
#include <activeThread.h>
#include <string>
#include <vector>
#include <iostream>

class ImportObserver
{
public:
	virtual void notify(int status,int nIsEnd) = 0;
};

class CImportThread : public brunt::CActiveThread
{
public:
	CImportThread();
	virtual ~CImportThread() {}
	int init(const std::vector<filename>& filelist);
	int importFile();
	long long int progress();
	int getStatus() { return m_status; }
	void setObserver(ImportObserver* pObserver) { m_pObserver = pObserver; }
	void setAdPath(std::string& adPath,std::string& adId);
	void setProgress();
private:
	virtual void doit();
private:
	std::string m_src;
	std::string m_dest;
	std::vector<filename>* m_pfilelist;
	long long int m_progress;
	int m_status;
	ImportObserver* m_pObserver;
	std::string m_adPath;
	std::string m_adId;
	int m_nIsEnd;
};

#endif
