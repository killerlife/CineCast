// Composition_PictureTrackFileAsset.cpp: implementation of the CComposition_PictureTrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_PictureTrackFileAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_PictureTrackFileAsset::CComposition_PictureTrackFileAsset(IXMLNode* pNode)
	: CComposition_TrackFileAsset(pNode)
{
}

CComposition_PictureTrackFileAsset::~CComposition_PictureTrackFileAsset()
{

}

bool CComposition_PictureTrackFileAsset::SetFrameRateElement(const RATIONAL& nval,long lPos)
{

	return SetChildElementText("FrameRate",nval,lPos);
}

RATIONAL CComposition_PictureTrackFileAsset::GetFrameRateElement() const
{
	return GetChildElementTextRational("FrameRate");
	
}

bool CComposition_PictureTrackFileAsset::SetScreenAspectRatioElement(const RATIONAL& nval,long lPos)
{
	return SetChildElementText("ScreenAspectRatio",nval,lPos);
	
}

RATIONAL CComposition_PictureTrackFileAsset::GetScreenAspectRatioElement() const
{
	return GetChildElementTextRational("ScreenAspectRatio");
	
}
