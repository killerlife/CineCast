///////////////////////////////////////////////////
// lp 2009.07.14
#pragma once

#include "ProgramQuery.h"
#include "ProgramImport.h"

#include <string>
#include <vector>
using namespace std;

#ifdef _WIN32
#ifdef PROGRAMMANAGER_EXPORTS
#define PROGRAMMANAGER_API __declspec(dllexport)
#else
#define PROGRAMMANAGER_API __declspec(dllimport)
#endif

#else 
#define PROGRAMMANAGER_API
#endif

typedef enum{
	PST_HDD = 0,
	PST_USB,
	PST_SIZEOF
}PROGRAM_SOURCE_TYPE;

struct InfoData 
{
	std::string  pData[16];
/*    ��ʾ�ڽ����ϵ���ϸ��Ϣ����
	"��ĿID:",
	"��Ŀ����:",
	"��Ŀ����:",
	"��Ȩ��ʽ:",
	"��Ƶ��ʽ:",
	"�����ʽ:",
	"��Ŀ����:",
	"�ֱ���:",
	"������:",
	"��������:",
	"���ڵĿ�ʼʱ��:"T"���ڵĽ���ʱ��:",
	"����:",
	"���й��ҡ�����:",
	"��������:",
	"�Ƿ����",
*/
};

class IProgramManager
{
public:
	virtual IProgramQuery* getIProgramQuery(int nSource) = 0; 
	virtual IProgramImport* getIProgramImport() = 0;
	// update programs' info of some source 
	virtual int update(const vector<int>& updateSrcList) = 0;
	// the program data whether is ready
	virtual bool isReady(int nSource) = 0;
	// get error message
	virtual int getLastError(string* msg) = 0;

	virtual int getProgramFileList(int nSrc, int type, std::vector<InfoData>& ProgramList)=0;

	virtual ~IProgramManager(){};
};

PROGRAMMANAGER_API IProgramManager* getIProgramManager();


