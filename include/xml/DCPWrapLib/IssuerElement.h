// IssuerElement.h: interface for the CIssuerElement class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ISSUERELEMENT_H_
#define __ISSUERELEMENT_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CIssuerElement : public CElement  
{
public:
	CIssuerElement(IXMLNode* pNode);
	virtual ~CIssuerElement();

public:
	USERTEXT GetIssuerElement()const;
	bool SetIssuerElement(const USERTEXT& szval,long lPos=-1);//optional
	DATETIME GetIssueDateElement()const;
	bool SetIssueDateElement(const DATETIME& datetime,long lPos=-1);
	USERTEXT GetCreatorElement()const;
	bool SetCreatorElement(const USERTEXT& szval,long lPos=-1);//optional
};

#endif // __ISSUERELEMENT_H_
