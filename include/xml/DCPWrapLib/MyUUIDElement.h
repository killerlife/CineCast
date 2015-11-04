// MYUUIDElement.h: interface for the CMYUUIDElement class.
//
//////////////////////////////////////////////////////////////////////
//set uuid and annotationtext

#ifndef __MYUUIDELEMENT_H_
#define __MYUUIDELEMENT_H_

#include "dcpwraplib.h"
#include <GeneralXMLLib/Element.h>

class DCPWRAPLIB_API CMYUUIDElement : public CElement  
{
public:
	CMYUUIDElement(IXMLNode* pNode);
	virtual ~CMYUUIDElement();

public:
	std::string GetUUIDElement()const;
	bool SetUUIDElement(const char* szval,long lPos=0);
	USERTEXT GetAnnotationTextElement()const;
	bool SetAnnotationTextElement(const USERTEXT& szval,long lPos=-1);//optional
	
};

#endif // __MYUUIDELEMENT_H_
