#ifndef __PROGRAMPARSE_H_200812031452__
#define __PROGRAMPARSE_H_200812031452__

#include <vector>
#include <string>

#ifdef _WIN32
#ifdef PROGRAMPARSE_EXPORTS
#define PROGRAMPARSE_API __declspec(dllexport)
#else
#define PROGRAMPARSE_API __declspec(dllimport)
#endif

#else 
#define PROGRAMPARSE_API
#endif

struct AudioInfo
{
	std::string type;
	std::string sampleRate;
	std::string bitIndeepness;
	std::string channel;
};

struct VideoInfo
{
	std::string type;
	std::string frameRate;
	std::string duration;
	std::string aspectRatio;
};

struct ProgramInfo
{
	std::string id;			// 影片ID
	std::string title;		// 影片名称
	std::string type;			// 影片类型
	std::string contentType;
	std::string duration;	// 影片时长(分钟)
	std::string issuer;		// 发行者
	std::string issueDate;	// 发行时间
	std::string creator;
	std::string country;		// 国家
	std::string language;	// 语言
	std::string subtitle;	// 字幕
	VideoInfo videoInfo;		// 视频信息
	AudioInfo audioInfo;		// 音频信息
};
struct HashInfo
{
	std::string fileName;
	std::string hashValue;
};
//{
//	std::string startDate;			// 档期开始日期
//	std::string endDate;			// 档期结束日期
//	std::string authorizationedTimes;	// 授权场次
//	std::string playedTimes;		// 已播放场次
//	std::string remainedTimes;		// 剩余场次
//	int hashVerify;			// 哈希验证,-1:验证失败;0:未验证;1:验证成功
//	int authorizationVerify;		// 授权验证,-1:验证失败;0:未验证;1:验证成功
//};

struct ReelFileInfo
{
	std::string    keyIdStr;
	std::string    fileName;
	long    		 entryPoint;
	long			 dur;
	ReelFileInfo():entryPoint(0), dur(24*60*60*100)
	{
	}
};
struct ReelInfo
{
	ReelFileInfo  viedoInfo;
	ReelFileInfo  audioInfo;
//	subtitle;
};

class PROGRAMPARSE_API IProgramParse 
{
public:
	virtual ~IProgramParse(){}
	virtual int init(const std:: string& programPath) = 0;
	virtual int getProgramNum() = 0;
	virtual int parseProgramInfo(ProgramInfo& info, int index = 0) = 0;
	virtual int getProgramFilePath(std::vector<ReelInfo>& filePathList, int index = 0) = 0;
	virtual int getHashValue(std::vector<HashInfo>& hashInfo, int index = -1) = 0;
	virtual int getProgramId(std::string& programId, int index = 0) = 0;
	virtual int getLastError() = 0;
	virtual std::string getKind()=0;
};

PROGRAMPARSE_API IProgramParse* createProgramParse();
PROGRAMPARSE_API void releaseProgramParse(IProgramParse* programParse);

#endif//__PROGRAMPARSE_H_200812031452__

