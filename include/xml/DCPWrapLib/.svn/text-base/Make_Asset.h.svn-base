// Make_Track.h: interface for the CMake_Asset class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MAKE_TRACK_H__C30C13D2_E6CE_4B92_9678_80A8FD6EFBC2__INCLUDED_)
//#define AFX_MAKE_TRACK_H__C30C13D2_E6CE_4B92_9678_80A8FD6EFBC2__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __MAKE_ASSET_H_
#define __MAKE_ASSET_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CMake_Asset : public CElement  
{
public:
	CMake_Asset(IXMLNode* pNode);
	virtual ~CMake_Asset();

public:
	bool SetType(const char* szVal);
	std::string GetType()const;

	std::string GetOtherwrapOption()const;
	bool SetOtherwrapOption(const char* szVal);
	Int64 GetSrcDuration()const;
	bool SetSrcDuration(const Int64 nVal);
	Int64 GetSrcOrigin()const;
	bool SetSrcOrigin(const Int64 nVal);
	RATIONAL GetSrcEditrate()const;
	bool SetSrcEditrate(const RATIONAL& szVal);
	std::string GetDst()const;
	bool SetDst(const char* szVal);
	std::string GetSrc()const;
	bool SetSrc(const char* szVal);

	bool SetTrackID(const char* szVal);//track file uuid
	std::string GetTrackID()const;
	
	bool SetPlayListID(const int nVal);//composition playlist id attr
	int GetPlayListID()const;
};

#endif // __MAKE_ASSET_H_


