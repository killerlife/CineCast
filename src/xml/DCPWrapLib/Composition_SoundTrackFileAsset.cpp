// Composition_SoundTrackFileAsset.cpp: implementation of the CComposition_SoundTrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_SoundTrackFileAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_SoundTrackFileAsset::CComposition_SoundTrackFileAsset(IXMLNode* pNode)
	: CComposition_TrackFileAsset(pNode)
{
}

CComposition_SoundTrackFileAsset::~CComposition_SoundTrackFileAsset()
{

}

bool CComposition_SoundTrackFileAsset::SetLanguageElement(const char *szval,long lPos)
{
	return SetChildElementText("Language",szval,lPos);
	
}

std::string CComposition_SoundTrackFileAsset::GetLanguageElement() const
{
	return GetChildElementText("Language");
	
}

//////////////////////////////////////////////////////////////////////////
//composition_subtitletrackfileasset.cpp

CComposition_SubtitleTrackFileAsset::CComposition_SubtitleTrackFileAsset(IXMLNode* pNode)
	: CComposition_TrackFileAsset(pNode)
{
}

CComposition_SubtitleTrackFileAsset::~CComposition_SubtitleTrackFileAsset()
{
	
}

bool CComposition_SubtitleTrackFileAsset::SetLanguageElement(const char *szval,long lPos)
{
	return SetChildElementText("Language",szval,lPos);
	
}

std::string CComposition_SubtitleTrackFileAsset::GetLanguageElement() const
{
	return GetChildElementText("Language");
	
}

