// Composition_AssetList.cpp: implementation of the CComposition_AssetList class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_AssetList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_AssetList::CComposition_AssetList(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_AssetList::~CComposition_AssetList()
{

}

CComposition_MarkerAsset CComposition_AssetList::CreateMarkerAsset(long lPos)
{
	return CComposition_MarkerAsset(CreateSingleChildNode("MainMarker",lPos,true));
	
}

CComposition_MarkerAsset CComposition_AssetList::GetMarkerAsset()const
{
	return CComposition_MarkerAsset(GetChildByNameIndexRept("MainMarker"));
	
}

int CComposition_AssetList::RemoveMarkerAsset()
{
	return RemoveChildByNameIndex("MainMarker");
//	CComposition_MarkerAsset markerAsset = GetMarkerAsset();
//	return RemoveChildNode(&markerAsset);
	
}


CComposition_PictureTrackFileAsset CComposition_AssetList::CreatePicture(long lPos)
{
	return CComposition_PictureTrackFileAsset(CreateSingleChildNode("MainPicture",lPos,true));
	
}

CComposition_PictureTrackFileAsset CComposition_AssetList::GetPicture()const
{
	IXMLNode* pChild = GetChildByNameIndexRept("MainPicture");
	if (pChild == NULL)
	{
		pChild = GetChildByNameIndexRept("msp-cpl:MainStereoscopicPicture");
	}
	if (pChild == NULL)
	{
		pChild = GetChildByNameIndexRept("MainStereoscopicPicture");
	}
	return CComposition_PictureTrackFileAsset(pChild);
}

int CComposition_AssetList::RemovePicture()
{
	return RemoveChildByNameIndex("MainPicture");
//	CComposition_PictureTrackFileAsset picture = GetPicture();
//	return RemoveChildNode(&picture);
	
}


CComposition_SoundTrackFileAsset CComposition_AssetList::CreateSound(long lPos)
{
	return CComposition_SoundTrackFileAsset(CreateSingleChildNode("MainSound",lPos,true));
	
}

CComposition_SoundTrackFileAsset CComposition_AssetList::GetSound()const
{
	return CComposition_SoundTrackFileAsset(GetChildByNameIndexRept("MainSound"));
	
}

int CComposition_AssetList::RemoveSound()
{
	return RemoveChildByNameIndex("MainSound");
//	CComposition_SoundTrackFileAsset sound = GetSound();
//	return RemoveChildNode(&sound);
	
}

CComposition_SubtitleTrackFileAsset CComposition_AssetList::CreateSubtitle(long lPos)
{
	return CComposition_SubtitleTrackFileAsset(CreateSingleChildNode("MainSubtitle",lPos,true));
	
}

CComposition_SubtitleTrackFileAsset CComposition_AssetList::GetSubtitle()const
{
	return CComposition_SubtitleTrackFileAsset(GetChildByNameIndexRept("MainSubtitle"));
	
}

int CComposition_AssetList::RemoveSubtitle()
{
	return RemoveChildByNameIndex("MainSubtitle");
//	CComposition_SubtitleTrackFileAsset subtitle = GetSubtitle();
//	return RemoveChildNode(&subtitle);
	
}

CComposition_AuxSoundTrackFileAsset CComposition_AssetList::CreateAuxiliarySound(long lPos)
{
	return CComposition_AuxSoundTrackFileAsset(CreateChildNodeRept("AuxiliarySound",lPos,true));
}

CComposition_AuxSoundTrackFileAsset CComposition_AssetList::GetAuxiliarySoundByIndex(int nIndex)const
{
	return CComposition_AuxSoundTrackFileAsset(GetChildByNameIndexRept("AuxiliarySound",nIndex));
}

CComposition_AuxSoundTrackFileAsset CComposition_AssetList::GetAuxiliarySoundByid(int id)const
{
	return CComposition_AuxSoundTrackFileAsset(GetChildByIdRept(id));
}

int CComposition_AssetList::RemoveAuxiliarySound(CComposition_AuxSoundTrackFileAsset* pAuxAudio)
{
	return RemoveChildNode(pAuxAudio);
}

int CComposition_AssetList::RemoveAuxiliarySoundByIndex(int nIndex)
{
	return RemoveChildByNameIndex("AuxiliarySound",nIndex);
//	CComposition_AuxSoundTrackFileAsset auxsound = GetAuxiliarySoundByIndex(nIndex);
//	return RemoveAuxiliarySound(&auxsound);
}

int CComposition_AssetList::RemoveAuxiliarySoundByid(int id)
{
	return RemoveChildById(id);
//	CComposition_AuxSoundTrackFileAsset auxsound = GetAuxiliarySoundByid(id);
//	return RemoveAuxiliarySound(&auxsound);
}

CComposition_GenericAsset CComposition_AssetList::CreateOtherAsset(const char* szName,long lPos)
{
	return CComposition_GenericAsset(CreateChildNodeRept(szName,lPos,true));
}

CComposition_GenericAsset CComposition_AssetList::GetOtherAssetByIndex(const char* szName,int nIndex)const
{
	return CComposition_GenericAsset(GetChildByNameIndexRept(szName,nIndex));
}

CComposition_GenericAsset CComposition_AssetList::GetOtherAssetByid(int id)const
{
	return CComposition_GenericAsset(GetChildByIdRept(id));
}

int CComposition_AssetList::RemoveOtherAsset(CComposition_GenericAsset* pOtherAsset)
{
	return RemoveChildNode(pOtherAsset);
}

int CComposition_AssetList::RemoveOtherAssetByIndex(const char* szName,int nIndex)
{
	return RemoveChildByNameIndex(szName,nIndex);
//	CComposition_GenericAsset otherasset = GetOtherAssetByIndex(szName,nIndex);
//	return RemoveOtherAsset(&otherasset);
}

int CComposition_AssetList::RemoveOtherAssetByid(int id)
{
	return RemoveChildById(id);
//	CComposition_GenericAsset otherasset = GetOtherAssetByid(id);
//	return RemoveOtherAsset(&otherasset);
}



