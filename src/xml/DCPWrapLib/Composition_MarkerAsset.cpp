// Composition_MarkerAsset.cpp: implementation of the CComposition_MarkerAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_MarkerAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_MarkerAsset::CComposition_MarkerAsset(IXMLNode* pNode)
	: CComposition_GenericAsset(pNode)
{
}

CComposition_MarkerAsset::~CComposition_MarkerAsset()
{

}

CComposition_MarkerList CComposition_MarkerAsset::CreateMarkerList(long lPos)
{
	return CComposition_MarkerList(CreateSingleChildNode("MarkerList",lPos));
	
}

CComposition_MarkerList CComposition_MarkerAsset::GetMarkerList()const
{
	return CComposition_MarkerList(GetChildByNameIndexRept("MarkerList"));
	
}

int CComposition_MarkerAsset::RemoveMarkerList()
{
	return RemoveChildByNameIndex("MarkerList");
//	CComposition_MarkerList markerList = GetMarkerList();
//	return RemoveChildNode(&markerList);
	
}



