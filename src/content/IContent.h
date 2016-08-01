#pragma once

#include <string>
#include <vector>
using namespace std;

typedef enum{
	PST_HDD = 0,
	PST_USB,
	PST_RAID,
	PST_FTP,
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
/*    ʾڽϵϸϢ
	"Film ID:",  0
	"Ŀ:", 1
	"ɶ:", 2
	"״̬:", 3
	"Ƶʽ:", 4
	"ļ:", 5
	"Ŀ:", 6 
	"2D/3D:", 7
	":", 8
	":",
	"ڵĿʼʱ:"T"ڵĽʱ:",
	"PKL file:",
	"йҡ:",
	"Օrg:",
	"·",
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