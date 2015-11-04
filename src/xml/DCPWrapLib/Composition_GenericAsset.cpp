// Composition_GenericAsset.cpp: implementation of the CComposition_GenericAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "Composition_GenericAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComposition_GenericAsset::CComposition_GenericAsset(IXMLNode* pNode)
	: CMYUUIDElement(pNode)
{
}

CComposition_GenericAsset::~CComposition_GenericAsset()
{

}

bool CComposition_GenericAsset::SetEditRateElement(const RATIONAL& nval,long lPos)
{
	return SetChildElementText("EditRate",nval,lPos);

}

RATIONAL CComposition_GenericAsset::GetEditRateElement() const
{
	return GetChildElementTextRational("EditRate");

}

bool CComposition_GenericAsset::SetIntrinsicDurationElement(long lval,long lPos)
{
	return SetChildElementText("IntrinsicDuration",lval,lPos);
	
}

long CComposition_GenericAsset::GetIntrinsicDurationElement() const
{
	return GetChildElementTextlong("IntrinsicDuration");
	
}

bool CComposition_GenericAsset::SetEntryPointElement(long lval,long lPos)
{
	return SetChildElementText("EntryPoint",lval,lPos);
	
}

long CComposition_GenericAsset::GetEntryPointElement() const
{
	return GetChildElementTextlong("EntryPoint");
	
}

bool CComposition_GenericAsset::SetDurationElement(long lval,long lPos)
{
	return SetChildElementText("Duration",lval,lPos);
	
}

long CComposition_GenericAsset::GetDurationElement() const
{
	return GetChildElementTextlong("Duration");
	
}
