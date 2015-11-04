#include "v15Interface.h"
//#include "v15ProgramInterface.h"
#include "brunt/debugprint.h"
#include "v15ProgramManager.h"
#include "ProgramError.h"

#include <iostream>

using namespace std ;

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#if 0	// delete CV15ProgramInfo implement , liang 2009.7.14

#ifdef SIZEOF_ARY
#undef SIZEOF_ARY
#endif//SIZEOF_ARY

#define SIZEOF_ARY(a)	(sizeof(a)/sizeof(a[0]))

CV15ProgramInfo& getV15ProgramInfo()
{
	static CV15ProgramInfo  _v15ProgramInfo;
	return _v15ProgramInfo;
}


CV15ProgramInfo::CV15ProgramInfo()
{
	m_error = 0;
	m_nUpdateSource = 0;
	m_updateStatus = 0;

	memset(m_infoValidMark, false, sizeof(m_infoValidMark));

//	loadInfo(V15S_HDD);
//    loadInfo(USB);
}

CV15ProgramInfo::~CV15ProgramInfo()
{
	stop();
}

const char* CV15ProgramInfo::getProgramSrc(int src)
{/*
   	if(src == 0)
      	return "/localhd/program/movie";
   	if(src == 1)
		return "/localhd/program/publicad";
   	if(src == 2)
		return "/localhd/program/businessad";
   	if(src == 3)
      	return "/usb/DMS_HDD/MovieList";
   	if(src == 4)
		return "/usb/DMS_HDD/PadList";
   	if(src == 5)
		return "/usb/DMS_HDD/BadList";
   */
	if(src==0)
		return "hdisk";
	if(src==1)
		return "usb" ;
	return "";
}	   

void CV15ProgramInfo::loadInfo(int nSource)
{
	IProgramQuery* programQuery = createProgramQuery();
//	IProgramParse*  programParse = createProgramParse();

	DPRINT((DP_Trace, "<<<+++", "loadInfo(%d)", nSource));

	if(programQuery)
	{
		int j = nSource ;
		m_infoValidMark[j] = false;

		boost::mutex::scoped_lock lock(m_mutexInfoList);
		m_programInfoList[j].clear();

		if(programQuery->open(getProgramSrc(j)) != 0)
		{
			DPRINT((DP_Error, "CV15ProgramInfo", "programQuery->open(%s) failed." , getProgramSrc(j)));
			releaseProgramQuery(programQuery);
			return;
		}
		
		std::vector<std::string> programList;
		for(int i = 0; i < programQuery->getProgramCount(); ++i)
		{
			TProgramInfo info;
			if(programQuery->getProgramInfo(i, info)<0)
				continue;

			InfoData viewInfo;
			viewInfo.pData[0] = info.id;
			viewInfo.pData[1] = info.title;
			viewInfo.pData[2] = "Movie";
			viewInfo.pData[3] = "";
			viewInfo.pData[4] = "MPEG2";
			viewInfo.pData[5] = info.type;
			viewInfo.pData[6] = FormatTime(atoi(info.duration.c_str()));
			viewInfo.pData[7] = "1080i60";
			viewInfo.pData[8] = info.issuer;
			viewInfo.pData[9] = info.issueDate;
			viewInfo.pData[10] = "";
			viewInfo.pData[11] = info.country;
			viewInfo.pData[12] = "";
			viewInfo.pData[13] = info.language;
			viewInfo.pData[14] = info.rootpath;//programList[i];

			// license
			ILicenseManager* licmgr = getLicenseManager();
			ILicenseCtrl* licensectrl = licmgr->queryILicenseCtrl(info.id);
			if(licensectrl)
			{
				viewInfo.pData[3] = licensectrl->checkType()?(licensectrl->getType()==LST_KDM?"KDM":"DMS"):"U";
				licensectrl->Release();
			}

			m_programInfoList[j].push_back(viewInfo);
		}

		m_infoValidMark[j] = true;

		releaseProgramQuery(programQuery);
	}	
}

std::string CV15ProgramInfo::getProgramPath(const std::string& programId)
{
	std::string programPath;
	boost::mutex::scoped_lock lock(m_mutexInfoList);

	for(int j = 0; j < SIZEOF_ARY(m_programInfoList); ++j)
	{
		std::vector<InfoData>::iterator ite = m_programInfoList[j].begin();
		for(unsigned int i = 0; ite != m_programInfoList[j].end(); ++i, ++ite)
		{
			if(programId == ite->pData[0])
			{
				programPath = ite->pData[1];
				return programPath;
			}
		}
	}
	return "";
}

