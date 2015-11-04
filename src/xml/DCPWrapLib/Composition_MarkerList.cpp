// Composition_MarkerList.cpp: implementation of the CComposition_MarkerList class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_MarkerList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_MarkerList::CComposition_MarkerList(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_MarkerList::~CComposition_MarkerList()
{

}

int CComposition_MarkerList::RemoveMarkerByIndex(int nIndex)
{
	return RemoveChildByNameIndex("Marker",nIndex);
//	CComposition_Marker marker = GetMarkerByIndex(nIndex);
//	return RemoveChildNode(&marker);
	
}

int CComposition_MarkerList::RemoveMarkerByid(int id)
{
	return RemoveChildById(id);
//	CComposition_Marker marker = GetMarkerByid(id);
//	return RemoveChildNode(&marker);
	
}

int CComposition_MarkerList::RemoveMarker(CComposition_Marker* pMarker)
{
	return RemoveChildNode(pMarker);
	
}

CComposition_Marker CComposition_MarkerList::GetMarkerByIndex(int nIndex)const
{
	return CComposition_Marker(GetChildByNameIndexRept("Marker",nIndex));
	
}

CComposition_Marker CComposition_MarkerList::GetMarkerByid(int id)const
{
	return CComposition_Marker(GetChildByIdRept(id));
	
}

CComposition_Marker CComposition_MarkerList::CreateMarker(long lPos)
{
	return CComposition_Marker(CreateChildNodeRept("Marker",lPos,true));
	
}


