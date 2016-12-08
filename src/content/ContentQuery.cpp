#include "IContentQuery.h"
#include "IContentParser.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <dvb/mytype.h>

#include <log/Log.h>

using namespace std;
namespace fs = boost::filesystem;

class ContentQuery : public IContentQuery 
{
public:
	virtual ~ContentQuery();
	ContentQuery();
	virtual int open(const std::string& root, bool usePath = false);
	virtual int getProgramCount();
	virtual int getProgramIndex(const std::string& programId);
	virtual int getProgramInfo(int index, TContentInfo& info);
	virtual int getProgramFiles(int index, std::vector<std::string>& filesList);	// get all files of one program, for import
	virtual int getProgramPath(int index, std::string& path);	// get the program root path
	virtual int getProgramPath(const std::string& programId, std::string& path);	// get the program root path

private:
	int init(const string& root);
	void findPath(const fs::path& root , std::vector<std::string>& pathlist);

	std::string m_root;
	//	std::list<CMyProgrameInfo> m_programInfoList;
	std::vector<TContentInfo> m_programInfoList;

	typedef std::vector<std::string> TPaths;
	std::vector<TPaths> m_programFiles;
	//	std::vector<std::string> m_programList;
	ILog* pLog;
};

IContentQuery* createContentQuery()
{
	return new ContentQuery;
}

void releaseContentQuery(IContentQuery*& contentQuery)
{
	delete contentQuery;
	contentQuery = NULL;
}


ContentQuery::ContentQuery()
{
	pLog = CreateLog();
}

ContentQuery::~ContentQuery()
{
	m_root = "";
	if (pLog)
	{
		ReleaseLog(pLog);
	}
}

int ContentQuery::open(const std::string& root, bool usePath )
{
	std::string diskRoot = "/storage";
	std::string usbRoot = "/media/usb";
	std::string raidRoot = "/raid";
#ifdef ENABLE_RAID
	std::string ftpRoot = "/raid";
#else
	std::string ftpRoot = "/storage/ftp";
#endif
	m_root = root;

	char str[512];
	if(!usePath)
	{
		if(root == "hdisk")
			m_root = diskRoot;
		else if(root == "usb")
			m_root = usbRoot;
		else if(root == "raid")
			m_root = raidRoot;
		else if(root == "ftp")
			m_root = ftpRoot;
		else
		{
			sprintf(str, "[ContentQuery] Unknow root: %s", root.c_str());
			if (pLog)
			{
				pLog->Write(LOG_ERROR, str);
			}
			return -1;
	}
	}


	return init(m_root);
}

int ContentQuery::getProgramCount()
{
	return (int)m_programInfoList.size();
}

int ContentQuery::getProgramIndex(const std::string& programId)
{
	for(int index=0 ; index<(int)m_programInfoList.size() ; index++)
	{
		if(programId==m_programInfoList[index].id)
			return index;
	}
	return -1;
}

int ContentQuery::getProgramInfo(int index, TContentInfo& info)
{
	if(index>=0 && index<(int)m_programInfoList.size())
	{
		info = m_programInfoList[index];
		return 0;
	}
	return -1;
}

int ContentQuery::getProgramFiles(int index, std::vector<std::string>& filesList)
{
	if(index>=0 && index<(int)m_programFiles.size())
	{
		filesList = m_programFiles[index];
		return 0;
	}
	return -1;
}

bool is_dcp_directory(const fs::path& path)
{
	return true;
}

