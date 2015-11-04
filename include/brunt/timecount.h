#ifndef _TIMECOUNT_20080722_H__
#define _TIMECOUNT_20080722_H__

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif//_WIN32

#include <memory.h>

#ifndef _TC_LONG
#define _TC_LONG	long
#endif
	

//#define DPRINT_TV(s, tv)   printf("%s %ld,%ld\n", s, tv.tv_sec, tv.tv_usec);
#define DPRINT_TV(s, tv)

class CTimeCount
{
	long m_count;
//	_TC_LONG m_total;
	struct timeval m_tvStart;
	struct timeval m_tvTotal;
	int m_error;
	char m_text[256];
	
public:
	CTimeCount():m_count(0), m_error(0)//, m_total(0)
	{
		m_tvTotal.tv_sec = m_tvTotal.tv_usec = 0;
	}
	
	virtual int GetTime(struct timeval& tv)
	{
#ifdef _WIN32
		unsigned int t = GetTickCount();
		tv.tv_sec = t/1000;
		tv.tv_usec = (t%1000)*1000;
		return 0;
#else
		struct timezone tz;
		return gettimeofday(&tv, &tz);
#endif
	}
	virtual void SubTime(struct timeval& tv1, struct timeval& tv2)
	{
		DPRINT_TV("tv1:", tv1);
		DPRINT_TV("tv2:", tv2);
		struct timeval tv;
		if(tv1.tv_usec >= tv2.tv_usec) 
			tv.tv_usec = tv1.tv_usec - tv2.tv_usec;
		else {
			tv1.tv_sec -= 1;
			tv1.tv_usec += 1000L*1000L;
			tv.tv_usec = tv1.tv_usec - tv2.tv_usec;
		}	
		tv.tv_sec = tv1.tv_sec - tv2.tv_sec;
		tv1 = tv;
		DPRINT_TV("tv:", tv1);
	}

	int Start()
	{
		m_error = GetTime(m_tvStart);
		return m_error;
	}

	int Stop()
	{
		m_count++;
		if(m_error!=-1)
		{
			struct timeval tvEnd;
			m_error = GetTime(tvEnd);
			if(m_error==-1)
				return m_error;
			SubTime(tvEnd, m_tvStart);
//			m_total += tvEnd.tv_sec * 1000 * 1000;
//			m_total += tvEnd.tv_usec;
			m_tvTotal.tv_sec += tvEnd.tv_sec; 
			m_tvTotal.tv_usec += tvEnd.tv_usec; 
			m_tvTotal.tv_sec += m_tvTotal.tv_usec/1000000L;
			m_tvTotal.tv_usec %= 1000000L;
		}
		return m_error;
	}

	// get total ms
	long GetTotal()	
	{
		return m_tvTotal.tv_sec * 1000 + m_tvTotal.tv_usec / 1000;
	}

	long GetCount()
	{
		return m_count;
	}
	
	long GetAverage()
	{
		return GetTotal()/GetCount();
	}
	
	char* GetText()
	{
		sprintf(m_text, "count:%ld total:%ldms average:%ldms", GetCount(), GetTotal(), GetAverage());
		return m_text;
	}
};

#ifdef TC_DEBUG
#define TC_VARNAME(a)	__test_timecount[ a ]
#define TC_INIT(a)		CTimeCount TC_VARNAME(a)	
#define TC_STATIC_INIT(a) static CTimeCount TC_VARNAME(a)
#define TC_START(a)		TC_VARNAME(a).Start()
#define TC_STOP(a)		TC_VARNAME(a).Stop()
#define TC_GETTOTAL(a)	TC_VARNAME(a).GetTotal()
#define TC_GETTEXT(a)	TC_VARNAME(a).GetText()
#else//TC_DEBUG
#define TC_ARRAY_DEF(a)	
#define TC_INIT(a)		
#define TC_STATIC_INIT(a) 
#define TC_START(a)	
#define TC_STOP(a)	
#define TC_GETTOTAL(a)	1
#define TC_GETTEXT(a)	""
#endif

#endif//_TIMECOUNT_20080722_H__
