// PackingList.h: interface for the CPackingList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PACKINGLIST_H_
#define __PACKINGLIST_H_

//#include "Signature.h"	// Added by ClassView
#include "Pack_AssetList.h"	// Added by ClassView
#include "DCIProjectElement.h"
//#include "ProjectElement.h"	// Added by ClassView

class DCPWRAPLIB_API CPackingList : public CDCIProjectElement
{
public:
	CPackingList();
	CPackingList(const char* szprefix,const char* sznamespace);
	CPackingList(const char* szInfo,SOURCEFLAG nFlag=OPENFILE_FLAG);
	virtual ~CPackingList();
	
public:
	std::string GetIconUUIDElement()const;
	bool SetIconUUIDElement(const char* szval,long lPos=-1);
	
public:
	CPack_AssetList GetAssetList()const;
	CPack_AssetList CreateAssetList(long lPos=-1);
	int RemoveAssetList();

};

#endif // __PACKINGLIST_H_
