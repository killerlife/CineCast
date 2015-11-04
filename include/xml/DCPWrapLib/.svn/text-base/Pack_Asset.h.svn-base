// Pack_Asset.h: interface for the CPack_Asset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PACK_ASSET_H_
#define __PACK_ASSET_H_

#include "MyUUIDElement.h"

class DCPWRAPLIB_API CPack_Asset : public CMYUUIDElement
{
public:
	CPack_Asset(IXMLNode* pNode);
	virtual ~CPack_Asset();
	
public:
	std::string GetHashElement()const;
	bool SetHashElement(const char* szval,long lPos=-1);//hexBinary
	Int64 GetSizeElement()const;
	bool SetSizeElement(Int64 nsize,long lPos=-1);
	std::string GetTypeElement()const;
	bool SetTypeElement(const char* szval,long lPos=-1);
	USERTEXT GetOriginalFileNameElement()const;
	bool SetOriginalFileNameElement(const USERTEXT& szval,long lPos=-1);
};

#endif //__PACK_ASSET_H_
