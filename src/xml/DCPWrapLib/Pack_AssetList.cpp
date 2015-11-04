// Pack_AssetList.cpp: implementation of the CPack_AssetList class.
//
//////////////////////////////////////////////////////////////////////

#include "Pack_AssetList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPack_AssetList::CPack_AssetList(IXMLNode* pNode)
	: CElement(pNode)
{
}

CPack_AssetList::~CPack_AssetList()
{

}

int CPack_AssetList::RemoveAssetByIndex(int nIndex)
{
	return RemoveChildByNameIndex("Asset",nIndex);
//	CPack_Asset asset = GetAssetByIndex(nIndex);
//	return RemoveChildNode(&asset);
}

int CPack_AssetList::RemoveAssetByid(int id)
{
	return RemoveChildById(id);
//	CPack_Asset asset = GetAssetByid(id);
//	return RemoveChildNode(&asset);
	
}

int CPack_AssetList::RemoveAsset(CPack_Asset* pTrack)
{
	return RemoveChildNode(pTrack);
}

CPack_Asset CPack_AssetList::GetAssetByIndex(int nIndex)const
{
	return CPack_Asset(GetChildByNameIndexRept("Asset",nIndex));
}

CPack_Asset CPack_AssetList::GetAssetByid(int id)const
{
	return CPack_Asset(GetChildByIdRept(id));
}

CPack_Asset CPack_AssetList::CreateAsset(long lPos)
{
	return CPack_Asset(CreateChildNodeRept("Asset",lPos,true));
}

