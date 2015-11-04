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
/*    显示在界面上的详细信息内容
	"节目ID:",
	"节目名称:",
	"节目类型:",
	"授权方式:",
	"视频格式:",
	"打包方式:",
	"节目长度:",
	"分辨率:",
	"发行商:",
	"发行日期:",
	"档期的开始时间:"T"档期的结束时间:",
	"国家:",
	"发行国家、区域:",
	"配音语言:",
	"是否加密",
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


