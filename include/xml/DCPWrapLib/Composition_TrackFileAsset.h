// Composition_TrackFileAsset.h: interface for the CComposition_TrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_TRACKFILEASSET_H_
#define __COMPOSITION_TRACKFILEASSET_H_

#include "Composition_GenericAsset.h"

class DCPWRAPLIB_API CComposition_TrackFileAsset : public CComposition_GenericAsset  
{
public:
	CComposition_TrackFileAsset(IXMLNode* pNode);
	virtual ~CComposition_TrackFileAsset();

public:
	std::string GetHashElement()const;//base64Binary
	bool SetHashElement(const char* szval,long lPos=-1);//optional
	std::string GetKeyIdElement()const;
	bool SetKeyIdElement(const char* szval,long lPos=-1);//optional
};

#endif // __COMPOSITION_TRACKFILEASSET_H_
