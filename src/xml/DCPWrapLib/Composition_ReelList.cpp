// Composition_ReelList.cpp: implementation of the CComposition_ReelList class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_ReelList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_ReelList::CComposition_ReelList(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_ReelList::~CComposition_ReelList()
{

}

int CComposition_ReelList::RemoveReelByIndex(int nIndex)
{
	return RemoveChildByNameIndex("Reel",nIndex);
//	CComposition_Reel reel = GetReelByIndex(nIndex); 
//	return RemoveChildNode(&reel);
	
}

int CComposition_ReelList::RemoveReelByid(int id)
{
	return RemoveChildById(id);
//	CComposition_Reel reel = GetReelByid(id); 
//	return RemoveChildNode(&reel);
	
}

int CComposition_ReelList::RemoveReel(CComposition_Reel* pReel)
{
	return RemoveChildNode(pReel);
	
}

CComposition_Reel CComposition_ReelList::GetReelByIndex(int nIndex)const
{
	return CComposition_Reel(GetChildByNameIndexRept("Reel",nIndex));
	
}

CComposition_Reel CComposition_ReelList::GetReelByid(int id)const
{
	return CComposition_Reel(GetChildByIdRept(id));
	
}

CComposition_Reel CComposition_ReelList::CreateReel(long lPos)
{
	return CComposition_Reel(CreateChildNodeRept("Reel",lPos,true));
	
}

