// Make_Track.h: interface for the CMake_Track class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MAKE_TRACK_H__C30C13D2_E6CE_4B92_9678_80A8FD6EFBC2__INCLUDED_)
//#define AFX_MAKE_TRACK_H__C30C13D2_E6CE_4B92_9678_80A8FD6EFBC2__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __MAKE_TRACK_H_
#define __MAKE_TRACK_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CMake_Track : public CElement  
{
public:
	CMake_Track(IXMLNode* pNode=NULL);
	virtual ~CMake_Track();
public:
	std::string GetOtherwrapOption()const;
	bool SetOtherwrapOption(const char* szVal);
	std::string GetSrcDuration()const;
	bool SetSrcDuration(const char* szVal);
	std::string GetSrcOrigin()const;
	bool SetSrcOrigin(const char* szVal);
	std::string GetSrcEditrate()const;
	bool SetSrcEditrate(const char* szVal);
	std::string GetDst()const;
	bool SetDst(const char* szVal);
	std::string GetSrc()const;
	bool SetSrc(const char* szVal);

};

#endif // __MAKE_TRACK_H_