int CV15ProgramInfo::getStatus()
{
	return m_updateStatus;
}

int CV15ProgramInfo::update(const vector<int>& updateSrcList)	// start the thread
{
	if(isStopped())
	{
		cleanThread();
	}

	m_updateSrcList = updateSrcList;
	if(!start())
	{
		DPRINT((DP_Error, "CV15ProgramInfo", "Start thread failed, status = %d.", status()));
		return -1;
	}

	return 0;
}

int CV15ProgramInfo::getProgramFileList(int nSrc, std::vector<struct InfoData>& infoList)
{
	boost::mutex::scoped_lock lock(m_mutexInfoList);
	if(nSrc>=0 && nSrc<SIZEOF_ARY(m_programInfoList))
	{
		infoList = m_programInfoList[nSrc];
		return 0;
	}
	return -1;
}


void CV15ProgramInfo::init()
{
	m_updateStatus = UPDATING;
	return CActiveThread::init();
}

void CV15ProgramInfo::doit()
{
	for(int i=0; i<m_updateSrcList.size() && status()!=thread_stoping; i++)
	{
		loadInfo(m_updateSrcList[i]);
	}
}

void CV15ProgramInfo::release()
{
	m_updateStatus = UPDATEOK;
	return CActiveThread::release();
}

void CV15ProgramInfo::cleanThread()
{
	if(_impl->m_pThread)
	{
		_impl->m_cond.notify_all();
		_impl->m_pThread->join();
		delete _impl->m_pThread;
		_impl->m_pThread = NULL; 
		delete _impl->m_pAdapter;
		_impl->m_pAdapter = NULL;
	}
}

/*
bool CV15ProgramInfo::getProgramFilePath(const std::string& programId, std::vector<playFileItem>& playFiles)
{
	
	string path = getProgramPath(programId);
	if(path.empty())
		return false;
	std::vector<ReelInfo>  reelList;
	IProgramParse*  programParse = createProgramParse();
	if(programParse 
		&&programParse->init(path) == 0)
	{
		int j = 0;
		int programNum = programParse->getProgramNum();
		for(j = 0; j < programNum; ++j)
		{
			std::string Id;
			programParse->getProgramId(Id, j);
			if(Id == programId)
				break;
		}
		if(j < programNum && programParse->getProgramFilePath(reelList, j) == 0)
		{
			for(unsigned int i = 0; i < reelList.size(); ++i)
			{
				
				playFileItem item;
				item.vFileName = reelList[i].viedoInfo.fileName;
				item.aFileName = reelList[i].audioInfo.fileName;
				item.vEntryPoint =  reelList[i].viedoInfo.entryPoint;
				item.aEntryPoint =  reelList[i].audioInfo.entryPoint;
				item.vDur =  reelList[i].viedoInfo.dur;
				item.aDur =  reelList[i].audioInfo.dur;
				item.vkeyIdStr =  reelList[i].viedoInfo.keyIdStr;
				item.akeyIdStr =  reelList[i].audioInfo.keyIdStr;
				//		cout << item.vkeyIdStr << endl;		// liang test 2008.07.04
				//		cout << item.akeyIdStr << endl;		// liang test 2008.07.04
				playFiles.push_back(item);
			}
			releaseProgramParse(programParse);
			return (playFiles.size() > 0);
		}
		releaseProgramParse(programParse);
	}
	return false;
}
*/
/*PLAYER_TYPE CProgramInfo::getProgramType(const  std::string& programId)
{
	for(int j = 0; j < 2; ++j)
	{
		std::list<CProgramBaseInfo>::iterator ite = m_programInfoList[j].begin();
		for(; ite != m_programInfoList[j].end(); ++ite)
		{
			if(ite->id == programId)
	           {
				if(ite->type == "v8")
					return ECP_PLAYER;
				else if(ite->type == "es")
					return ES_PLAYER;
				else if(ite->type == "mxf" || ite->type == "DCP")
					return MXF_PLAYER;
	           }
			
		}
	}
	return ECP_PLAYER;
}
*/

