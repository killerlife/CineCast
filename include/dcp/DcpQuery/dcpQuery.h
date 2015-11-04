#ifndef DCP_QUERY_H
#define DCP_QUERY_H
#include <vector>
#include <string>

#ifdef _WIN32
#ifdef DCP_QUERY_EXPORTS
#define DCPQUERY_API __declspec(dllexport)
#else
#define DCPQUERY_API __declspec(dllimport)
#endif

#else 
#define DCPQUERY_API
#endif

class LessThan
{
public:
	bool operator()(const std::string &s1, const std::string &s2)
	{
		return (s1.size() < s2.size());
	}
};

struct FilePath
{
	std::string name;	// dcp name
	std::string path;	// dcp path
};

enum CPLINFOTYPE
{
	COMPOSITION_FILE = 0,
	PACKINGLIST_FILE = 1,
	DCP_PATH = 2
};

enum QUERYTYPE
{
	USB_QUERY = 0,
	DISK_QUERY = 1,
	FTP_QUERY = 2
};

class DCPQUERY_API IDcpQuery 
{
public:
	virtual ~IDcpQuery(){};
	virtual bool open(QUERYTYPE type = DISK_QUERY) = 0;
	virtual bool getDcpList(std::vector<FilePath>& dcpList) = 0;
	virtual const std::string queryCplInfo(const std::string& cplId, unsigned int infoType) = 0;
	virtual bool getRootPath(std::vector<std::string>& rootPath) = 0;
	virtual void close() = 0;
	virtual int getLastError() = 0;
};

DCPQUERY_API IDcpQuery* createDcpQuery();
DCPQUERY_API void releaseDcpQuery(IDcpQuery* dcpQuery);

#endif
