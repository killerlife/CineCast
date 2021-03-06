#include "ProgramQuery.h"
//#include "ProgramParse.h"
#include "program/ProgramError.h"
#include <DcpParser/DcpParser.h>
#include <DcpParser/AssistReel.h>
#include <DcpParser/AssistHash.h>
#include <DcpParser/AssistComposition.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <iostream>
#include <list>
#ifdef DEBUGPRINT
#include "brunt/debugprint.h"
#else
#ifdef DEBUG
#define DPRINT(arg,...) printf(arg,...)
#define DP_Trace(arg,...) printf(arg,...)
#define DP_Warning(arg,...) printf(arg,...)
#else
#define DPRINT(arg...)
#define DP_Trace(arg,...)
#define DP_Warning(arg,...)
#endif
#endif
#include "v15common.h"
#include "config/config.h"

using namespace std;
namespace fs = boost::filesystem;

#define DISKROOT	"/storage"
//getConfig().getProgramRootPath()	// "/hdisk/program/"

#define USBROOT		"/media/usb"
//getConfig().getUsbRootPath()		// "/usb/DMS_HDD/"

#define ASSETMAP	"ASSETMAP"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

struct CMyProgrameInfo
{
	std::string id;
	std::string type;
	std::string path;
};


class CProgramQuery : public IProgramQuery 
{
public:
	virtual ~CProgramQuery();
	CProgramQuery();
	virtual int open(const std::string& root, bool usePath = false);
	virtual int getProgramCount();
	virtual int getProgramIndex(const std::string& programId);
	virtual int getProgramInfo(int index, TProgramInfo& info);
	virtual int getProgramFiles(int index, std::vector<std::string>& filesList);	// get all files of one program, for import
	virtual int getProgramPath(int index, std::string& path);	// get the program root path
	virtual int getProgramPath(const std::string& programId, std::string& path);	// get the program root path
	virtual int getHashValue(int index, std::vector<CHashInfo>& hashInfo);

private:
	int init(const string& root);
	void findPath(const fs::path& root , std::vector<std::string>& pathlist);

	std::string m_root;
//	std::list<CMyProgrameInfo> m_programInfoList;
	std::vector<TProgramInfo> m_programInfoList;

typedef std::vector<std::string> TPaths;
	std::vector<TPaths> m_programFiles;
//	std::vector<std::string> m_programList;
};


IProgramQuery* createProgramQuery()
{
	return new CProgramQuery;
}

void releaseProgramQuery(IProgramQuery*& programQuery)
{
	delete programQuery;
	programQuery = NULL;
}


CProgramQuery::CProgramQuery()
{
}

CProgramQuery::~CProgramQuery()
{
	m_root = "";
}

int CProgramQuery::open(const std::string& root, bool usePath )
{
	std::string diskRoot = DISKROOT;
	std::string usbRoot = USBROOT;		// "/usb/" -> "/hdisk/usb/" : liang changed 2008.06.26 

	m_root = root;

	if(!usePath)
	{
		if(root == "hdisk")
			m_root = diskRoot;
		else if(root == "usb")
			m_root = usbRoot;
		else
			return init_root_path_error;
	}


	return init(m_root);
}

int CProgramQuery::getProgramCount()
{
	return (int)m_programInfoList.size();
}

int CProgramQuery::getProgramIndex(const std::string& programId)
{
	for(int index=0 ; index<(int)m_programInfoList.size() ; index++)
	{
		if(programId==m_programInfoList[index].id)
			return index;
	}
	return -1;
}

int CProgramQuery::getProgramInfo(int index, TProgramInfo& info)
{
	if(index>=0 && index<(int)m_programInfoList.size())
	{
		info = m_programInfoList[index];
		return 0;
	}
	return -1;
}

int CProgramQuery::getHashValue(int index, std::vector<CHashInfo>& hashInfo)
{
	DPRINT("CProgramQuery getHashValue had not been implemented!\n");
//	DPRINT((DP_Trace, "CProgramQuery", "getHashValue had not been implemented!"));
	return 0;
}

