// UUIDElement.cpp: implementation of the CUUIDElement class.
//
//////////////////////////////////////////////////////////////////////

#include "UUIDElement.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUUIDElement::CUUIDElement(IXMLNode* pNode)
	: CElement(pNode)
{

}

CUUIDElement::CUUIDElement(const char* rootName,const char* szInfo,SOURCEFLAG nFlag)
:CElement(rootName,szInfo,nFlag)
{

}

CUUIDElement::~CUUIDElement()
{

}

std::string CUUIDElement::GetUUIDElement()const
{
	return GetChildElementText("Id");
}

bool CUUIDElement::SetUUIDElement(const char* szval,long lPos)
{
	return SetChildElementText("Id",szval,lPos);
}

