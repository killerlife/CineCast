#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AssistReel.h"
#include "assetMap.h"
#include <iostream>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
namespace fs = boost::filesystem;

CAssistReel::CAssistReel(const CCompositionPlayList& cpl, const CPackingList& packingList, const std::string& path):m_cpl(cpl),m_packingList(packingList)//,m_basePath(path +"/")
{
	m_basePath = path;
#ifdef _WIN32
	m_basePath += "\\";
#else
	m_basePath += "/";
#endif
}

CAssistReel::~CAssistReel()
{
};
	
int CAssistReel::GetReelCount()const
{
	CComposition_ReelList reelList = m_cpl.GetReelList();
	return reelList.GetChildCountByName("Reel");
}
std::string getFileSrc(CComposition_TrackFileAsset& track , const CPackingList& packing)
{
	try
	{
		string uuid = track.GetUUIDElement();
		CPack_AssetList assetList = packing.GetAssetList();
		for(int i = 0;i < assetList.GetChildCount(); ++i)
		{
			CPack_Asset asset = assetList.GetAssetByIndex(i);
			if(asset.GetUUIDElement() == uuid)
			{
				return asset.GetOriginalFileNameElement().strValue;
			}
		}
	}
	catch(std::exception&)
	{
	  
	}
	return "";
}

std::string CAssistReel::getFilePath(CComposition_TrackFileAsset& track)const
{
	std::string file = m_basePath + getFileSrc(track, m_packingList);
	if(file == m_basePath)
	{
		CTaAssetMap assertMap;
		if(assertMap.open(m_basePath))
		{
			file = assertMap.findFile(track.GetUUIDElement());
			assertMap.close();
		}
		else
		{
			cout << "CAssistReel: AssetMap open failed." << endl;
		}
		if(file.empty())
		{
			cout << "CAssistReel: AssetMap file empty." << endl;
			return "";
		}
		file = m_basePath + file;	//m_basePath + "/" + file;  liang modified 2008.07.03		
	}

	fs::path thepath(file);

	return thepath.native();
//	info.fileName = file;
//	info.entryPoint = track.GetEntryPointElement();
//	info.dur = track.GetDurationElement();
//	info.keyIdStr = track.GetKeyIdElement();
//	return true;
}

#include "mxf/mxfEssenceReader/IMetaDataParse.h"

bool CAssistReel::GetReelInfo(int index, _reel_info& info)const
{
	CComposition_ReelList reelList = m_cpl.GetReelList();
	int reelNum = reelList.GetChildCountByName("Reel");
	if(reelNum <= index)
		return false;

	CComposition_Reel reel = reelList.GetReelByIndex(index);
	CComposition_AssetList assetList = reel.GetAssetList();
	CComposition_PictureTrackFileAsset vTrack = assetList.GetPicture();
	CComposition_SoundTrackFileAsset   aTrack = assetList.GetSound();
	ITaMxfMetaDataParse*  mxfParse = createMetaDataParse();

	info.m_reelIndex = index;
	info.m_videoInfo.fileName = getFilePath(vTrack);
	info.m_videoInfo.entryPoint = vTrack.GetEntryPointElement();
	info.m_videoInfo.duration = vTrack.GetDurationElement();
	info.m_videoInfo.keyIdStr = vTrack.GetKeyIdElement();
	info.m_videoInfo.aspectRatio = vTrack.GetScreenAspectRatioElement();
	info.m_videoInfo.frameRate = vTrack.GetFrameRateElement();
	mxfParse->parse(info.m_videoInfo.fileName.c_str());
	info.m_videoInfo.videoFormat = mxfParse->essenceType();

	if (!aTrack.isNull())
	{
		info.m_audioInfo.fileName = getFilePath(aTrack);
		info.m_audioInfo.entryPoint = aTrack.GetEntryPointElement();
		info.m_audioInfo.duration = aTrack.GetDurationElement();
		info.m_audioInfo.keyIdStr = aTrack.GetKeyIdElement();
		info.m_audioInfo.language = aTrack.GetLanguageElement();
		mxfParse->parse(info.m_audioInfo.fileName.c_str());
		info.m_audioInfo.audioFormat = mxfParse->essenceType();
		info.m_audioInfo.channelCount = mxfParse->audioProperty().Channels;
		info.m_audioInfo.sampleRate = mxfParse->audioProperty().SampleRate.Numerator / 
			mxfParse->audioProperty().SampleRate.Denominator;
	}

	releaseMetaDataParse(mxfParse);
	return true;
}

bool CAssistReel::GetReelPaths(std::vector<std::string>& paths)
{
	CPack_AssetList assetList = m_packingList.GetAssetList();

	bool bNeedAssetmap = false;
	CTaAssetMap assetMap;
	std::string file;

	for(long i=0; i<assetList.GetChildCount(); i++)
	{
		CPack_Asset asset = assetList.GetAssetByIndex(i);
		file = asset.GetOriginalFileNameElement().strValue;
		if(file.empty())
		{
			if(assetMap.open(m_basePath))
			{
				file = assetMap.findFile(asset.GetUUIDElement());
				assetMap.close();
			}
			bNeedAssetmap = true;
		}

		if(!file.empty())
		{
			file = m_basePath + file;
			paths.push_back(file);
		}
	}

	if(bNeedAssetmap)
	{
		file = m_basePath + assetMap.getAssetMap();
		paths.push_back(file);
	}

	return true;
}
