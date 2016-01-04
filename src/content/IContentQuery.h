#ifndef _CONTENT_QUERY_H_200812021419__
#define _CONTENT_QUERY_H_200812021419__

#include <vector>
#include <string>

struct TContentInfo
{
	std::string id;			// ӰƬID
	std::string fileName;		// ӰƬ����
	std::string type;			// ӰƬ����
	std::string progress; 
	std::string duration;	// ӰƬʱ��(����)
	std::string issuer;		// ������
	std::string issueDate;	// ����ʱ��
	std::string creator;
	std::string country;		// ����
	std::string language;	// ����
	std::string subtitle;	// ��Ļ
	std::string rootpath;	// ��Ŀ����Ŀ¼
	std::string license;	// ��Ȩ��ʽ
	std::string status;
	std::string fileLength; //�ļ�����
	std::string stereoScopic; //3D/2D
	std::string dateTime; //����ʱ��
	std::string totalSegment; //����Ƭ
	std::string receiveSegment; //���յķ�Ƭ
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

