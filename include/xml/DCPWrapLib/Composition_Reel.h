// Composition_Reel.h: interface for the CComposition_Reel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_REEL_H_
#define __COMPOSITION_REEL_H_

#include "Composition_AssetList.h"
#include "MyUUIDElement.h"

class DCPWRAPLIB_API CComposition_Reel : public CMYUUIDElement
{
public:
	CComposition_Reel(IXMLNode* pNode);
	virtual ~CComposition_Reel();

public:
	CComposition_AssetList CreateAssetList(long lPos=-1);
	CComposition_AssetList GetAssetList()const;
	int RemoveAssetList();
};

#endif // __COMPOSITION_REEL_H_
