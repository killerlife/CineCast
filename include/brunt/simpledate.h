#ifndef _SIMPLEDATE_H_200811261554__
#define _SIMPLEDATE_H_200811261554__


class CSimpleDate
{
public:
	int nyear;
	int nmonth;
	int nday;
	int nhour;
	int nminute;
	int nsecond;

	CSimpleDate():nyear(1970),nmonth(1),nday(1),nhour(0), nminute(0), nsecond(0)
	{
		time_t timep;
		time(&timep);
		tm* p = localtime(&timep);
		nyear = p->tm_year+1900;
		nmonth = p->tm_mon+1;
		nday = p->tm_mday;
		nhour = p->tm_hour;
		nminute = p->tm_min;
		nsecond = p->tm_sec;
	}
	CSimpleDate(const char* sval)
	{
		if(strlen(sval) < 10)
		{
			*this = CSimpleDate();
			return;
		}
		nyear = (int)GetNumber(sval);
		nmonth = (int)GetNumber(sval,5,2);
		nday = (int)GetNumber(sval,8,2);
		nhour = (int)GetNumber(sval, 11,2);
		nminute = (int)GetNumber(sval,14,2);
		nsecond = (int)GetNumber(sval,17,2);
	}
	bool operator==(const CSimpleDate& date2) const
	{
		const CSimpleDate& date1 = *this;
		if(this==&date2) return true;
		if(date1.nyear==date2.nyear && 
				date1.nmonth == date2.nmonth &&
				date1.nday == date2.nday &&
				date1.nhour == date2.nhour &&
				date1.nminute == date2.nminute &&
				date1.nsecond == date2.nsecond)
			return true;
		return false;
	}
	bool operator > ( const CSimpleDate& date2) const
	{
		const CSimpleDate& date1 = *this;
		if(this==&date2) return false;
		if(date1.nyear < date2.nyear)
			return false;
		if(date1.nyear > date2.nyear)
			return true;
		if(date1.nmonth < date2.nmonth)
			return false;
		if(date1.nmonth > date2.nmonth)
			return true;
		if(date1.nday < date2.nday)
			return false;
		if(date1.nday > date2.nday)
			return true;
		if(date1.nhour < date2.nhour)
			return false;
		if(date1.nhour > date2.nhour)
			return true;
		if(date1.nminute < date2.nminute)
			return false;
		if(date1.nminute > date2.nminute)
			return true;
		if(date1.nminute < date2.nminute)
			return false;
		if(date1.nminute > date2.nminute)
			return true;
		return false;
	}
	bool operator < (const CSimpleDate& date2) const
	{
		const CSimpleDate& date1 = *this;
		if(this==&date2) return false;
		return date2 > date1;
	}
	bool operator<=(const CSimpleDate& date2) const
	{
		const CSimpleDate& date1 = *this;
		if(this==&date2) return true;
		return (date1<date2 || date1==date2);
	}
	bool operator>=(const CSimpleDate& date2) const
	{
		const CSimpleDate& date1 = *this;
		if(this==&date2) return true;
		return (date1> date2 || date1==date2);
	}
	std::string GetString()const
	{
		char szval[255];
		sprintf(szval,"%04d-%02d-%02d %02d:%02d:%02d",nyear,nmonth,nday, nhour, nminute, nsecond);
		return std::string(szval);
	}
	bool isNear(const CSimpleDate& date2) const
	{
		tm tm1;
		tm1.tm_year = nyear - 1900;
		tm1.tm_mon = nmonth - 1;
		tm1.tm_mday = nday;
		tm1.tm_hour = nhour;
		tm1.tm_min = nminute;
		tm1.tm_sec = nsecond;
		tm tm2;
		tm2.tm_year = date2.nyear - 1900;
		tm2.tm_mon = date2.nmonth - 1;
		tm2.tm_mday = date2.nday;
		tm2.tm_hour = date2.nhour;
		tm2.tm_min = date2.nminute;
		tm2.tm_sec = date2.nsecond;
		time_t t1 = mktime(&tm1);
		time_t t2 = mktime(&tm2);
		if((t2 - t1)  <= (5*3600*24))
			return true;
		else
			return false;
	}
protected:
	long GetNumber(const char* szbuf,int nStart=0,int nCount=4)
	{
		if(nCount >= 255)
			return 0;
		int nlen = (int)strlen(szbuf);
		if(nlen < (nStart + nCount))
			return 0;
		char sztmp[255];
		memcpy(sztmp,&szbuf[nStart],nCount);
		sztmp[nCount]=0;
		return atol(sztmp);
	}
};


#endif//
