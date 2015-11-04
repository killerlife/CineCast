// PackingList.cpp: implementation of the CPackingList class.
//
//////////////////////////////////////////////////////////////////////

#include "PackingList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackingList::CPackingList()
:CDCIProjectElement("PackingList",NULL,CREATE_FLAG)
{
}

CPackingList::CPackingList(const char* szprefix,const char* sznamespace)
:CDCIProjectElement(std::string(szprefix)+std::string(":PackingList"),sznamespace,CREATE_FLAG)
{

}

CPackingList::CPackingList(const char* szInfo,SOURCEFLAG nFlag)
:CDCIProjectElement("PackingList",szInfo,nFlag)
{

}

CPackingList::~CPackingList()
{

}

bool CPackingList::SetIconUUIDElement(const char *szval,long lPos)
{
	return SetChildElementText("IconId",szval,lPos);

}

std::string CPackingList::GetIconUUIDElement()const
{
	return GetChildElementText("IconId");

}


CPack_AssetList CPackingList::CreateAssetList(long lPos)
{
	return CPack_AssetList(CreateSingleChildNode("AssetList",lPos));
}

CPack_AssetList CPackingList::GetAssetList() const
{
	return CPack_AssetList(GetChildByNameIndexRept("AssetList"));

}

int CPackingList::RemoveAssetList()
{
	return RemoveChildByNameIndex("AssetList");
//	CPack_AssetList assetList = GetAssetList();
//	return RemoveChildNode(&assetList);
}