int CProgramQuery::getProgramFiles(int index, std::vector<std::string>& filesList)
{
	if(index>=0 && index<(int)m_programFiles.size())
	{
		filesList = m_programFiles[index];
		return 0;
	}
	return -1;
}

int CProgramQuery::getProgramPath(int index, std::string& path)
{
	if(index>=0 && index<(int)m_programInfoList.size())
	{
		path = m_programInfoList[index].rootpath;
		return 0;
	}
	return -1;
}

int CProgramQuery::getProgramPath(const std::string& programId, std::string& path)
{
	int index = getProgramIndex(programId);
	if(index<0) return -1;

	path = m_programInfoList[index].rootpath;
	return 0;
}


bool is_dcp_directory(const fs::path& path)
{
	return true;
}

void CProgramQuery::findPath(const fs::path& root , std::vector<std::string>& programList)
{
	try
	{
		// find the ASSETMAP file whether is exist in root path
		if(is_dcp_directory(root))
			programList.push_back(root.native());

		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(root); itr != end_itr; ++itr)
		{
			try
			{
				boost::filesystem::path* dir_name = NULL;
				//Modify by Killerlife
				if(fs::is_directory(*itr))// && is_dcp_directory(*itr))
				{
					//programList.push_back(itr->path().native());
					findPath(*itr, programList);
				}
				//End
			}
			catch(const fs::filesystem_error& e)
			{
				DPRINT("CProgramQuery findPath: Skip the IO error, except: %s.\n", e.what());
//				DPRINT((DP_Warning, "CProgramQuery", "findPath: Skip the IO error, except: %s.", e.what()));
			}
		}
	}
	catch(const fs::filesystem_error& )
	{
		//run on 8634, when last ++itr, always throw exception, so omit this exception.
		//return get_program_list_catch_exception_error;
		//cout << "CProgramQuery::getProgramList, except:" << e.what() << endl;
	}
}

