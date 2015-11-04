// MakeComposition.h: interface for the CMakeComposition class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MAKECOMPOSITION_H__0709A6E1_871C_4327_9BF2_B3CB172145B6__INCLUDED_)
//#define AFX_MAKECOMPOSITION_H__0709A6E1_871C_4327_9BF2_B3CB172145B6__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __MAKECOMPOSITION_H_
#define __MAKECOMPOSITION_H_

#include "Make_Reel.h"

class DCPWRAPLIB_API CMake_Composition : public CElement  
{
public:
	CMake_Composition(IXMLNode* pNode);
	virtual ~CMake_Composition();
public:
	std::string GetPlaylist()const;
	bool SetPlaylist(const char* szVal);
	
public:
	int RemoveReelByid(int id);
	CMake_Reel GetReelByid(int id)const;
	int RemoveReel(CMake_Reel* pReel);
	int RemoveReelByIndex(int nIndex);
	CMake_Reel GetReelByIndex(int nIndex) const;
	CMake_Reel CreateReel(long lPos=-1);
};

#endif // __MAKECOMPOSITION_H_


