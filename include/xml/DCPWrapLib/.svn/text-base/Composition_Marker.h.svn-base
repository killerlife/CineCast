// Composition_Marker.h: interface for the CComposition_Marker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_MARKER_H_
#define __COMPOSITION_MARKER_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CComposition_Marker : public CElement  
{
public:
	CComposition_Marker(IXMLNode* pNode);
	virtual ~CComposition_Marker();

public:
	long GetOffsetElement()const;
	bool SetOffsetElement(long lval,long lPos=-1);
	SCOPEATTR GetLabelElement()const;
	bool SetLabelElement(const SCOPEATTR& szval,long lPos=-1);//attr:scope[op](URI)
	USERTEXT GetAnnotationTextElement()const;
	bool SetAnnotationTextElement(const USERTEXT& szval,long lPos=-1);
	
};

#endif // __COMPOSITION_MARKER_H_
