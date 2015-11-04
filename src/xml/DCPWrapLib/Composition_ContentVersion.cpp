// Composition_ContentVersion.cpp: implementation of the CComposition_ContentVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_ContentVersion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_ContentVersion::CComposition_ContentVersion(IXMLNode* pNode)
	: CElement(pNode)
{
	
}

CComposition_ContentVersion::~CComposition_ContentVersion()
{

}

bool CComposition_ContentVersion::SetIdElement(const char *szval,long lPos)
{
	return SetChildElementText("Id",szval,lPos);

}

std::string CComposition_ContentVersion::GetIdElement() const
{
	return GetChildElementText("Id");

}

bool CComposition_ContentVersion::SetLabelTextElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("LabelText",szval,lPos);
	
}

USERTEXT CComposition_ContentVersion::GetLabelTextElement() const
{
	return GetChildElementTextUsertext("LabelText");
	
}

