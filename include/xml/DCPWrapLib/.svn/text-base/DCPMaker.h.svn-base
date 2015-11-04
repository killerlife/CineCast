// DCPMaker.h: interface for the CDCPMaker class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_DCPMAKER_H__32830A90_668B_4B66_9966_2686EA1A7F26__INCLUDED_)
//#define AFX_DCPMAKER_H__32830A90_668B_4B66_9966_2686EA1A7F26__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __DCPMAKER_H_
#define __DCPMAKER_H_

#ifdef _MSC_VER// Visual studio 6 specifics
//#if _MSC_VER < 1300
#pragma warning(disable:4251)			// Ignore "identifer > 255 characters" warning
										// This is produced from many STL class specialisations
										// Note: Not all these warnings go away (another MS-Bug??)
#endif // _MSC_VER < 1300
//#pragma warning (disable : 4251)

//#include "ProjectElement.h"
#include "MakeComposition.h"	// Added by ClassView

class DCPWRAPLIB_API CDCPMaker : public CElement
{
public:
	CDCPMaker();
	CDCPMaker(const char* szprefix,const char* sznamespace);
	CDCPMaker(const char* szInfo,SOURCEFLAG nFlag=OPENFILE_FLAG);
	virtual ~CDCPMaker();
public:
	int RemoveComposition(CMake_Composition* pComposition);
	int RemoveCompositionByIndex(int nIndex);
	CMake_Composition GetCompositionByIndex(int nIndex)const;
	CMake_Composition CreateComposition(long lPos=-1);
	std::string GetPacklist()const;
	bool SetPacklist(const char* szVal);

	
};

#endif //__DCPMAKER_H_
