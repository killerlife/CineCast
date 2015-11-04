#include "v15Interface.h"
#include "v15ImportInterface.h"
//#include "v15ProgramInterface.h"
#include "config/config.h"

#include <iostream>
using namespace std;

static CV15ImportInterface g_ImportInterface;


CV15ImportInterface::CV15ImportInterface()
{
	if(m_programImport == NULL)
		m_programImport = createProgramImport();

}

CV15ImportInterface::~CV15ImportInterface()
{
	if(m_programImport)
		releaseProgramImport(m_programImport);	
}

IProgramImport* CV15ImportInterface::getProgramImportObject()
{
	return m_programImport;
}


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

