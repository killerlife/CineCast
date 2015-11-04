// Composition_Rating.cpp: implementation of the CComposition_Rating class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_Rating.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_Rating::CComposition_Rating(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_Rating::~CComposition_Rating()
{

}

bool CComposition_Rating::SetAgencyElement(const char *szval,long lPos)
{
	return SetChildElementText("Agency",szval,lPos);

}

std::string CComposition_Rating::GetAgencyElement() const
{
	return GetChildElementText("Agency");
}


bool CComposition_Rating::SetLabelElement(const char *szval,long lPos)
{
	return SetChildElementText("Label",szval,lPos);
}

std::string CComposition_Rating::GetLabelElement() const
{
	return GetChildElementText("Label");
}
