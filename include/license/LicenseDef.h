//////////////////////////////////////////////////////////////////////////
//license control h
#ifndef __LICENSEDEF_H_200811251753__
#define __LICENSEDEF_H_200811251753__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <time.h>
#include "brunt/simpledate.h"

#ifdef _WIN32

#ifdef LICENSE_EXPORTS
#define LICENSE_API __declspec(dllexport)
#else
#define LICENSE_API __declspec(dllimport)
#endif

#else

#define LICENSE_API 

#endif//_WIN32


struct TLicenseInfo
{
//	std::string licensecode;
//	_terminalcode terminal;
	std::string moviecode;
	int licensenum;
	int playnum;
	int leftnum;
	CSimpleDate startdate;
	CSimpleDate enddate;
	char secretkey[16];		
	std::string keyid;
	std::string keystr;
};

enum LICENSETYPE{
		LST_TAMT = 0,		// Time antaeus media tech.
		LST_KDM,			// kdm
		LST_DMS			// dms
};

enum LICENSEVERIFY{
	LSC_FAILED = -1,			// 授权检查失败
	LSC_OK = 0,					// license check ok
	LSC_NOKEY,				// NO KEY for license
	LSC_DATE_OVERFLOW,		// 档期超期
	LSC_COUNT_OVERFLOW		//场次超次
};


#endif	//__LICENSEDEF_H_200811251753__

