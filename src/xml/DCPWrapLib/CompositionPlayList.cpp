// CompositionPlayList.cpp: implementation of the CCompositionPlayList class.
//
//////////////////////////////////////////////////////////////////////

#include "CompositionPlayList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompositionPlayList::CCompositionPlayList()
:CDCIProjectElement("CompositionPlaylist",NULL,CREATE_FLAG)
{
}

CCompositionPlayList::CCompositionPlayList(const char* szprefix,const char* sznamespace)
:CDCIProjectElement(std::string(szprefix)+std::string(":CompositionPlaylist"),sznamespace,CREATE_FLAG)
{

}

CCompositionPlayList::CCompositionPlayList(const char* szInfo,SOURCEFLAG nFlag)
:CDCIProjectElement("CompositionPlaylist",szInfo,nFlag)
{

}

CCompositionPlayList::~CCompositionPlayList()
{

}

bool CCompositionPlayList::SetIconUUIDElement(const char *szval,long lPos)
{
	return SetChildElementText("IconId",szval,lPos);
	
}

std::string CCompositionPlayList::GetIconUUIDElement()const
{
	return GetChildElementText("IconId");
	
}

bool CCompositionPlayList::SetContentTitleTextElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("ContentTitleText",szval,lPos);
	
}

USERTEXT CCompositionPlayList::GetContentTitleTextElement() const
{
	return GetChildElementTextUsertext("ContentTitleText");
	
}

bool CCompositionPlayList::SetContentKindElement(const SCOPEATTR& szval,long lPos)
{
	return SetChildElementText("ContentKind",szval,lPos);
	
}

SCOPEATTR CCompositionPlayList::GetContentKindElement() const
{
	return GetChildElementTextScopetext("ContentKind");
	
}

CComposition_ContentVersion CCompositionPlayList::CreateContentVersion(long lPos)
{
	return CComposition_ContentVersion(CreateSingleChildNode("ContentVersion",lPos));
	
}

CComposition_ContentVersion CCompositionPlayList::GetContentVersion() const
{
	return CComposition_ContentVersion(GetChildByNameIndexRept("ContentVersion"));
	
}


CComposition_RatingList CCompositionPlayList::CreateRatingList(long lPos)
{
	return CComposition_RatingList(CreateSingleChildNode("RatingList",lPos));
}

CComposition_RatingList CCompositionPlayList::GetRatingList() const
{
	return CComposition_RatingList(GetChildByNameIndexRept("RatingList"));
}


CComposition_ReelList CCompositionPlayList::CreateReelList(long lPos)
{
	return CComposition_ReelList(CreateSingleChildNode("ReelList",lPos));
	
}

CComposition_ReelList CCompositionPlayList::GetReelList() const
{
	return CComposition_ReelList(GetChildByNameIndexRept("ReelList"));
	
}

int CCompositionPlayList::RemoveContentVersion()
{
	return RemoveChildByNameIndex("ContentVersion");
//	CComposition_ContentVersion version = GetContentVersion();
//	return RemoveChildNode(&version);
}

int CCompositionPlayList::RemoveReelList()
{
	return RemoveChildByNameIndex("ReelList");
//	CComposition_ReelList reelList = GetReelList();
//	return RemoveChildNode(&reelList);
}

int CCompositionPlayList::RemoveRatingList()
{
//	return RemoveChildNode(&GetRatingList());
	return RemoveChildByNameIndex("RatingList");
}

