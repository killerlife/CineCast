// Composition_RatingList.cpp: implementation of the CComposition_RatingList class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_RatingList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_RatingList::CComposition_RatingList(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_RatingList::~CComposition_RatingList()
{

}

int CComposition_RatingList::RemoveRating(CComposition_Rating *pRating)
{
	return RemoveChildNode(pRating);
}

CComposition_Rating CComposition_RatingList::CreateRating(long lPos)
{
	return CComposition_Rating(CreateChildNodeRept("Rating",lPos,true));
	
}

CComposition_Rating CComposition_RatingList::GetRatingByIndex(int nIndex)const
{
	return CComposition_Rating(GetChildByNameIndexRept("Rating",nIndex));
	
}

CComposition_Rating CComposition_RatingList::GetRatingById(int id) const
{
	return CComposition_Rating(GetChildByIdRept(id));
	
}

int CComposition_RatingList::RemoveRatingByIndex(int nIndex)
{
	return RemoveChildByNameIndex("Rating",nIndex);
//	CComposition_Rating rating = GetRatingByIndex(nIndex);
//	return RemoveChildNode(&rating);
}

int CComposition_RatingList::RemoveRatingById(int id)
{
	return RemoveChildById(id);
//	CComposition_Rating rating = GetRatingById(id);
//	return RemoveChildNode(&rating);
}
