#ifdef WIN32
#include "stdafx.h"
#endif
#include "breakPointImport.h"
#include <ini.h>
#include <fstream>
#include <iostream>
//#include <sysLog.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "config/config.h"
using namespace std;
using namespace brunt;

// Aavan:2008-7-6 �ϵ���ϢSECTION
#define BKP_SECTION_IMPORT			           "V15_BREAKPOINT_IMPORT"
#define CONFIG_ROOT_IMPORT_PATH               "/hdisk/data/bp/"
#define CONFIGBK_ROOT_IMPORT_PATH           "/hdisk/databackup/bp/"

class CBreakPointImport: public IBreakPointImport
{
public:
	CBreakPointImport();
	virtual ~CBreakPointImport();
	bool open();
	void close();
	bool write(const CBreakPointImportData& breakPoint);
	bool read(CBreakPointImportData& breakPoint);
	bool del();
	void release();
private:
    int save();
private:    
	ICMyini* m_pIni;	
};


IBreakPointImport* createBreakPointImport()
{

	return new CBreakPointImport();
}

void releaseBreakPointImport(IBreakPointImport *pMgr)
{
	delete pMgr;
}


CBreakPointImport::CBreakPointImport():m_pIni(0)
{
}

CBreakPointImport::~CBreakPointImport()
{
	close();
}

bool CBreakPointImport::open()
{
	if(m_pIni)
		return true;

	m_pIni = createMyini();
	//TCHAR szPath[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szPath);
	string fileName = ".";//getConfig().getBinRootPath();
	fileName += "\\breakpointimport.ini";

	cout << "::open    filename = " << fileName <<endl;
	
	if(!m_pIni->load(fileName))
	{
		fstream f(fileName.c_str(), ios::out);
	}

	if(m_pIni->load(fileName))
		return true;
	else
		return false;
	
}

void CBreakPointImport::close()
{
	if(m_pIni)
	{
		save();
		releaseMyini(m_pIni);
		m_pIni = 0;
	}
}
bool CBreakPointImport::write(const CBreakPointImportData& breakPoint)
{
	if(!m_pIni)
		return false;
	if (!writeIni(*m_pIni, BKP_SECTION_IMPORT, "id" , breakPoint.sID.c_str()))
		return false;
	
	if (!writeIni(*m_pIni, BKP_SECTION_IMPORT, "type" , breakPoint.nType))
		return false;

	if (!writeIni(*m_pIni, BKP_SECTION_IMPORT, "size" , breakPoint.nProSize))
		return false;

	if (!writeIni(*m_pIni, BKP_SECTION_IMPORT, "path" , breakPoint.sFilepath.c_str()))
		return false;

		save();
	return true;
}

bool CBreakPointImport::read(CBreakPointImportData& breakPoint)
{
	if (!m_pIni)
		return false;
	
	if (!m_pIni->read(BKP_SECTION_IMPORT, "id", breakPoint.sID))
		return false;
	if (breakPoint.sID.empty())
		return false;
	

	readIni(*m_pIni, BKP_SECTION_IMPORT, "type", breakPoint.nType);
	readIni(*m_pIni, BKP_SECTION_IMPORT, "size", breakPoint.nProSize);
	
	if (!m_pIni->read(BKP_SECTION_IMPORT, "path", breakPoint.sFilepath))
		return false;
	if (breakPoint.sFilepath.empty())
		return false;
	cout << " CBreakPointImport::read" << endl;

	return true;
}

bool CBreakPointImport::del()
{
	if(!m_pIni)
		return false;

	//TCHAR szPath[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szPath);
	string fileName = ".";//getConfig().getBinRootPath();;
	fileName += "\\breakpointimport.ini";

	if (remove(fileName.c_str()) == -1)
		return true;

	return false;
}

void CBreakPointImport::release()
{
	delete this;
}

int CBreakPointImport::save()
{
	int result = 0;
	//TCHAR szPath[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szPath);
	string fileName = ".";//getConfig().getBinRootPath();
	fileName += "\\breakpointimport.ini";
	if(m_pIni->save(fileName))
		++result;

	return result;
}

