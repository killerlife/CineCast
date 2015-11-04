// UUIDElement.cpp: implementation of the CMYUUIDElement class.
//
//////////////////////////////////////////////////////////////////////

#include "MyUUIDElement.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMYUUIDElement::CMYUUIDElement(IXMLNode* pNode)
	: CElement(pNode)
{

}

CMYUUIDElement::~CMYUUIDElement()
{

}

std::string CMYUUIDElement::GetUUIDElement()const
{
	return GetChildElementText("Id");
}

bool CMYUUIDElement::SetUUIDElement(const char* szval,long lPos)
{
	return SetChildElementText("Id",szval,lPos);
}

USERTEXT CMYUUIDElement::GetAnnotationTextElement()const
{
	return GetChildElementTextUsertext("AnnotationText");
}

bool CMYUUIDElement::SetAnnotationTextElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("AnnotationText",szval,lPos);
}

