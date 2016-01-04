#ifndef _CONTENT_QUERY_H_200812021419__
#define _CONTENT_QUERY_H_200812021419__

#include <vector>
#include <string>

struct TContentInfo
{
	std::string id;			// 影片ID
	std::string fileName;		// 影片名称
	std::string type;			// 影片类型
	std::string progress; 
	std::string duration;	// 影片时长(分钟)
	std::string issuer;		// 发行者
	std::string issueDate;	// 发行时间
	std::string creator;
	std::string country;		// 国家
	std::string language;	// 语言
	std::string subtitle;	// 字幕
	std::string rootpath;	// 节目所在目录
	std::string license;	// 授权方式
	std::string status;
	std::string fileLength; //文件长度
	std::string stereoScopic; //3D/2D
	std::string dateTime; //接收时间
	std::string totalSegment; //總分片
	std::string receiveSegment; //接收的分片
};

struct CHashInfo
{
	std::string fileName;
	std::string hashValue;
};

class IContentQuery 
{
public:
	virtual ~IContentQuery(){};
	virtual int open(const std::string& root, bool usePath = false) = 0;
	virtual int getProgramCount() = 0;
	virtual int getProgramIndex(const std::string& programId) = 0;
	virtual int getProgramInfo(int index, TContentInfo& info) = 0;
	virtual int getProgramFiles(int index, std::vector<std::string>& filesList) = 0;	// get all files of one program, for import
	virtual int getProgramPath(int index, std::string& path) = 0;	// get the program root path
	virtual int getProgramPath(const std::string& programId, std::string& path) = 0;	// get the program root path
};

IContentQuery* createContentQuery();
void releaseContentQuery(IContentQuery*& contentQuery);

#endif//_CONTENT_QUERY_H_200812021419__

