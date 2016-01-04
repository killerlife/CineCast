#pragma once

#include <string>
#include <vector>

struct  DcpInfo
{
	std::string id;
	std::string title;
	std::string filmLength;
	std::string recvRatio;
	std::string stereoScopic;
	std::string recvStatus;
	std::string issuer;
	std::string issueDate;
	std::string creator;
	std::string recvDateTime;
	std::string pklFile;
	std::string totalSegment;
	std::string recvSegment;
	std::string lostSegment;
	std::string crcError;
};

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

class IContentParser 
{
public:
	//for get packinglistFile and composition's absolute file name,path must be absolute path!
	virtual bool open(const std::string& path)=0;
	virtual void close()=0;

	virtual int getProgramNum()=0;
	virtual int parseProgramInfo(DcpInfo& info, int index = 0)=0;
	virtual int getProgramFilePath(std::vector<ReelInfo>&filePathList, int index = 0)=0; //(add by qwy)根据uuid得到cpl文件(包含路径)
	virtual int getProgramId(std::string programId, int index = 0)=0;

	virtual int getLastError()=0;

};
IContentParser* CreateContentParser();
void ReleaseContentParser(IContentParser** ppParser);

