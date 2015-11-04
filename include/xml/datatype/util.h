//////////////////////////////////////////////////////////////////////////
//
#if!defined __UTIL2006_H_
#define __UTIL2006_H_

namespace _xmlDataType
{
struct general_func
{
	static int findstringpos(const char* strori, const char* strfind)
	{
		char* strpos = (char*)strstr(strori,strfind);
		if(!strpos)
			return -1;
		return int(strpos-strori);
	}
	
	static int copystring(char* szDes,const char* szOri,int nCopylen)
	{
		if((int)strlen(szOri) < nCopylen)
			return 0;
		memcpy(szDes,szOri,nCopylen);
		szDes[nCopylen] = 0;
		return nCopylen;
	}
	
	static long GetIntegerValue(const char* szOri,int nStartPos,int nLen)
	{
		if(nLen >= 20)
			return -1;
		char sztmp[20];
		copystring(sztmp,&szOri[nStartPos],nLen);
		return atol(sztmp);
	}
};
};

#endif	//__UTIL2006_H_

