// Pack_Asset.cpp: implementation of the CPack_Asset class.
//
//////////////////////////////////////////////////////////////////////

#include "Pack_Asset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPack_Asset::CPack_Asset(IXMLNode* pNode)
	: CMYUUIDElement(pNode)
{
}

CPack_Asset::~CPack_Asset()
{

}

bool CPack_Asset::SetHashElement(const char *szval,long lPos)
{
	return SetChildElementText("Hash",szval,lPos);

}

std::string CPack_Asset::GetHashElement()const
{
	return GetChildElementText("Hash");

}

bool CPack_Asset::SetSizeElement(Int64 nsize,long lPos)
{
	return SetChildElementText("Size",nsize,lPos);

}

Int64 CPack_Asset::GetSizeElement()const
{
	return GetChildElementTextInt64("Size");

}

bool CPack_Asset::SetTypeElement(const char *szval,long lPos)
{
	return SetChildElementText("Type",szval,lPos);
	
}

std::string CPack_Asset::GetTypeElement()const
{
	return GetChildElementText("Type");
	
}

USERTEXT CPack_Asset::GetOriginalFileNameElement()const
{
	return GetChildElementTextUsertext("OriginalFileName");
}

bool CPack_Asset::SetOriginalFileNameElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("OriginalFileName",szval,lPos);
}



