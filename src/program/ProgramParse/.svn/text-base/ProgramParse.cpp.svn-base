#include "IniProgramParse.h"
#include "DcpProgramParse.h"
#include <ProgramError.h>
#include <iostream>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

class CProgramParse : public IProgramParse 
{
public:
	~CProgramParse();
	CProgramParse();
	int init(const std:: string& programPath);
	int getProgramNum();
	int parseProgramInfo(ProgramInfo& info, int index = 0);
	int getProgramFilePath(std::vector<ReelInfo>& filePathList, int index = 0);
	int getHashValue(std::vector<HashInfo>& hashInfo, int index = -1);
	int getProgramId(std::string& programId, int index = 0);
	int getLastError();
	std::string getKind();
private:
	void clear();	
private:
	CIniProgramParse* m_iniParse;
	CDcpProgramParse* m_dcpParse;
};

PROGRAMPARSE_API IProgramParse* createProgramParse()
{
	return new CProgramParse;
}

PROGRAMPARSE_API void releaseProgramParse(IProgramParse* programParse)
{
	delete programParse;
}

CProgramParse::CProgramParse():m_iniParse(NULL),m_dcpParse(NULL)
{

}

CProgramParse::~CProgramParse()
{
	clear();
}

void CProgramParse::clear()
{
	if(m_dcpParse)
		delete m_dcpParse;
	if(m_iniParse)
		delete m_iniParse;
	m_dcpParse = NULL;
	m_iniParse = NULL;
}

int CProgramParse::init(const std::string& programPath)
{
	clear();
	m_dcpParse = new CDcpProgramParse();
	if(m_dcpParse && m_dcpParse->init(programPath) == no_error)
		return no_error;
	delete m_dcpParse	;
	m_dcpParse = NULL;

	m_iniParse = new CIniProgramParse();
	if(m_iniParse && m_iniParse->init(programPath) == no_error)
		return no_error;
	delete m_iniParse	;
	m_iniParse = NULL;
		
	return load_describe_file_error;
}

int CProgramParse::getProgramNum()
{

	if(m_dcpParse)
		return m_dcpParse->getProgramNum();
	if(m_iniParse)
		return m_iniParse->getProgramNum();
	return 0;
}
	
	
int CProgramParse::getProgramId(std::string& programId, int index)
{
	if(m_dcpParse)
		return m_dcpParse->getProgramId(programId, index);
	if(m_iniParse)
		return m_iniParse->getProgramId(programId, index);
	return load_describe_file_error;
}

int CProgramParse::parseProgramInfo(ProgramInfo& info, int index)
{
	if(m_dcpParse)
		return m_dcpParse->parseProgramInfo(info, index);
	if(m_iniParse)
		return m_iniParse->parseProgramInfo(info, index);
	return load_describe_file_error;
}

int CProgramParse::getProgramFilePath(std::vector<ReelInfo>& filePathList, int index)
{
	if(m_dcpParse)
		return m_dcpParse->getProgramFilePath(filePathList, index);

	if(m_iniParse)
		return m_iniParse->getProgramFilePath(filePathList, index);
	return load_describe_file_error;
}

int CProgramParse::getHashValue(std::vector<HashInfo>& hashInfo, int index)
{
	if(m_dcpParse)
		return m_dcpParse->getHashValue(hashInfo, index);

	if(m_iniParse)
		return m_iniParse->getHashValue(hashInfo, index);
	return load_describe_file_error;
}

int CProgramParse::getLastError()
{
	if(m_dcpParse)
		return m_dcpParse->getLastError();

	if(m_iniParse)
		return m_iniParse->getLastError();
	return load_describe_file_error;
}
std::string CProgramParse::getKind()
{
	return std::string("");
}
