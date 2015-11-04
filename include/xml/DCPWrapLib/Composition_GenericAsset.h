// Composition_GenericAsset.h: interface for the CComposition_GenericAsset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_GENERICASSET_H_
#define __COMPOSITION_GENERICASSET_H_

#include "MyUUIDElement.h"

class DCPWRAPLIB_API CComposition_GenericAsset : public CMYUUIDElement
{
public:
	CComposition_GenericAsset(IXMLNode* pNode);
	virtual ~CComposition_GenericAsset();

public:
	long GetDurationElement()const;
	bool SetDurationElement(long lval,long lPos=-1);//optional
	long GetEntryPointElement()const;
	bool SetEntryPointElement(long lval,long lPos=-1);/*optional*/
	long GetIntrinsicDurationElement()const;
	bool SetIntrinsicDurationElement(long lval,long lPos=-1);
	RATIONAL GetEditRateElement()const;
	bool SetEditRateElement(const RATIONAL& nval,long lPos=-1);
};

#endif // __COMPOSITION_GENERICASSET_H_
