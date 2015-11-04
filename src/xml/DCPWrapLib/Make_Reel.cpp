// Make_Reel.cpp: implementation of the CMake_Reel class.
//
//////////////////////////////////////////////////////////////////////

#include "Make_Reel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMake_Reel::CMake_Reel(IXMLNode* pNode)
	: CElement(pNode)
{
}

CMake_Reel::~CMake_Reel()
{

}

/*
CMake_Asset CMake_Reel::CreateAsset(const char* szType,long lpos)
{
	CMake_Asset track(CreateChildNode("Asset",lpos,true));
	track.SetType(szType);
	return track;
}

CMake_Asset CMake_Reel::GetAssetByIndex(int nIndex)const
{
	return CMake_Asset(GetChildByNameIndex("Asset",nIndex));
}

CMake_Asset CMake_Reel::GetAssetByid(int id)const
{
	return CMake_Asset(GetChildById(id));
}

CMake_Asset CMake_Reel::GetAssetByTypeIndex(const char* szType,int nIndex)const
{
	long lChildCount = GetChildCount();
	int nItem = 0;
	for(int i=0;i<lChildCount;i++)
	{
		CMake_Asset track = GetAssetByIndex(i);
		std::string strtype = track.GetType();
		if(StrNoCaseCompare(strtype.c_str(),szType)==0)
		{
			if(nItem == nIndex)
				return track;
			nItem ++;
		}
	}
	return CMake_Asset();
}

int CMake_Reel::RemoveAssetByIndex(int nIndex)
{
	return RemoveChildNode(&(GetAssetByIndex(nIndex)));
	
}

int CMake_Reel::RemoveAssetByid(int id)
{
	return RemoveChildNode(&(GetAssetByid(id)));
}

int CMake_Reel::RemoveAssetByTypeIndex(const char* szType,int nIndex)
{
	return RemoveChildNode(&(GetAssetByTypeIndex(szType,nIndex)));
}

int CMake_Reel::RemoveAsset(CMake_Asset* pTrack)
{
	return RemoveChildNode(pTrack);
}
*/


CMake_Asset CMake_Reel::CreatePicture(long lpos)
{
	CMake_Asset track(CreateSingleChildNode(STR_MAIN_PICTURE,lpos,true));
	track.SetType(ASSETTYPE_PICTURE);
	return track;
	
}

CMake_Asset CMake_Reel::CreateSound(long lpos)
{
	CMake_Asset track(CreateSingleChildNode(STR_MAIN_SOUND,lpos,true));
	track.SetType(ASSETTYPE_SOUND);
	return track;
	
}

CMake_Asset CMake_Reel::CreateSubtitle(long lpos)
{
	CMake_Asset track(CreateSingleChildNode(STR_MAIN_SUBTITLE,lpos,true));
	track.SetType(ASSETTYPE_SUBTITLE);
	return track;
	
}

CMake_Asset CMake_Reel::CreateAuxSound(long lpos)
{
	CMake_Asset track(CreateChildNodeRept(STR_AUX_SOUND,lpos,true));
	track.SetType(ASSETTYPE_SOUND);
	return track;
	
}

CMake_Asset CMake_Reel::GetPicture()const
{
	CMake_Asset track(GetChildByNameIndexRept(STR_MAIN_PICTURE,0));
	return track;
	
}

CMake_Asset CMake_Reel::GetSound()const
{
	CMake_Asset track(GetChildByNameIndexRept(STR_MAIN_SOUND,0));
	return track;
	
}

CMake_Asset CMake_Reel::GetSubtitle()const
{
	CMake_Asset track(GetChildByNameIndexRept(STR_MAIN_SUBTITLE,0));
	return track;
	
}

CMake_Asset CMake_Reel::GetAuxSoundByIndex(int nIndex)const
{
	CMake_Asset track(GetChildByNameIndexRept(STR_AUX_SOUND,nIndex));
	return track;
}

CMake_Asset CMake_Reel::GetAuxSoundByid(int id)const
{
	CMake_Asset track(GetChildByIdRept(id));
	return track;
}

int CMake_Reel::RemovePicture()
{
	return RemoveChildByNameIndex(STR_MAIN_PICTURE);
//	CMake_Asset track = GetPicture();
//	return RemoveChildNode(&track);
	
}

int CMake_Reel::RemoveSound()
{
	return RemoveChildByNameIndex(STR_MAIN_SOUND);
//	CMake_Asset track = GetSound();
//	return RemoveChildNode(&track);
	
}

int CMake_Reel::RemoveSubtitle()
{
	return RemoveChildByNameIndex(STR_MAIN_SUBTITLE);
//	CMake_Asset track = GetSubtitle();
//	return RemoveChildNode(&track);
	
}

int CMake_Reel::RemoveAuxSound(CMake_Asset* maketrack)
{
	return RemoveChildNode(maketrack);
	
}

CMake_Asset CMake_Reel::CreateOtherAsset(const char* szName,long lpos)
{
	return CMake_Asset(CreateChildNodeRept(szName,lpos,true));
}

CMake_Asset CMake_Reel::GetOtherAssetByIndex(const char* szName,int nIndex)const
{
	return CMake_Asset(GetChildByNameIndexRept(szName,nIndex));
}

CMake_Asset CMake_Reel::GetOtherAssetByid(int id)const
{
	return CMake_Asset(GetChildByIdRept(id));
}

int CMake_Reel::RemoveOtherAsset(CMake_Asset* makeAsset)
{
	return RemoveChildNode(makeAsset);
}