/*
std::vector<InfoData>& CV15ProgramInfo::getProgramFileList(int nSrc ,int type)
{
		string sType ;
		if(type==0){
			sType = "Movie" ;
		}else if(type==1){
			sType = "BusinessAd" ;
		}else{
			sType = "PublicAd" ;
		}
		if(m_programInfoList[nSrc].size()<=0){
			return m_programInfoList[nSrc] ;
		}
		std::vector<InfoData>::iterator ite = m_programInfoList[nSrc].begin();
		for(unsigned int i = 0; ite != m_programInfoList[nSrc].end(); ++i, ++ite)
		{
			InfoData ptmp = *ite ;
			if((ptmp.pData[2].compare(sType))){
				m_programInfoList[nSrc].erase(ite);
				if(ite == m_programInfoList[nSrc].end()){
					break ;
				}
			}
		}
		return m_programInfoList[nSrc];
}		
*/
#endif	// delete CV15ProgramInfo implement , liang 2009.7.14

///////////////////////////////////////////////////////////////////////////////
//
//
//
#if 0
int v15ProgramUpdate(const vector<int> srcList)
{
	return getV15ProgramManager().update(srcList);
}

bool v15ProgramIsReady(int nSrc)
{
	return getV15ProgramManager().isReady(nSrc);
}

int v15GetProgramList(StorageSource nSrc, ProgramType type, std::vector<InfoData>& ProgramList)
{
//	DPRINT((DP_Trace, ">>>>>>>>>>>>>>>>", "v15GetProgramList." ));
	return getV15ProgramManager().getProgramFileList(nSrc, ProgramList);
}
#endif

std::string v15FormatErrorMsg(int error)
{
	switch(error)
	{
// manager
	case get_program_parse_object_error:
		return "get_program_parse_object_error";
	case get_program_import_object_error:
		return "get_program_import_object_error";
	case exist_same_program_name:
		return "get_program_with_same_uuid_error";//相同id号的节目已经存在";
//parse
	case create_ini_object_error:
		return "create_ini_object_error";
	case load_describe_file_error:
		return "load_describe_file_error";
	case could_not_find_video_reel_file_error:
		return "could_not_find_video_reel_file_error";
	case get_programid_xml_exception:
		return "get_programid_xml_exception";
	case get_programid_unknown_exception:
		return "get_programid_unknown_exception";
	case get_program_id_error:
		return "get_program_id_error";
	case get_programInfo_xml_exception:
		return "get_programInfo_xml_exception";
	case get_programInfo_unknown_exception:
		return "get_programInfo_unknown_exception";
	case get_programInof_error:
		return "get_programInof_error";
	case get_programReel_xml_exception:
		return "get_programReel_xml_exception";
	case get_programReel_unknown_exception:
		return "get_programReel_unknown_exception";
	case could_not_find_reel_error:
		return "could_not_find_reel_error";
	case get_programHash_xml_exception:
		return "get_programHash_xml_exception";
	case get_programHash_unknown_exception:
		return "get_programHash_unknown_exception";
//query
	case init_root_path_error:
		return "init_root_path_error";
	case get_program_path_error:
		return "get_program_path_error";
	case not_find_program_path:
		return "not_find_program_path";
	case get_program_list_catch_exception_error:
		return "get_program_list_catch_exception_error";
//import
	case program_import_root_error:
		return "program_import_root_error";
	case invalid_program_error:
		return "invalid_program_error";
	case could_not_find_file_error:
		return "could_not_find_file_error";
	case not_enough_space:
		return "not_enough_space";
	case program_import_error:
		return "program_import_error";
	case stop_import_error:
		return "stop_import_error";
	case get_program_parse_error:
		return "get_program_parse_error";
	case get_program_query_error:
		return "get_program_query_error";
	case run_thread_manager_error:
		return "run_thread_manager_error";
//hash
	case hash_file_error:
		return "hash_file_error";
	case could_not_find_hash_file_error:
		return "could_not_find_hash_file_error";
	case file_was_destroyed_error:
		return "file_was_destroyed_error";
	case no_hash_verify_result_error:
		return "no_hash_verify_result_error";
	case get_hash_result_error:
		return "get_hash_result_error";
//del
	case deleteProgram_error:
		return "deleteProgram_error";
	}
	return "";
}



