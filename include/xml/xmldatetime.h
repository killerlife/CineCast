/////////////////////////////////////////////////////////////////////
//xmldatetime.h

#ifndef __XMLDATETIME20060927_H_
#define __XMLDATETIME20060927_H_


#ifdef _MSC_VER// Visual studio 6 specifics
#pragma warning(disable:4275)			// Ignore "identifer > 255 characters" warning
										// This is produced from many STL class specialisations
										// Note: Not all these warnings go away (another MS-Bug??)
#endif // _MSC_VER < 1300

#include <time.h>
#include <string>

#include <datatype/DataType.h>

class _xmldatetime : public _xmlDataType::DATETIME
{
public:
	_xmldatetime(const time_t& t);
	_xmldatetime(int nY=1900,int nM=1,int nD=1,int nh=0,int nmin=0,int nsec=0,int nmillsec=0,bool btz=false,int ntzh=0,int ntzmin=0);
	_xmldatetime(const char* szval);
public:
	int GetYear()const{return nYear;};
	int GetMonth()const{return nMonth;};
	int GetDay()const{return nDay;};
	int GetHour()const{return nHour;};
	int GetMinute()const{return nMinute;};
	int GetSecond()const{return nSecond;};
	int GetMillsecond()const{return nMillSecond;};
	bool GetZoneFlag()const{return bTimeZoned;}
	int GetZoneHour()const{return nZoneHour;};
	int GetZoneMinute()const{return nZoneMinute;};

	//method
public:
	void GmtToLocal();
	void LocalToGmt();

public:
	static long GetTimeZone();//返回值单位为分，+-号表示东区或西区
public:
	virtual const _xmldatetime& operator=(const _xmldatetime& cref);
	virtual const _xmldatetime& operator=(const time_t& t);
	friend bool operator>(const _xmldatetime& leftval, const _xmldatetime& rightval);
	friend bool operator<(const _xmldatetime& leftval, const _xmldatetime& rightval);
	friend bool operator==(const _xmldatetime& leftval, const _xmldatetime& rightval);
	friend bool operator>=(const _xmldatetime& leftval, const _xmldatetime& rightval);
	friend bool operator<=(const _xmldatetime& leftval, const _xmldatetime& rightval);


private:
	void convertdateByDay();
	void convertdateByTime();

};


typedef struct _datezone
{
	_xmldatetime startDate;
	_xmldatetime endDate;
}DATEZONE;


#endif //__XMLDATETIME20060927_H_