int CProgramQuery::init(const string& root)
{
	DPRINT("CProgramQuery init root path: %s\n", root.c_str());
//	DPRINT((DP_Trace, "CProgramQuery", "init root path: %s", root.c_str()));

	int result = no_error;
	std::vector<std::string> pathlist;

	try
	{
		fs::path path(root);
		if(!fs::exists(path))
			return init_root_path_error;
		
		findPath(path, pathlist);

		if(pathlist.size()==0)
		{
			DPRINT("CProgramQuery program path list is null.\n");
//			DPRINT((DP_Error, "CProgramQuery", "program path list is null."));
			return 0;
		}

		for(unsigned int i = 0; i<pathlist.size() && result==no_error; ++i)
		{
			IDcpParser* pParser = CreateDcpParser();
			if(pParser)	
			{
				if(!pParser->open(pathlist[i]))
				{
					DPRINT("CProgramQuery '%s' is not valid program path.\n", pathlist[i].c_str());
//					DPRINT((DP_Warning, "CProgramQuery", "'%s' is not valid program path.", pathlist[i].c_str()));
					continue;
				}

				// may be more one program in one path
				for(unsigned int k = 0; k < pParser->packingListCount(); ++k)
				{
NEXT_PKL:
					const CPackingList* packing = pParser->packingList(k);
					if(!packing)
					{
						result = pParser->getLastError();
						break;
					}

					TProgramInfo programInfo;
// 					programInfo.type = "DCP";
					programInfo.rootpath = pathlist[i];
					std::vector<std::string> paths;
					for(unsigned int j=0; j<pParser->compositionCount(k); ++j)
					{
						const CCompositionPlayList* pcomtmp = pParser->composition(k, j);
						if(pcomtmp)
						{
							CAssistReel assistReel(*pcomtmp, *packing, pathlist[i]);
							CAssistComposition assistComp(pcomtmp);
#ifndef INDIA
							if(assistComp.GetKind()=="feature")
							{
#endif
								programInfo.id = assistComp.GetUUID().substr(strlen("urn:uuid:"));
								programInfo.title = assistComp.GetTitle();
								programInfo.contentType = assistComp.GetKind();
								programInfo.issuer = assistComp.GetIsssuer();
								programInfo.issueDate = assistComp.GetIssueDate();
								programInfo.creator = assistComp.GetCreator();
								programInfo.country = "";
								programInfo.language = assistComp.GetAudioInfo().strLanguage;
								programInfo.subtitle = "";
								long duration = assistComp.GetPlayDuration(true);
								char buf[20];
								sprintf(buf, "%ld.%03d", duration/1000, duration%1000);
								programInfo.duration = buf;
#ifndef INDIA
							}
							else
							{
								continue;
							}
#endif
							unsigned int reelNum = assistReel.GetReelCount();
							for(unsigned int i = 0; i < reelNum; ++i)
							{
								_reel_info info;
								if(!assistReel.GetReelInfo(i,info))
									break;

								TVideoInfo vinfo;
								TAudioInfo ainfo;

								vinfo.fileName = info.m_videoInfo.fileName;
								vinfo.entryPoint = info.m_videoInfo.entryPoint;
								vinfo.duration = info.m_videoInfo.duration;
								vinfo.keyIdStr = info.m_videoInfo.keyIdStr;
								vinfo.aspectRatio = info.m_videoInfo.aspectRatio.GetString();
								vinfo.frameRate = info.m_videoInfo.frameRate.GetString();
								programInfo.type = vinfo.type = info.m_videoInfo.videoFormat; //"MPEG2";
								//Add by Jaontolt
								//vinfo.iduration = info.m_videoInfo.iduration;
								//End
#if 0 //Disable by support JPEG200
								if (!(vinfo.type == "MPEG2" || vinfo.type == "MPEG2_3D"))
								{
									continue;
								}
#endif							
								char buf[20];
								ainfo.fileName = info.m_audioInfo.fileName;
								ainfo.entryPoint = info.m_audioInfo.entryPoint;
								ainfo.duration = info.m_audioInfo.duration;
								ainfo.keyIdStr = info.m_audioInfo.keyIdStr;
								ainfo.type = info.m_audioInfo.audioFormat;
								sprintf(buf, "%d", info.m_audioInfo.sampleRate);
								ainfo.sampleRate = buf;
								ainfo.bitIndeepness = "24";
								sprintf(buf, "%d", info.m_audioInfo.channelCount);
								ainfo.channel = buf;
								//Add by Jaontolt
								//ainfo.iduration = info.m_audioInfo.iduration;

								programInfo.videoInfo.push_back(vinfo);
								programInfo.audioInfo.push_back(ainfo);

								if(!vinfo.keyIdStr.empty())
									programInfo.license = "KEY";
							}

							// get paths of a composition
							assistReel.GetReelPaths(paths);
						}
					}

					if (programInfo.videoInfo.size() > 0)
					{
						paths.push_back(pParser->packingListFile(k));
						m_programFiles.push_back(paths);
						m_programInfoList.push_back(programInfo);
					}
				}

				ReleaseDcpParser(&pParser);
			}
		}
		DPRINT("CProgramQuery 'init m_programInfoList ... ok\n");
//		DPRINT((DP_Trace, "CProgramQuery", "'init m_programInfoList ... ok"));

		if(result!=no_error)
			DPRINT("CProgramQuery program parse error: %d.\n", result);
//			DPRINT((DP_Error, "CProgramQuery", "program parse error: %d.", result));

	}
	catch(const fs::filesystem_error& e)
	{
		//run on 8634, when last ++itr, always throw exception, so omit this exception.
		//return get_program_list_catch_exception_error;
		//cout << "CProgramQuery::getProgramList, except:" << e.what() << endl;
//		DPRINT((DP_Error, "CProgramQuery", "init unknown error: %s.", e.what()));
		DPRINT("CProgramQuery init unknown error: %s.\n", e.what());
	}
	
	return result;
}

