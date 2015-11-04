// Composition_MarkerList.h: interface for the CComposition_MarkerList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_MARKERLIST_H_
#define __COMPOSITION_MARKERLIST_H_

#include "Composition_Marker.h"

class DCPWRAPLIB_API CComposition_MarkerList : public CElement  
{
public:
	CComposition_MarkerList(IXMLNode* pNode);
	virtual ~CComposition_MarkerList();

public:
	int RemoveMarkerByIndex(int nIndex);
	int RemoveMarkerByid(int id);
	int RemoveMarker(CComposition_Marker* pMarker);
	CComposition_Marker GetMarkerByIndex(int nIndex)const;
	CComposition_Marker GetMarkerByid(int id)const;
	CComposition_Marker CreateMarker(long lPos=-1);
	
};

#endif // __COMPOSITION_MARKERLIST_H_
