// ProjectElement.h: interface for the CProjectElement class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DCIPROJECTELEMENT_H_
#define __DCIPROJECTELEMENT_H_

//#ifdef _MSC_VER// Visual studio 6 specifics
//#pragma warning(disable:4251)			// Ignore "identifer > 255 characters" warning
										// This is produced from many STL class specialisations
										// Note: Not all these warnings go away (another MS-Bug??)
//#endif // _MSC_VER < 1300

//#pragma warning (disable : 4251)

//#include "IssuerElement.h"	// Added by ClassView
#include "dcpwraplib.h"
#include <GeneralXMLLib/UUIDElement.h>
#include "IssuerElement.h"

class DCPWRAPLIB_API CDCIProjectElement : public CUUIDElement  
{
public:
//	CDCIProjectElement(const char* szproject);
//	CDCIProjectElement()
	CDCIProjectElement(const std::string& rootName,const char* szInfo=NULL,SOURCEFLAG nFlag=CREATE_FLAG);
	virtual ~CDCIProjectElement();

public:
	const CIssuerElement GetIssuer();


public:
	USERTEXT GetAnnotationTextElement()const;
	bool SetAnnotationTextElement(const USERTEXT& szval,long lPos=-1);//optional

};

#endif // __DCIPROJECTELEMENT_H_
