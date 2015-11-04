// UUIDElement.h: interface for the CUUIDElement class.
//
//////////////////////////////////////////////////////////////////////
//set uuid and annotationtext

#ifndef __UUIDELEMENT_H_
#define __UUIDELEMENT_H_

#include "Element.h"

class GENERALXMLLIB_API CUUIDElement : public CElement  
{
public:
	CUUIDElement(IXMLNode* pNode);
	CUUIDElement(const char* rootName,const char* szInfo=NULL,SOURCEFLAG nFlag=CREATE_FLAG);
	virtual ~CUUIDElement();

public:
	std::string GetUUIDElement()const;
	bool SetUUIDElement(const char* szval,long lPos=0);
	
};

#endif // __UUIDELEMENT_H_
