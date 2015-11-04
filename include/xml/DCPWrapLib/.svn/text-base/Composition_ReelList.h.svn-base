// Composition_ReelList.h: interface for the CComposition_ReelList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_REELLIST_H_
#define __COMPOSITION_REELLIST_H_

#include "Composition_Reel.h"

class DCPWRAPLIB_API CComposition_ReelList : public CElement  
{
public:
	CComposition_ReelList(IXMLNode* pNode);
	virtual ~CComposition_ReelList();

public:
	CComposition_Reel CreateReel(long lPos=-1);
	CComposition_Reel GetReelByIndex(int nIndex)const;
	CComposition_Reel GetReelByid(int id)const;
	int RemoveReelByIndex(int nIndex);
	int RemoveReelByid(int id);
	int RemoveReel(CComposition_Reel* pReel);
};

#endif // __COMPOSITION_REELLIST_H_
