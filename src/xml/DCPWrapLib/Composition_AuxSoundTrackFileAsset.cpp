// Composition_AuxSoundTrackFileAsset.cpp: implementation of the CComposition_AuxSoundTrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_AuxSoundTrackFileAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_AuxSoundTrackFileAsset::CComposition_AuxSoundTrackFileAsset(IXMLNode* pNode)
	: CComposition_SoundTrackFileAsset(pNode)
{

}

CComposition_AuxSoundTrackFileAsset::~CComposition_AuxSoundTrackFileAsset()
{

}

bool CComposition_AuxSoundTrackFileAsset::SetTrackIdElement(const char *szval, long lpos)
{
	return SetChildElementText("TrackId",szval,lpos);
	
}


std::string CComposition_AuxSoundTrackFileAsset::GetTrackIdElement()const
{
	return GetChildElementText("TrackId");
	
}

