// Composition_ContentVersion.h: interface for the CComposition_ContentVersion class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITION_CONTENTVERSION_H_
#define __COMPOSITION_CONTENTVERSION_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CComposition_ContentVersion : public CElement  
{
public:
	CComposition_ContentVersion(IXMLNode* pNode);
	virtual ~CComposition_ContentVersion();

public:
	USERTEXT GetLabelTextElement()const;
	bool SetLabelTextElement(const USERTEXT& szval,long lPos=-1);
	std::string GetIdElement()const;
	bool SetIdElement(const char* szval,long lPos=0);

};

#endif // __COMPOSITION_CONTENTVERSION_H_
