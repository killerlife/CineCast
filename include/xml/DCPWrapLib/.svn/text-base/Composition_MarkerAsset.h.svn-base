// Composition_MarkerAsset.h: interface for the CComposition_MarkerAsset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_MARKERASSET_H_
#define __COMPOSITION_MARKERASSET_H_

#include "Composition_GenericAsset.h"
#include "Composition_MarkerList.h"

class DCPWRAPLIB_API CComposition_MarkerAsset : public CComposition_GenericAsset  
{
public:
	CComposition_MarkerAsset(IXMLNode* pNode);
	virtual ~CComposition_MarkerAsset();

public:
	CComposition_MarkerList CreateMarkerList(long lPos=-1);
	CComposition_MarkerList GetMarkerList()const;
	int RemoveMarkerList();
};

#endif // __COMPOSITION_MARKERASSET_H_
