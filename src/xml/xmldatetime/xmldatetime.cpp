// xmldatetime.cpp : 定义 DLL 应用程序的入口点。
//

#include <iostream>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////
//
#include "xmldatetime.h"
using namespace _xmlDataType;

_xmldatetime::_xmldatetime(const time_t& t)
{
	tm* ptmp = gmtime(&t);
	tm putctime = *ptmp;
	tm* pcurtime = localtime(&t);
	nYear = pcurtime->tm_year + 1900;
	nMonth = pcurtime->tm_mon + 1;
	nDay = pcurtime->tm_mday;
	nHour = pcurtime->tm_hour;
	nMinute = pcurtime->tm_min;
	nSecond = pcurtime->tm_sec;
	bTimeZoned = true;
	nZoneHour = pcurtime->tm_hour - putctime.tm_hour;
	nZoneMinute = 0;
	nMillSecond = 0;
}

_xmldatetime::_xmldatetime(int nY,int nM,int nD,int nh,int nmin,int nsec,int nmillsec,bool btz,int ntzh,int ntzmin)
:_xmlDataType::DATETIME(nY,nM,nD,nh,nmin,nsec,nmillsec,btz,ntzh,ntzmin)
{
	if(nYear < 1900)
		GetCurrentTime();
}

_xmldatetime::_xmldatetime(const char* szval)
:_xmlDataType::DATETIME(szval)
{
	if(strlen(szval)<19)
	{
		nMillSecond = 0;
		GetCurrentTime();
	}
}

const _xmldatetime& _xmldatetime::operator=(const _xmldatetime& cref)
{
	nYear = cref.nYear;
	nMonth = cref.nMonth;
	nDay = cref.nDay;
	nHour = cref.nHour;
	nMinute = cref.nMinute;
	nSecond = cref.nSecond;
	nMillSecond = cref.nMillSecond;
	bTimeZoned = cref.bTimeZoned;
	nZoneHour = cref.nZoneHour;
	nZoneMinute = cref.nZoneMinute;
	return *this;
}

const _xmldatetime& _xmldatetime::operator=(const time_t& t)
{
	tm* pcurtime = gmtime(&t);
	nYear = pcurtime->tm_year + 1900;
	nMonth = pcurtime->tm_mon + 1;
	nDay = pcurtime->tm_mday;
	nHour = pcurtime->tm_hour;
	nMinute = pcurtime->tm_min;
	nSecond = pcurtime->tm_sec;
	nMillSecond = 0;
	bTimeZoned = true;
	nZoneHour = 0;
	nZoneMinute = 0;
	return *this;
}

bool operator>(const _xmldatetime& leftval, const _xmldatetime& rightval)
{
	_xmldatetime val1 = leftval;
	if(!val1.bTimeZoned)
	{
		val1.bTimeZoned = true;
		val1.nHour -= 14 ;
	}
	val1.LocalToGmt();
	_xmldatetime val2 = rightval;
	if(!val2.bTimeZoned)
	{
		val2.bTimeZoned = true;
		val2.nHour += 14;
	}
	val2.LocalToGmt();
	if(val1.nYear > val2.nYear)
		return true;
	else if(val1.nYear == val2.nYear)
	{
		if(val1.nMonth > val2.nMonth)
			return true;
		else if(val1.nMonth == val2.nMonth)
		{
			Int64 ltime1 = (Int64)((val1.nDay*24 + val1.nHour)*3600+val1.nMinute*60+val1.nSecond)*1000 + val1.nMillSecond;
			Int64 ltime2 = (Int64)((val2.nDay*24 + val2.nHour)*3600+val2.nMinute*60+val2.nSecond)*1000 + val2.nMillSecond;
			if(ltime1 > ltime2)
				return true;
		}
	}
	return false;
}

bool operator<(const _xmldatetime& leftval, const _xmldatetime& rightval)
{
	_xmldatetime val1 = leftval;
	if(!val1.bTimeZoned)
	{
		val1.bTimeZoned = true;
		val1.nHour += 14;
	}
	val1.LocalToGmt();
	_xmldatetime val2 = rightval;
	if(!val2.bTimeZoned)
	{
		val2.bTimeZoned = true;
		val2.nHour -= 14;
	}
	val2.LocalToGmt();
	if(val1.nYear < val2.nYear)
		return true;
	else if(val1.nYear == val2.nYear)
	{
		if(val1.nMonth < val2.nMonth)
			return true;
		else if(val1.nMonth == val2.nMonth)
		{
			Int64 ltime1 = (Int64)((val1.nDay*24 + val1.nHour)*3600+val1.nMinute*60+val1.nSecond)*1000 + val1.nMillSecond;
			Int64 ltime2 = (Int64)((val2.nDay*24 + val2.nHour)*3600+val2.nMinute*60+val2.nSecond)*1000 + val2.nMillSecond;
			if(ltime1 < ltime2)
				return true;
		}
	}
	return false;
}

