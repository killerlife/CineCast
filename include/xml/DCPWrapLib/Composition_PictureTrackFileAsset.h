// Composition_PictureTrackFileAsset.h: interface for the CComposition_PictureTrackFileAsset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_PICTURETRACKFILEASSET_H_
#define __COMPOSITION_PICTURETRACKFILEASSET_H_

#include "Composition_TrackFileAsset.h"

class DCPWRAPLIB_API CComposition_PictureTrackFileAsset : public CComposition_TrackFileAsset  
{
public:
	CComposition_PictureTrackFileAsset(IXMLNode* pNode);
	virtual ~CComposition_PictureTrackFileAsset();

public:
	RATIONAL GetScreenAspectRatioElement()const;
	bool SetScreenAspectRatioElement(const RATIONAL& nval,long lPos=-1);
	RATIONAL GetFrameRateElement()const;
	bool SetFrameRateElement(const RATIONAL& nval,long lPos=-1);
};

#endif // __COMPOSITION_PICTURETRACKFILEASSET_H_
