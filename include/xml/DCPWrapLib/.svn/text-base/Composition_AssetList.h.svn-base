// Composition_AssetList.h: interface for the CComposition_AssetList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_ASSETLIST_H_
#define __COMPOSITION_ASSETLIST_H_

#include "Composition_MarkerAsset.h"
#include "Composition_PictureTrackFileAsset.h"
//#include "Composition_SoundTrackFileAsset.h"
#include "Composition_AuxSoundTrackFileAsset.h"

class DCPWRAPLIB_API CComposition_AssetList : public CElement  
{
public:
	CComposition_AssetList(IXMLNode* pNode);
	virtual ~CComposition_AssetList();

public:
	CComposition_MarkerAsset CreateMarkerAsset(long lPos=0);
	CComposition_MarkerAsset GetMarkerAsset()const;
	int RemoveMarkerAsset();

	CComposition_PictureTrackFileAsset CreatePicture(long lPos=1);
	CComposition_PictureTrackFileAsset GetPicture()const;
	int RemovePicture();

	CComposition_SoundTrackFileAsset CreateSound(long lPos=2);
	CComposition_SoundTrackFileAsset GetSound()const;
	int RemoveSound();

	CComposition_SubtitleTrackFileAsset CreateSubtitle(long lPos=-1);
	CComposition_SubtitleTrackFileAsset GetSubtitle()const;
	int RemoveSubtitle();

	CComposition_AuxSoundTrackFileAsset CreateAuxiliarySound(long lPos=-1);
	CComposition_AuxSoundTrackFileAsset GetAuxiliarySoundByIndex(int nIndex=0)const;
	CComposition_AuxSoundTrackFileAsset GetAuxiliarySoundByid(int id)const;
	int RemoveAuxiliarySound(CComposition_AuxSoundTrackFileAsset* pAuxAudio);
	int RemoveAuxiliarySoundByIndex(int nIndex);
	int RemoveAuxiliarySoundByid(int id);

	CComposition_GenericAsset CreateOtherAsset(const char* szName,long lPos=-1);
	CComposition_GenericAsset GetOtherAssetByIndex(const char* szName,int nIndex=0)const;
	CComposition_GenericAsset GetOtherAssetByid(int id)const;
	int RemoveOtherAsset(CComposition_GenericAsset* pOtherAsset);
	int RemoveOtherAssetByIndex(const char* szName,int nIndex);
	int RemoveOtherAssetByid(int id);
};

#endif // __COMPOSITION_ASSETLIST_H_
