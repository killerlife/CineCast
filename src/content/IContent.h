#pragma once

#include <string>
#include <vector>
using namespace std;

typedef enum{
	PST_HDD = 0,
	PST_USB,
	PST_RAID,
	PST_SIZEOF
};

typedef enum  ProgramType
{
	TYPE_MOVIE = 0, 
	TYPE_PUBLICAD,
	TYPE_BUSINESSAD,
	TYPE_FILE_LIST
} ;

struct InfoData 
{
	std::string  pData[16];
/*    显示在界面上的详细信息内容
	"Film ID:",  0
	"节目名称:", 1
	"完成度:", 2
	"接收状态:", 3
	"视频格式:", 4
	"文件长度:", 5
	"节目长度:", 6 
	"2D/3D:", 7
	"发行商:", 8
	"发行日期:",
	"档期的开始时间:"T"档期的结束时间:",
	"PKL file:",
	"发行国家、区域:",
	"接收時間:",
	"路徑",
	"Creator",

	enum{
	C_ID = 0,
	C_NAME,
	C_PROGRESS,
	C_STATUS,
	C_FORMAT,
	C_FILM_LENGTH,
	C_PROGRAM_LENGTH,
	C_STEREO,
	C_ISSUER,
	C_ISSUEDATE,
	C_TIMERANGE,
	C_RECVSEGMENT,
	C_TOTAL_SEGMENT,
	C_RECV_DATETIME,
	C_LOCATE,
	};
*/
};

class IContentManager
{
public:
	virtual int update(const vector<int>& updateSrcList) = 0;
	virtual bool isReady(int nSource) = 0;
	virtual int getLastError(string*msg) = 0;
	virtual int getProgramFileList(int nSrc, int type, std::vector<InfoData>& programList) = 0;
	virtual ~IContentManager(){};
};

IContentManager* getIContentManager();