// Make_Reel.h: interface for the CMake_Reel class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MAKE_REEL_H__5BB9325D_74B7_4689_8EA5_387C491F18E5__INCLUDED_)
//#define AFX_MAKE_REEL_H__5BB9325D_74B7_4689_8EA5_387C491F18E5__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __MAKE_REEL_H_
#define __MAKE_REEL_H_

#include "Make_Asset.h"


class DCPWRAPLIB_API CMake_Reel : public CElement  
{
public:
	CMake_Reel(IXMLNode* pNode);
	virtual ~CMake_Reel();

public:
/*
	CMake_Asset CreateAsset(const char* szType,long lpos=-1);
	CMake_Asset GetAssetByIndex(int nIndex)const;
	CMake_Asset GetAssetByid(int id)const;
	CMake_Asset GetAssetByTypeIndex(const char* szType,int nIndex=0)const;
	int RemoveAssetByIndex(int nIndex);
	int RemoveAssetByid(int id);
	int RemoveAssetByTypeIndex(const char* szType,int nIndex=0);
	int RemoveAsset(CMake_Asset* pTrack);
*/
	
	CMake_Asset CreatePicture(long lpos=-1);
	CMake_Asset GetPicture()const;
	int RemovePicture();
	
	CMake_Asset CreateSound(long lpos=-1);
	CMake_Asset GetSound()const;
	int RemoveSound();
	
	CMake_Asset CreateAuxSound(long lpos=-1);
	CMake_Asset GetAuxSoundByIndex(int nIndex=0)const;
	CMake_Asset GetAuxSoundByid(int id)const;
	int RemoveAuxSound(CMake_Asset* maketrack);

	CMake_Asset CreateSubtitle(long lpos=-1);
	CMake_Asset GetSubtitle()const;
	int RemoveSubtitle();

	CMake_Asset CreateOtherAsset(const char* szName,long lpos=-1);
	CMake_Asset GetOtherAssetByIndex(const char* szName,int nIndex=0)const;
	CMake_Asset GetOtherAssetByid(int id)const;
	int RemoveOtherAsset(CMake_Asset* makeAsset);
};

#endif // __MAKE_REEL_H_

