// Composition_Marker.cpp: implementation of the CComposition_Marker class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_Marker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_Marker::CComposition_Marker(IXMLNode* pNode)
	: CElement(pNode)
{
}

CComposition_Marker::~CComposition_Marker()
{

}

bool CComposition_Marker::SetLabelElement(const SCOPEATTR& szval,long lPos)
{
	return SetChildElementText("Label",szval,lPos);
}

SCOPEATTR CComposition_Marker::GetLabelElement() const
{
	return GetChildElementTextScopetext("Label");
}

bool CComposition_Marker::SetOffsetElement(long lval,long lPos)
{
	return SetChildElementText("Offset",lval,lPos);
}

long CComposition_Marker::GetOffsetElement() const
{
	return GetChildElementTextlong("Offset");
}

USERTEXT CComposition_Marker::GetAnnotationTextElement()const
{
	return GetChildElementTextUsertext("AnnotationText");
}

bool CComposition_Marker::SetAnnotationTextElement(const USERTEXT& szval,long lPos)
{
	return SetChildElementText("AnnotationText",szval,lPos);
}


