// Composition_RatingList.h: interface for the CComposition_RatingList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_RATINGLIST_H_
#define __COMPOSITION_RATINGLIST_H_

#include "Composition_Rating.h"

class DCPWRAPLIB_API CComposition_RatingList : public CElement  
{
public:
	CComposition_RatingList(IXMLNode* pNode);
	virtual ~CComposition_RatingList();
public:
	int RemoveRatingById(int id);
	int RemoveRatingByIndex(int nIndex);
	CComposition_Rating GetRatingById(int id)const;
	CComposition_Rating GetRatingByIndex(int nIndex)const;
	CComposition_Rating CreateRating(long lPos=-1);
	int RemoveRating(CComposition_Rating* pRating);
};

#endif // __COMPOSITION_RATINGLIST_H_
