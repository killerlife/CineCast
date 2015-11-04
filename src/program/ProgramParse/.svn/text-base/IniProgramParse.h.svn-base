#ifndef INIPROGRAMPARSE_H_
#define INIPROGRAMPARSE_H_
#include "ProgramParse.h"
#include <ini.h>

class CIniProgramParse : public IProgramParse 
{
public:
	~CIniProgramParse();
	CIniProgramParse();
	int init(const std:: string& programPath);
	int getProgramNum()
	{
		return 1;
	}
	int parseProgramInfo(ProgramInfo& info, int index = 0);
	int getProgramFilePath(std::vector<ReelInfo>& filePathList, int index = 0);
	int getHashValue(std::vector<HashInfo>& hashInfo, int index = -1);
	int getProgramId(std::string& programId, int index = 0);
	int getLastError();
	std::string getKind(){return std::string("ECP");}
private:
	brunt::ICMyini* m_myIni;
	std::string m_programPath;
};

#endif /*INIPROGRAMPARSE_H_*/
