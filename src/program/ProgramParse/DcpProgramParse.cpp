#include <DcpParser/DcpParser.h>
#include <DcpParser/AssistComposition.h>
#include <DcpParser/AssistReel.h>
#include <DcpParser/AssistHash.h>
#include <exception>
#include "DcpProgramParse.h"
#include <program/ProgramError.h>
#include <iostream>
#include <string>
using namespace std;


CDcpProgramParse::CDcpProgramParse()
{
	m_pParser = CreateDcpParser();
}

CDcpProgramParse::~CDcpProgramParse()
{
	ReleaseDcpParser(&m_pParser);
}

int CDcpProgramParse::init(const std::string& programPath)
{
	
	if(m_pParser->open(programPath))
	{
		m_programPath = programPath;
		return no_error;
	}
	m_programPath = "";
	return load_describe_file_error;	
}

int CDcpProgramParse::getProgramNum()
{
	return m_pParser->compositionCount(0);
}
	
int CDcpProgramParse::getProgramId(std::string& programId, int index)
{
	try
	{
		const CCompositionPlayList* pcomtmp = m_pParser->composition(index);
		if(pcomtmp)
		{
			programId = pcomtmp->GetUUIDElement();
			programId = programId.substr(strlen("urn:uuid:"));
			return no_error;
		}
	}
	catch (std::exception& e) {
		cout <<"CDcpProgramParse::getProgramId, catch exception: " << e.what() << endl;
		return get_programid_xml_exception;
		
	}
	catch (...) {
		cout <<"CDcpProgramParse::getProgramId, catch unkown exception: " << endl;
		return get_programid_unknown_exception;
	}
	return get_program_id_error;
}

int CDcpProgramParse::parseProgramInfo(ProgramInfo& info, int index)
{
//	cout << "CDcpProgramParse::parseProgramInfo()" << endl;
	try
	{
		const CCompositionPlayList* pcomtmp = m_pParser->composition(index);
		if(pcomtmp)
		{
				CAssitComposition  assit(pcomtmp);
				info.type = "DCP";
				info.id = pcomtmp->GetUUIDElement();
				info.id = info.id.substr(strlen("urn:uuid:"));
				info.title = assit.GetTitle();
				info.contentType = assit.GetKind();
				info.issueDate = assit.GetIssueDate();
				info.issuer = assit.GetIsssuer();
				info.creator = assit.GetCreator();
				info.country = "";
				_audio_info ainfo = assit.GetAudioInfo();
				info.language = ainfo.strLanguage;
				info.subtitle = "";
				info.videoInfo.type = "MPEG2";
				_video_info vinfo = assit.GetVideoInfo();
				info.videoInfo.aspectRatio =vinfo.nAspectRatio.GetString();
				info.videoInfo.frameRate = vinfo.nFrameRate.GetString();
				char buf[20];
				sprintf(buf, "%ld",assit.GetPlayDuration()/24);
				info.videoInfo.duration = buf;
				info.audioInfo.type = "PCM";
				info.audioInfo.sampleRate = "48000";
				info.audioInfo.bitIndeepness = "24";
				info.audioInfo.channel = "6";
				info.duration = info.videoInfo.duration;
//			cout << "CAssitComposition end ok."<< endl;
				return no_error;
		}
	}
	catch (std::exception& e) {
		cout <<"CDcpProgramParse::parseProgramInfo, catch exception ?? : " << e.what() << endl;
		return get_programInfo_xml_exception;
		
	}
	catch (...) {
		cout <<"CDcpProgramParse::parseProgramInfo, catch unkown exception: " << endl;
		return get_programInfo_unknown_exception;
	}
	
	return get_programInof_error;
}

int CDcpProgramParse::getProgramFilePath(std::vector<ReelInfo>& filePathList, int index)
{
	try
	{
		const CCompositionPlayList* pcomtmp = m_pParser->composition(index);
		if(pcomtmp)
		{
			const CPackingList& list = m_pParser->packingList();
		
			CAssistReel assistReel(*pcomtmp, m_pParser->packingList(),m_programPath);
			unsigned int reelNum = assistReel.GetReelCount();
			
			for(unsigned int i = 0; i < reelNum; ++i)
			{
				_reel_info info;
				if(!assistReel.GetReelInfo(i,info))
					break;
				ReelInfo mediaFile;
				mediaFile.viedoInfo.fileName = info.m_videoInfo.fileName;
				mediaFile.viedoInfo.dur = info.m_videoInfo.dur;
				mediaFile.viedoInfo.entryPoint = info.m_videoInfo.entryPoint;
				mediaFile.viedoInfo.keyIdStr = info.m_videoInfo.keyIdStr;		
				
				mediaFile.audioInfo.fileName = info.m_audioInfo.fileName;
				mediaFile.audioInfo.dur = info.m_audioInfo.dur;
				mediaFile.audioInfo.entryPoint = info.m_audioInfo.entryPoint;
				mediaFile.audioInfo.keyIdStr = info.m_audioInfo.keyIdStr;		
				filePathList.push_back(mediaFile);
			}
			if(filePathList.size() == reelNum)
				return no_error;
		}
	}
	catch (std::exception& e) {
		cout <<"CDcpProgramParse::getProgramFilePath, catch exception: " << e.what() << endl;
		return get_programReel_xml_exception;
		
	}
	catch (...) {
		cout <<"CDcpProgramParse::getProgramFilePath, catch unkown exception: " << endl;
		return get_programReel_unknown_exception;
	}
	
	return could_not_find_reel_error;
}

int CDcpProgramParse::getHashValue(std::vector<HashInfo>& hashInfo, int index)
{
	try
	{
		CAssistHash assistHash(m_pParser->packingList(),m_programPath);
		std::vector<_hash_file_info> hashFiles;
		assistHash.GetHashInfo(hashFiles);
		for(unsigned int i = 0; i < hashFiles.size(); ++i)
		{
			HashInfo info;
			info.fileName = hashFiles[i].fileName;
			info.hashValue = hashFiles[i].hashValue;
			hashInfo.push_back(info);	
		}
	}
	catch (std::exception& e) 
	{
		cout <<"CDcpProgramParse::getHashValue, catch exception: " << e.what() << endl;
		return get_programHash_xml_exception;
		
	}
	catch (...) 
	{
		cout <<"CDcpProgramParse::getHashValue, catch unkown exception: " << endl;
		return get_programHash_unknown_exception;
	}
	return no_error;
}

int CDcpProgramParse::getLastError()
{
	return no_error;
}