bool operator==(const _xmldatetime& leftval, const _xmldatetime& rightval)
{
	if(!leftval.bTimeZoned || !rightval.bTimeZoned)
		return false;
	_xmldatetime val1 = leftval;
	val1.LocalToGmt();
	_xmldatetime val2 = rightval;
	val2.LocalToGmt();
	if(val1.nYear == val2.nYear
		&& val1.nMonth      == val2.nMonth
		&& val1.nDay        == val2.nDay
		&& val1.nHour       == val2.nHour
		&& val1.nMinute     == val2.nMinute
		&& val1.nSecond     == val2.nSecond
		&& val1.nMillSecond == val2.nMillSecond)
		return true;
	return false;
}

bool operator>=(const _xmldatetime& leftval, const _xmldatetime& rightval)
{
	return (leftval>rightval || leftval==rightval);
}

bool operator<=(const _xmldatetime& leftval, const _xmldatetime& rightval)
{
	return (leftval<rightval || leftval==rightval);
}

long _xmldatetime::GetTimeZone()
{
	time_t curtime;
	time(&curtime);
	tm* ptmp = gmtime(&curtime);
	tm putctime = *ptmp;
	tm* pcurtime = localtime(&curtime);
	long nZoneHour = pcurtime->tm_hour - putctime.tm_hour;
	long nZoneMinute = pcurtime->tm_min - putctime.tm_min;
	return nZoneHour*60+nZoneMinute;
}

void _xmldatetime::GmtToLocal()
{
	if(!bTimeZoned || nZoneHour || nZoneMinute)
		return;
	long zoneTime = _xmldatetime::GetTimeZone();
	nZoneHour = zoneTime/60;
	nZoneMinute = zoneTime%60;
	bTimeZoned = true;
	nMinute = nMinute + nZoneMinute;
	nHour = nHour + nZoneHour;
	convertdateByTime();
	convertdateByDay();
}

void _xmldatetime::LocalToGmt()
{
	if(!bTimeZoned)
		return;
	nHour = nHour - nZoneHour;
	nMinute = nMinute - nZoneMinute;
	nZoneMinute = 0;
	nZoneHour = 0;
	convertdateByTime();
	convertdateByDay();

}

void _xmldatetime::convertdateByTime()
{
	int ntmpsec= nSecond/60;
	nSecond = nSecond%60;
	if(nSecond < 0)
	{
		nSecond += 60;
		ntmpsec -= 1;
	}
	nMinute += ntmpsec;

	int ntmpmin = nMinute/60;
	nMinute = nMinute%60;
	if(nMinute < 0)
	{
		nMinute += 60;
		ntmpmin -= 1;
	}
	nHour += ntmpmin;

	int ntmphour = nHour/24;
	nHour = nHour%24;
	if(nHour < 0)
	{
		nHour += 24;
		ntmphour -= 1;
	}
	nDay += ntmphour;

}

void _xmldatetime::convertdateByDay()
{
	if(nDay <= 0)
	{
		switch(nMonth)
		{
		case 1:
			nDay += 31;
			nMonth = 12;
			nYear -= 1;
			break;
		case 2:
			nDay += 31;
			nMonth -= 1;
			break;
		case 3:
			if(nYear%4)
				nDay += 28;
			else
				nDay += 29;
			nMonth -= 1;
			break;
		case 4:
			nDay += 31;
			nMonth -= 1;
			break;
		case 5:
			nDay += 30;
			nMonth -= 1;
			break;
		case 6:
			nDay += 31;
			nMonth -= 1;
			break;
		case 7:
			nDay += 30;
			nMonth -= 1;
			break;
		case 8:
			nDay += 31;
			nMonth -= 1;
			break;
		case 9:
			nDay += 31;
			nMonth -= 1;
			break;
		case 10:
			nDay += 30;
			nMonth -= 1;
			break;
		case 11:
			nDay += 31;
			nMonth -= 1;
			break;
		case 12:
			nDay += 30;
			nMonth -= 1;
			break;
		default:
			throw std::runtime_error("datetime exception: format error");
			break;
		}
	}
	else
	{
		switch(nMonth)
		{
		case 1:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 2:
			if(nYear%4)
			{
				if(nDay > 28)
				{
					nDay = 1;
					nMonth += 1;
				}
			}
			else
			{
				if(nDay > 29)
				{
					nDay = 1;
					nMonth += 1;
				}
			}
			break;
		case 3:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 4:
			if(nDay > 30)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 5:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 6:
			if(nDay > 30)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 7:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 8:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 9:
			if(nDay > 30)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 10:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 11:
			if(nDay > 30)
			{
				nDay = 1;
				nMonth += 1;
			}
			break;
		case 12:
			if(nDay > 31)
			{
				nDay = 1;
				nMonth = 1;
				nYear += 1;
			}
			break;
		default:
			throw std::runtime_error("datetime exception: format error");
			break;
		}
	}
}



