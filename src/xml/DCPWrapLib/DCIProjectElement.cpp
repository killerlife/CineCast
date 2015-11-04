// ProjectElement.cpp: implementation of the CDCIProjectElement class.
//
//////////////////////////////////////////////////////////////////////

#include "DCIProjectElement.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CDCIProjectElement::CDCIProjectElement(const char* szproject)
	: CProjectElement(szproject)
{

}
*/

CDCIProjectElement::CDCIProjectElement(const std::string& rootName,const char* szInfo,SOURCEFLAG nFlag)
:CUUIDElement(rootName.c_str(),szInfo,nFlag)
{

}

CDCIProjectElement::~CDCIProjectElement()
{

}

const CIssuerElement CDCIProjectElement::GetIssuer()
{
	AddRef();
	return CIssuerElement(m_pNode);

}


USERTEXT CDCIProjectElement::GetAnnotationTextElement()const
{
	return GetChildElementTextUsertext("AnnotationText");
}

bool CDCIProjectElement::SetAnnotationTextElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("AnnotationText",szval,lPos);
}



