// Composition_Reel.cpp: implementation of the CComposition_Reel class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_Reel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_Reel::CComposition_Reel(IXMLNode* pNode)
	: CMYUUIDElement(pNode)
{
}

CComposition_Reel::~CComposition_Reel()
{

}

CComposition_AssetList CComposition_Reel::CreateAssetList(long lPos)
{
	return CComposition_AssetList(CreateSingleChildNode("AssetList",lPos));
	
}


CComposition_AssetList CComposition_Reel::GetAssetList()const
{
	return CComposition_AssetList(GetChildByNameIndexRept("AssetList"));
	
}

int CComposition_Reel::RemoveAssetList()
{
	return RemoveChildByNameIndex("AssetList");
//	CComposition_AssetList assetList = GetAssetList();
//	return RemoveChildNode(&assetList);
	
}

