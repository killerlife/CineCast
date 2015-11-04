#ifndef DCPPROGRAMPARSE_H_
#define DCPPROGRAMPARSE_H_
#include "ProgramParse.h"
class IDcpParser;
class CDcpProgramParse : public IProgramParse 
{
public:
	~CDcpProgramParse();
	CDcpProgramParse();
	int init(const std:: string& programPath);
	int getProgramNum();
	int parseProgramInfo(ProgramInfo& info, int index = 0);
	int getProgramFilePath(std::vector<ReelInfo>& filePathList, int index = 0);
	int getHashValue(std::vector<HashInfo>& hashInfo, int index = -1);
	int getProgramId(std::string& programId, int index = 0);
	int getLastError();
	std::string getKind(){return std::string("DCP");}
private:
	std::string m_programPath;
	IDcpParser* m_pParser;
};

#endif /*DCPPROGRAMPARSE_H_*/
