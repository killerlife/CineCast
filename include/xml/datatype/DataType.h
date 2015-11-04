#ifndef __DATATYPE_H_
#define __DATATYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "MyMacroDef.h"

#include <string>
#include <time.h>


//////////////////////////////////////////////////////////////////////////
//datetime 见XML Schema Part 2: Datatypes Second Edition(http://www.w3.org/TR/xmlschema-2/) 3.2.7
namespace _xmlDataType
{
#pragma pack(push,1)

struct _DATE_TIME
{
public:
	int nYear;
	int nMonth;//1~12
	int nDay;//1~31
	int nHour;//0~23
	int nMinute;//0~59
	int nSecond;//0~59
	int nMillSecond;//0~1000

	//timezoned
	bool bTimeZoned;
	int nZoneHour;//0~14
	int nZoneMinute;//0~59(nZoneHour=14时，只能为0)
public:
	_DATE_TIME(int nY=1,int nM=1,int nD=1,int nh=0,int nmin=0,int nsec=0,int nmillsec=0,bool btz=false,int ntzh=0,int ntzmin=0)
		:nYear(nY),nMonth(nM),nDay(nD),nHour(nh),nMinute(nmin),nSecond(nsec),nMillSecond(nmillsec),bTimeZoned(btz),nZoneHour(ntzh),nZoneMinute(ntzmin)
	{}
	_DATE_TIME(const char* szval)
	{
		int nlen = (int)strlen(szval);
		if(nlen < 19)
			return;
		nYear = general_func::GetIntegerValue(szval,0,4);
		nMonth = general_func::GetIntegerValue(szval,5,2);
		nDay = general_func::GetIntegerValue(szval,8,2);
		nHour = general_func::GetIntegerValue(szval,11,2);
		nMinute = general_func::GetIntegerValue(szval,14,2);
		nSecond = general_func::GetIntegerValue(szval,17,2);
		
		nMillSecond = 0;
		bTimeZoned = false;
		nZoneHour = 0;
		nZoneMinute = 0;
		if(nlen > 19)
		{
			int npos = 19;
			if(nlen >= npos+4 && szval[npos] == '.')
			{
				nMillSecond = general_func::GetIntegerValue(szval,npos+1,3);
				npos+=4;
			}
			if( nlen > npos && (szval[npos] == '+' || szval[npos] == '-' || szval[npos] == 'Z'))
			{
				bTimeZoned = true;
				if(nlen == npos + 6 && szval[npos] != 'Z')
				{
					nZoneHour = general_func::GetIntegerValue(szval,npos+1,2);
					nZoneMinute = general_func::GetIntegerValue(szval,npos+4,2);
					if(szval[npos] == '-')
					{
						nZoneHour = -nZoneHour;
						nZoneMinute = -nZoneMinute;
					}
				}
			}
		}
	}
	
public:
	std::string GetString()const
	{
		char szval[255];
		sprintf(szval,"%04d-%02d-%02dT%02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
		std::string strreval = szval;
		if(nMillSecond > 0)
		{
			sprintf(szval,".%03d",nMillSecond);
			strreval += szval;
		}
		if(bTimeZoned)
		{
			if(nZoneHour != 0 || nZoneMinute != 0)
			{
				bool bnegative = false;
				int ntmphour=nZoneHour,ntmpMinute=nZoneMinute;
				if(nZoneHour < 0)
				{
					bnegative = true;
					ntmphour = -nZoneHour;
				}
				if(nZoneMinute < 0)
				{
					bnegative = true;
					ntmpMinute = -nZoneMinute;
				}
				if(bnegative)
					sprintf(szval,"-%02d:%02d",ntmphour,ntmpMinute);
				else
					sprintf(szval,"+%02d:%02d",ntmphour,ntmpMinute);
				strreval += szval;
			}
			else
				strreval += "Z";
		}
		return strreval;
	}
	const _DATE_TIME& GetCurrentTime()
	{
		time_t curtime;
		time(&curtime);
		tm* ptmp = gmtime(&curtime);
		tm putctime = *ptmp;
		tm* pcurtime = localtime(&curtime);
		nYear = pcurtime->tm_year + 1900;
		nMonth = pcurtime->tm_mon + 1;
		nDay = pcurtime->tm_mday;
		nHour = pcurtime->tm_hour;
		nMinute = pcurtime->tm_min;
		nSecond = pcurtime->tm_sec;
		bTimeZoned = true;
		nZoneHour = pcurtime->tm_hour - putctime.tm_hour;
		nZoneMinute = 0;
		return *this;
	}
	
};

typedef _DATE_TIME DATETIME;

struct _RATIONAL
{
public:
	long lNmerator;
	long lDenominator;
public:
	_RATIONAL():lDenominator(1)
	{}
	_RATIONAL(long lnmer,long ldeno=1):lNmerator(lnmer),lDenominator(ldeno)
	{}
	_RATIONAL(const char* szval)
	{
		lNmerator = 0;
		lDenominator = 1;
		if(szval)
		{
			int nfind = general_func::findstringpos(szval," ");
			if(nfind <= 0)
				return;
			lNmerator = general_func::GetIntegerValue(szval,0,nfind);
			int nleft = (int)strlen(szval)-nfind-1;
			if(nleft > 0)
			{
				lDenominator = general_func::GetIntegerValue(szval,nfind+1,nleft);
				if(lDenominator <= 0)
					lDenominator = 1;
			}
		}
	}
	
	
public:
	_RATIONAL operator=(long lval)	{lNmerator=lval; return *this;}
	bool operator==(const _RATIONAL& cref)const	{return ((lNmerator==cref.lNmerator)&&(lDenominator==cref.lDenominator));}
	std::string GetString(int nFlag=0)const
	{
		char szval[20];
		if(nFlag == 1)
			sprintf(szval,"%ld/%ld",lNmerator,lDenominator);
		else
			sprintf(szval,"%ld %ld",lNmerator,lDenominator);
		return std::string(szval);
	}
};

typedef _RATIONAL RATIONAL;


typedef struct _OPTIONAL_ATTR
{
	std::string strValue;
	std::string strAttrValue;
	bool bAttribute;
	std::string strAttrName;
	//construct
public:
	_OPTIONAL_ATTR(const char* szval="",const char* szattr="",bool battr=false,const char* szname="")
		:strValue(szval),strAttrValue(szattr),bAttribute(battr),strAttrName(szname)
	{}
	virtual ~_OPTIONAL_ATTR(){}
	//method
public:
	virtual _OPTIONAL_ATTR operator=(const char* szval){strValue = szval; return *this;}
	virtual bool operator==(const _OPTIONAL_ATTR& cref)const
	{
		return ((StrNoCaseCompare(strValue.c_str(),cref.strValue.c_str())==0)
				&& (StrNoCaseCompare(strAttrValue.c_str(),cref.strAttrValue.c_str())==0)
				&& (StrNoCaseCompare(strAttrName.c_str(),cref.strAttrName.c_str())==0)
				&& (bAttribute==cref.bAttribute));
	}
}OPTIONAL_ATTR;

struct _USERTEXT : public _OPTIONAL_ATTR
{
	//construct
public:
	_USERTEXT():_OPTIONAL_ATTR("","eng",false,"Language"){}
	_USERTEXT(const char* szval,const char* szattr="eng",bool battr=false):_OPTIONAL_ATTR(szval,szattr,battr,"Language"){}
};

struct _SCOPEATTR : public _OPTIONAL_ATTR
{
	//construct
public:
	_SCOPEATTR():_OPTIONAL_ATTR("","",false,"Scope"){}
	_SCOPEATTR(const char* szval,const char* szattr="",bool battr=false):_OPTIONAL_ATTR(szval,szattr,battr,"Scope"){}
};

#pragma pack(pop)

typedef _USERTEXT USERTEXT;
typedef _SCOPEATTR SCOPEATTR;

typedef std::string base64Binary;
typedef std::string CryptoBinary;
typedef std::string hexBinary;

};

#endif

