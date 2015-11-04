// Composition_TrackFileAsset.cpp: implementation of the CComposition_TrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_TrackFileAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_TrackFileAsset::CComposition_TrackFileAsset(IXMLNode* pNode)
	: CComposition_GenericAsset(pNode)
{
}

CComposition_TrackFileAsset::~CComposition_TrackFileAsset()
{

}

bool CComposition_TrackFileAsset::SetKeyIdElement(const char *szval,long lPos)
{
	return SetChildElementText("KeyId",szval,lPos);
	
}

std::string CComposition_TrackFileAsset::GetKeyIdElement() const
{
	return GetChildElementText("KeyId");
	
}

bool CComposition_TrackFileAsset::SetHashElement(const char *szval,long lPos)
{
	return SetChildElementText("Hash",szval,lPos);
	
}

std::string CComposition_TrackFileAsset::GetHashElement() const
{
	return GetChildElementText("Hash");
	
}