void ContentQuery::findPath(const fs::path& root , std::vector<std::string>& programList)
{
	char str[512];
	try
	{
		// find the ASSETMAP file whether is exist in root path
		//DPRINTF("%s \n", root.native().c_str());
		if(is_dcp_directory(root))
			programList.push_back(root.native());

		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(root); itr != end_itr; ++itr)
		{
			try
			{
				boost::filesystem::path* dir_name = NULL;
				if(fs::is_symlink(*itr))
				{
					sprintf(str, "[ContentQuery] findPath: dir is symlink %s.", ((fs::path)*itr).native().c_str());
					if(pLog)
						pLog->Write(LOG_ERROR, str);
				}
				else if(fs::is_directory(*itr))// && is_dcp_directory(*itr))
				{
					findPath(*itr, programList);
					//programList.push_back(itr->native_file_string());
				}
			}
			catch(const fs::filesystem_error& e)
			{
				sprintf(str, "[ContentQuery] findPath: Skip the IO error, except: %s.", e.what());
				if (pLog)
				{
					pLog->Write(LOG_ERROR, str);
				}
				DPRINTF("[ContentQuery] findPath: Skip the IO error, except: %s.", e.what());
			}
		}
	}
	catch(const fs::filesystem_error& e)
	{
		//run on 8634, when last ++itr, always throw exception, so omit this exception.
		//return get_program_list_catch_exception_error;
		//cout << "CProgramQuery::getProgramList, except:" << e.what() << endl;
		sprintf(str, "[ContentQuery] findPath: except: %s.", e.what());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
	}
}

int ContentQuery::init(const string& root)
{
	DPRINTF("[ContentQuery] init root path: %s\n", root.c_str());

	int result = 0;
	std::vector<std::string> pathlist;

	char str[512];

	try
	{
		fs::path path(root);
		if(!fs::exists(path))
			return -1;

		findPath(path, pathlist);

		if(pathlist.size()==0)
		{
			sprintf(str, "[ContentQuery] program path list is null: %s", root.c_str());
			DPRINTF("[ContentQuery] program path list is null.\n");
			if (pLog)
			{
				pLog->Write(LOG_ERROR, str);
			}
			return 0;
		}

		for(unsigned int i = 0; i<pathlist.size() && result==0; ++i)
		{
			IContentParser* pParser = CreateContentParser();
			if(pParser)	
			{
				if(!pParser->open(pathlist[i]))
				{
					sprintf(str, "[ContentQuery] '%s' is not valid program path.", pathlist[i].c_str());
					if (pLog)
					{
						pLog->Write(LOG_ERROR, str);
					}
					DPRINTF("[ContentQuery] '%s' is not valid program path.\n", pathlist[i].c_str());
					continue;
				}
				for(int j = 0; j< pParser->getProgramNum(); j++)
				{
					TContentInfo ci;
					DcpInfo di;
					DPRINTF("[ContentQuery] init: parse %d\n", j);
					pParser->parseProgramInfo(di, j);
					ci.id = di.id;
					ci.rootpath = pathlist[i];
					ci.fileName = di.title;
					ci.issueDate = di.issueDate;
					ci.issuer = di.issuer;
					ci.fileLength = di.filmLength;
					ci.progress = di.recvRatio;
					ci.stereoScopic = di.stereoScopic;
					ci.status = di.recvStatus;
					ci.totalSegment = di.totalSegment;
					ci.dateTime = di.recvDateTime;
					ci.receiveSegment = di.recvSegment;
					ci.country = di.pklFile;
					ci.creator = di.creator;
					m_programInfoList.push_back(ci);
				}

				ReleaseContentParser(&pParser);
			}
		}
		//		DPRINT((DP_Trace, "CProgramQuery", "'init m_programInfoList ... ok"));

		if(result!=0)
		{
			sprintf(str, "[ContentQuery] program parse error: %d.", result);
			if (pLog)
			{
				pLog->Write(LOG_ERROR, str);
			}
			DPRINTF("[ContentQuery] program parse error: %d.\n", result);
		}

	}
	catch(const fs::filesystem_error& e)
	{
		//run on 8634, when last ++itr, always throw exception, so omit this exception.
		//return get_program_list_catch_exception_error;
		//cout << "CProgramQuery::getProgramList, except:" << e.what() << endl;
		sprintf(str, "[ContentQuery] init unknown error: %s.\n", e.what());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		DPRINTF("[ContentQuery] init unknown error: %s.\n", e.what());
	}

	return result;
}

int ContentQuery::getProgramPath(int index, std::string& path)
{
	if(index>=0 && index<(int)m_programInfoList.size())
	{
		path = m_programInfoList[index].rootpath;
		return 0;
	}
	return -1;
}

int ContentQuery::getProgramPath(const std::string& programId, std::string& path)
{
	int index = getProgramIndex(programId);
	if(index<0) return -1;

	path = m_programInfoList[index].rootpath;
	return 0;
}