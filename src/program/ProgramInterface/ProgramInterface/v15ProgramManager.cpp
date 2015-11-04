#include "v15Interface.h"
#include "v15ProgramManager.h"
//#include "v15ProgramInterface.h"
#include "config/config.h"

#include <iostream>
using namespace std;

static CV15ProgramManager g_ProgramManager;


CV15ProgramManager::CV15ProgramManager()
{
	if(m_programManager == NULL)
		m_programManager = getIProgramManager();
}

CV15ProgramManager::~CV15ProgramManager()
{
// 	if(m_programManager)
// 		releaseProgramImport(m_programManager);	
}

IProgramManager* CV15ProgramManager::getProgramManagerObject()
{
	return m_programManager;
}

int v15ProgramUpdate(const vector<int> srcList)
{
	IProgramManager* pProgramManager = g_ProgramManager.getProgramManagerObject();
	return pProgramManager->update(srcList);
}

bool v15ProgramIsReady(int nSrc)
{
	IProgramManager* pProgramManager = g_ProgramManager.getProgramManagerObject();
	return pProgramManager->isReady(nSrc);
}

int v15GetProgramList(StorageSource nSrc, ProgramType type, std::vector<InfoData>& ProgramList)
{
#if 1
	IProgramManager* pProgramManager = g_ProgramManager.getProgramManagerObject();
	return pProgramManager->getProgramFileList(nSrc, type, ProgramList);
#else
	return 0;
#endif
	//	DPRINT((DP_Trace, ">>>>>>>>>>>>>>>>", "v15GetProgramList." ));
// 	return getV15ProgramManager().getProgramFileList(nSrc, ProgramList);
}

#if 0
int v15StartImportProgram(ProgramType type, const string& programId, long long  int& programSize)
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	pProgramImport->setProgramType(type);
	int nRet = 0;

	string programPath = getConfig().getUsbRootPath();
	nRet  = pProgramImport->import(programPath, programId);

	programSize = pProgramImport->getProgramSize() ;
	return nRet;
}

int v15StopImportProgram()
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	return pProgramImport->stop();
}

int v15GetCopySize(long long int& pSize)
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	return pProgramImport->getImportSize(pSize);	
}

int v15IsBreakPointImport(string& id,unsigned int& nSize,int& nType)
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	return pProgramImport->isBreakPointImport(id, nSize,nType);
}

int v15DelBreakPointImport()
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	return pProgramImport->DelBreakPointImport();
}

void v15DelBreakPointImportFile()
{
	IProgramImport* pProgramImport = g_ImportInterface.getProgramImportObject();
	pProgramImport->DelBreakPointImportFile();
}

#endif