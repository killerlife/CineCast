// IssuerElement.cpp: implementation of the CIssuerElement class.
//
//////////////////////////////////////////////////////////////////////

#include "IssuerElement.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIssuerElement::CIssuerElement(IXMLNode* pNode)
	: CElement(pNode)
{
}

CIssuerElement::~CIssuerElement()
{
}


bool CIssuerElement::SetIssueDateElement(const DATETIME& datetime,long lPos)
{
	return SetChildElementText("IssueDate",datetime,lPos);
}

DATETIME CIssuerElement::GetIssueDateElement() const
{
	return GetChildElementTextDatetime("IssueDate");
}

bool CIssuerElement::SetIssuerElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("Issuer",szval,lPos);
}

USERTEXT CIssuerElement::GetIssuerElement() const
{
	return GetChildElementTextUsertext("Issuer");
}

bool CIssuerElement::SetCreatorElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("Creator",szval,lPos);
	
}

USERTEXT CIssuerElement::GetCreatorElement() const
{
	return GetChildElementTextUsertext("Creator");
	
}

