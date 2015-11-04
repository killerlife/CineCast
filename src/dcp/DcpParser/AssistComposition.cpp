//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#include "AssitComposition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DcpParser.h"

#include "errordefine.h"
#include "AssistComposition.h"
#include <stdexcept>


CAssistComposition::CAssistComposition(const CCompositionPlayList* pcomp)
{
	m_pComp = (CCompositionPlayList*)pcomp;
	if(!m_pComp)
		throw std::runtime_error("composition playlist pointer is NULL.");
}

CAssistComposition::~CAssistComposition()
{
}


std::string CAssistComposition::GetUUID() const
{
	return m_pComp->GetUUIDElement();
}

std::string CAssistComposition::GetTitle()const
{
	return m_pComp->GetContentTitleTextElement().strValue;
}

std::string CAssistComposition::GetKind()const
{
	return m_pComp->GetContentKindElement().strValue;
}

std::string CAssistComposition::GetVersionDescriptor()const
{
	try
	{
		CComposition_ContentVersion ver = m_pComp->GetContentVersion();
		return ver.GetLabelTextElement().strValue;
	}
	catch (std::exception&)
	{
		
	}
	return "";
	
}

int CAssistComposition::GetRating(std::vector<_rating>& arr_rat)
{
	try{
		CComposition_RatingList ratinglist = m_pComp->GetRatingList();
		int nCount  = ratinglist.GetChildCountByName("Rating");
		_rating ratItem;
		for(int i=0;i<nCount;i++)
		{
			CComposition_Rating rat = ratinglist.GetRatingByIndex(i);
			ratItem.strAgency = rat.GetAgencyElement();
			ratItem.strLable = rat.GetLabelElement();
			arr_rat.push_back(ratItem);
		}
		return nCount;
	}
	catch(...)
	{
	}
	return ERR_ASSIT_GETRATING_FAILED;
}

std::string CAssistComposition::GetCreator()const
{
	return m_pComp->GetIssuer().GetCreatorElement().strValue;
}

std::string CAssistComposition::GetIsssuer()const
{
	return m_pComp->GetIssuer().GetIssuerElement().strValue;
}

std::string  CAssistComposition::GetIssueDate()const
{
	return m_pComp->GetIssuer().GetIssueDateElement().GetString();
}

int CAssistComposition::GetReelCount()const
{
	try{
	CComposition_ReelList reelList = m_pComp->GetReelList();
	return reelList.GetChildCountByName("Reel");
	}
	catch (std::exception&)
	{

	}
	return 0;
}

long CAssistComposition::GetPlayDuration(bool second)const
{
	try{
		CComposition_ReelList reelList = m_pComp->GetReelList();
		int nCount = reelList.GetChildCountByName("Reel");
		long lTmpDur,lDuration = 0;
		for(int i=0;i<nCount;i++)
		{
			CComposition_Reel reel = reelList.GetReelByIndex(i);
			lTmpDur = GetReelDuration(reel, second);
			if(lTmpDur < 0)
				return lTmpDur;
			lDuration += lTmpDur;
		}
		return lDuration;
	}
	catch (std::exception&)
	{

	}
	return 0;
}

RATIONAL CAssistComposition::GetEditRate()const
{
	CComposition_AssetList* assetList=NULL;
	try
	{
		CComposition_ReelList reelList = m_pComp->GetReelList();
		CComposition_Reel reel = reelList.GetReelByIndex(0);
		assetList = new CComposition_AssetList(reel.GetAssetList());
	}
	catch (std::exception&)
	{
	}
	if(assetList)
	{
		try
		{
			CComposition_PictureTrackFileAsset picture = assetList->GetPicture();
			delete assetList;
			return picture.GetEditRateElement();
		}
		catch(...)
		{
		}

		//long lDuration = 0;
		try
		{
			CComposition_SoundTrackFileAsset sound = assetList->GetSound();
			delete assetList;
			return sound.GetEditRateElement();
		}
		catch(...)
		{
		}

		try
		{
			CComposition_SubtitleTrackFileAsset subtitle = assetList->GetSubtitle();
			delete assetList;
			return subtitle.GetEditRateElement();
		}
		catch(...)
		{
		}
		delete assetList;
	}
	
	return (long)0;
}


_video_info CAssistComposition::GetVideoInfo()const
{
	try
	{
		CComposition_ReelList reelList = m_pComp->GetReelList();
		CComposition_Reel reel = reelList.GetReelByIndex(0);
		CComposition_AssetList assetList = reel.GetAssetList();
		CComposition_PictureTrackFileAsset picture = assetList.GetPicture();
		_video_info videoInfo;
		videoInfo.nAspectRatio = picture.GetScreenAspectRatioElement();
		videoInfo.nFrameRate = picture.GetFrameRateElement();
		return videoInfo;

	}
	catch(...)
	{
	}
	return _video_info();
}

_audio_info CAssistComposition::GetAudioInfo()const
{
	try
	{
		CComposition_ReelList reelList = m_pComp->GetReelList();
		CComposition_Reel reel = reelList.GetReelByIndex(0);
		CComposition_AssetList assetList = reel.GetAssetList();
		CComposition_SoundTrackFileAsset sound = assetList.GetSound();
		_audio_info audioInfo;
		audioInfo.strLanguage = sound.GetLanguageElement();
		return audioInfo;

	}
	catch(...)
	{
	}
	return _audio_info();
}

_subtitle_info CAssistComposition::GetSubtitleInfo()const
{
	try
	{
		CComposition_ReelList reelList = m_pComp->GetReelList();
		CComposition_Reel reel = reelList.GetReelByIndex(0);
		CComposition_AssetList assetList = reel.GetAssetList();
		CComposition_SubtitleTrackFileAsset subtitle = assetList.GetSubtitle();
		_subtitle_info subtitleInfo;
		subtitleInfo.strLanguage = subtitle.GetLanguageElement();
		return subtitleInfo;

	}
	catch(...)
	{
	}
	return _subtitle_info();
}



long CAssistComposition::GetReelDuration(const CComposition_Reel& reel, bool second)const
{
	CComposition_AssetList *assetList = NULL;
	try
	{
		assetList = new CComposition_AssetList(reel.GetAssetList());
	}
	catch (std::exception&)
	{
		if (assetList)
		{
			delete assetList;
			assetList = NULL;
		}
	}
	if(assetList)
	{
		try
		{
			CComposition_PictureTrackFileAsset picture = assetList->GetPicture();
			long ldur = picture.GetDurationElement();
			if(ldur <= 0)
				ldur = picture.GetIntrinsicDurationElement();
			if(second)
			{
				ldur *= picture.GetEditRateElement().lDenominator;
				ldur *= 1000;
				ldur /= picture.GetEditRateElement().lNmerator;
			}
			delete assetList;
			return ldur;
		}
		catch(...)
		{
		}

		long lDuration = 0;
		try
		{
			CComposition_SoundTrackFileAsset sound = assetList->GetSound();
			lDuration = sound.GetDurationElement();
			if(lDuration <= 0)
				lDuration = sound.GetIntrinsicDurationElement();
		}
		catch(...)
		{
		}

		try
		{
			CComposition_SubtitleTrackFileAsset subtitle = assetList->GetSubtitle();
			long lTmpDur = subtitle.GetDurationElement();
			if(lTmpDur <= 0)
				lTmpDur = subtitle.GetIntrinsicDurationElement();
			if(lDuration > lTmpDur)
				lDuration = lTmpDur;
		}
		catch(...)
		{
		}
		delete assetList;
		return lDuration;
	}
	return 0;

}
/*

*/
