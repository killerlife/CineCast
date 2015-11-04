#include "IniProgramParse.h"
#include <ProgramError.h>
#include <iostream>
using namespace brunt;
using namespace std;

CIniProgramParse::CIniProgramParse() : m_myIni(NULL)
{
	m_myIni = createMyini();
}

CIniProgramParse::~CIniProgramParse()
{
	if(m_myIni != NULL)
		releaseMyini(m_myIni);
	m_programPath.clear();
}

int CIniProgramParse::init(const std::string& programPath)
{
	if(m_myIni == NULL)
		return create_ini_object_error;
    if(programPath.empty())
    	return load_describe_file_error;
	if(programPath[programPath.length() - 1] != '/' && programPath[programPath.length() - 1] != '\\')
		m_programPath = programPath + "/";
	else 
		m_programPath = programPath;

	if(!m_myIni->load(m_programPath + "describe.ini"))
		return load_describe_file_error;
	return no_error;
}

int CIniProgramParse::getProgramId(std::string& programId, int index)
{
	if(!m_myIni->read("baseinfo", "id", programId))
		return get_program_id_error;

	return no_error;
}

int CIniProgramParse::parseProgramInfo(ProgramInfo& info, int index)
{
	m_myIni->read("baseinfo", "type", info.type);
	m_myIni->read("baseinfo", "id", info.id);
	m_myIni->read("baseinfo", "title", info.title);
	m_myIni->read("baseinfo", "contentType", info.contentType);
	m_myIni->read("baseinfo", "issueDate", info.issueDate);
	m_myIni->read("baseinfo", "issuer ", info.issuer);
	m_myIni->read("baseinfo", "creator", info.creator);
	m_myIni->read("baseinfo", "country", info.country);
	m_myIni->read("baseinfo", "language", info.language);
	m_myIni->read("baseinfo", "subtitle", info.subtitle);

	m_myIni->read("video", "type", info.videoInfo.type);
	m_myIni->read("video", "frameRate", info.videoInfo.frameRate);
	m_myIni->read("video", "aspectRatio", info.videoInfo.aspectRatio);
	m_myIni->read("video", "duration", info.videoInfo.duration);

	m_myIni->read("audio", "type", info.audioInfo.type);
	m_myIni->read("audio", "sampleRate", info.audioInfo.sampleRate);
	m_myIni->read("audio", "bitIndeepness", info.audioInfo.bitIndeepness);
	m_myIni->read("audio", "channel", info.audioInfo.channel);
	info.duration = info.videoInfo.duration;
	return no_error;
}

int CIniProgramParse::getProgramFilePath(std::vector<ReelInfo>& filePathList, int index)
{
	int reelNum = 0;
	if(!readIni(*m_myIni, "reel", "reelNum", reelNum) || reelNum <= 0)
		return could_not_find_reel_error;
	for(int i = 1; i <= reelNum; i++)
	{
		char reelVideo[16];
		char reelAudio[16];
		memset(reelVideo, 0, sizeof(reelVideo));
		memset(reelAudio, 0, sizeof(reelAudio));
		sprintf(reelVideo, "reel_%d_video", i);
		sprintf(reelAudio, "reel_%d_audio", i);
		std::string videoName;
		std::string audioName;
		ReelInfo fileInfo;
		if(m_myIni->read("reel", reelVideo, videoName) && !videoName.empty())
		{
			fileInfo.viedoInfo.fileName = m_programPath + videoName;
		}
		else
			return could_not_find_video_reel_file_error;
		if(m_myIni->read("reel", reelAudio, audioName) && !audioName.empty())
		{
			fileInfo.audioInfo.fileName = m_programPath + audioName;
		}
		else
			return could_not_find_audio_reel_file_error;
		filePathList.push_back(fileInfo);
	}
	return no_error;
}

int CIniProgramParse::getHashValue(std::vector<HashInfo>& hashInfo, int index)
{
	int hashNum = 0;
	if(!readIni(*m_myIni, "hash", "fileNum", hashNum) || hashNum <= 0)
		return no_error;

	for(int i = 1; i <= hashNum; i++)
	{
		char file[16];
		char hash[16];
		memset(file, 0, sizeof(file));
		memset(hash, 0, sizeof(hash));
		sprintf(file, "file_%d", i);
		sprintf(hash, "hash_%d", i);
		HashInfo info;
		if(m_myIni->read("hash", file, info.fileName)
		   && !info.fileName.empty()
		   && m_myIni->read("hash", hash, info.hashValue)
		   && !info.hashValue.empty())
		{
			info.fileName = m_programPath + info.fileName;
			hashInfo.push_back(info);
		}
	}
	return no_error;
}

int CIniProgramParse::getLastError()
{
	return no_error;
}
