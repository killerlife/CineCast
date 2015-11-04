// Composition_SoundTrackFileAsset.h: interface for the CComposition_SoundTrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_SOUNDTRACKFILEASSET_H_
#define __COMPOSITION_SOUNDTRACKFILEASSET_H_

#include "Composition_TrackFileAsset.h"

class DCPWRAPLIB_API CComposition_SoundTrackFileAsset : public CComposition_TrackFileAsset  
{
public:
	CComposition_SoundTrackFileAsset(IXMLNode* pNode);
	virtual ~CComposition_SoundTrackFileAsset();
public:
	std::string GetLanguageElement()const;
	bool SetLanguageElement(const char* szval,long lPos=-1);
};

class DCPWRAPLIB_API CComposition_SubtitleTrackFileAsset : public CComposition_TrackFileAsset  
{
public:
	CComposition_SubtitleTrackFileAsset(IXMLNode* pNode);
	virtual ~CComposition_SubtitleTrackFileAsset();
public:
	std::string GetLanguageElement()const;
	bool SetLanguageElement(const char* szval,long lPos=-1);
	
};

#endif // __COMPOSITION_SOUNDTRACKFILEASSET_H_
