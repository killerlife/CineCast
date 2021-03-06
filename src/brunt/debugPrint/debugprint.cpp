// 修改于崔海军的DebugOutTrace
#ifdef WIN32
#include "stdafx.h"
#endif
#include "debugprint.h"
//#include "ini.h"
#include "zSocket.h"

#include "config/config.h"
#include "brunt/ini.h"
using namespace brunt;

#include <time.h>
#include <stdarg.h>   
#include <assert.h>

#define MYIP	"127.0.0.1"
#define MYPORT	8877
#define DPRINT_INI		"debugprint.ini"


class CMySocket
{
protected:
	CZSocket m_socket;
	int initok;

	int OpenSocket(CZSocket& socket, const char* addr, int port)
	{
		int error = 0;
		if(!addr) return -1;
		try
		{
			if(!socket.Create(AF_INET, SOCK_STREAM, 0))
				throw -1;
			struct sockaddr_in addr_in;
			memset(&addr_in, 0, sizeof(sockaddr_in));
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(port);
			addr_in.sin_addr.s_addr = inet_addr(addr);

			do
			{
				t_timeout tm = 3000;
				error = socket.Connect(&addr_in, &tm);
				if(error && error!=CZSocket::WAIT_R_TIMEOUT)
					throw -1;
			}while(error);

		}
		catch(int&)
		{
			char szError[100];
			socket.GetLastError(szError, 100);
			DP_PrintS("socket error: %s", szError);
		}

		return error;
	}
public:
	CMySocket():initok(-1){
		string path = getConfig().getBinRootPath();
		path += DPRINT_INI;
		string addr(MYIP);
		int port = MYPORT;

		ICMyini* ini = createMyini();
		if(ini->load(path))
		{
			string value;
			ini->read("output", "ip", addr);
			if(ini->read("output", "port", value))
				port = atoi(value.c_str());
		}
		releaseMyini(ini);

		initok = OpenSocket(m_socket, addr.c_str(), port);
		char buff[100];
		sprintf(buff, "Debug Print init ok.\n");
		Send(buff);
	}
	virtual ~CMySocket(){
		char buff[100];
		sprintf(buff, "Debug Print exit, Bye.\n");
		Send(buff);
		m_socket.Destroy();
	}
	bool Send(const char* buff){
		size_t sent_size = 0;
		return 0==m_socket.Send(buff, (int)strlen(buff), sent_size);
	}
} MySocket;


void DP_PrintF(FILE* fp, const char *fmt,...) 
{
	/* Format the variable length parameter list */
	int n, size = 256;
	char *p = NULL;
	va_list ap;

	if ((p = (char*)malloc (size)) == NULL)
		return;

	while (1) 
	{
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
#ifdef _WIN32
		n = _vsnprintf (p, size, fmt, ap);
#else
		n =  vsnprintf (p, size, fmt, ap);
#endif//_WIN32
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		char *p0 = p;
		if ((p = (char*)realloc (p, size)) == NULL)
		{
			free(p0);
			return;
		}
	}

	assert(p);

#ifdef _WIN32
	OutputDebugString(p);
#else
	fprintf(fp, p);
#endif

	MySocket.Send(p);

	free(p);
}

void DP_PrintS(const char *fmt,...) 
{
	/* Format the variable length parameter list */
	int n, size = 256;
	char *p = NULL;
	va_list ap;

	time_t timep;
	time(&timep);
	tm* pt = localtime(&timep);
	char buff[MAX_PATH];
#ifdef _WIN32
	_snprintf(buff, MAX_PATH, "%02d:%02d:%02d ",
			pt->tm_hour,
			pt->tm_min,
			pt->tm_sec
			);
	OutputDebugString(buff);
#else
	printf(buff);
#endif//_WIN32
	MySocket.Send(buff);

	if ((p = (char*)malloc (size)) == NULL)
		return;

	while (1) 
	{
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
#ifdef _WIN32
		n = _vsnprintf (p, size, fmt, ap);
#else
		n =  vsnprintf (p, size, fmt, ap);
#endif//_WIN32
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		char *p0 = p;
		if ((p = (char*)realloc (p, size)) == NULL)
		{
			free(p0);
			return;
		}
	}

	assert(p);

#ifdef _WIN32
	OutputDebugString(p);
	OutputDebugString("\n");
#else
	printf(p);
	printf("\n");
#endif

	MySocket.Send(p);
	MySocket.Send("\n");

	free(p);
}

class CDebugPrint: public IDebugPrint
{
public:
	CDebugPrint(int level = DP_All,  const char* filename = NULL);
	virtual ~CDebugPrint();
	
	/**
	 * @brief 设置信息级别
	 * @param lev 输出Info级别 
	 */
	virtual void SetPrintLevel(int lev);

	virtual int GetPrintLevel();

	/**
	 * @brief 写日志
	 * @param lev          信息级别
	 * @param moduleName   模块名称
	 * @param sFormat      输出参数(等同printf的第一个参数)   
	 * @param ...          参数列表(等同printf的参数列表，支持 %d%f%s)
	 */
	virtual void Print(int lev, const char* moduleName, const char *fmt, ...);
  
private:
	inline void Flush();

	bool	m_bPrintTime;
	FILE*	m_fp;
    int		m_level;   ///< 级别
//	CZSocket  m_socket;
	bool	m_bSocketReady;
};// end Logger


CDebugPrint::CDebugPrint(int level, const char* filename) : m_bPrintTime(true), m_fp(0), m_level(level)
{
	if(filename)
		m_fp = fopen(filename, "r+t");
	if(!m_fp)
		m_fp = stdout;

//	OpenSocket(m_socket, MYIP, MYPORT);
}

CDebugPrint::~CDebugPrint()
{
	if(m_fp!=stdout)
		fclose(m_fp);
}

void CDebugPrint::Flush()
{
	fflush(m_fp);
}

void CDebugPrint::SetPrintLevel(int lev)
{
	m_level = lev;
}

int CDebugPrint::GetPrintLevel()
{
	return m_level;
}

void CDebugPrint::Print(int lev, const char* moduleName, const char *fmt, ...)
{
	if(lev < m_level)
		return ;

#ifdef _WIN32
	char buff[MAX_PATH];
#endif//_WIN32

	// print time
	if(m_bPrintTime)
	{
		time_t timep;
		time(&timep);
		tm* p = localtime(&timep);
		fprintf(m_fp, "%02d:%02d:%02d ",
			p->tm_hour,
			p->tm_min,
			p->tm_sec
			);
#ifdef _WIN32
		_snprintf(buff, MAX_PATH, "%02d:%02d:%02d ",
			p->tm_hour,
			p->tm_min,
			p->tm_sec
			);
		OutputDebugString(buff);
#endif//_WIN32

		MySocket.Send(buff);
	}

	// print module name
	if(moduleName)
	{
		fprintf(m_fp, "%s ", moduleName);
#ifdef _WIN32
		OutputDebugString(moduleName);
		OutputDebugString(" ");
#endif//_WIN32

		MySocket.Send(moduleName);
		MySocket.Send(" ");
	}

	// print last content
	/* Format the variable length parameter list */
	int n, size = 256;
	char *p = NULL;
	va_list ap;

	if ((p = (char*)malloc (size)) == NULL)
		return;

	while (1) 
	{
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
#ifdef _WIN32
		n = _vsnprintf (p, size, fmt, ap);
#else
		n =  vsnprintf (p, size, fmt, ap);
#endif//_WIN32
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		char *p0 = p;
		if ((p = (char*)realloc (p, size)) == NULL)
		{
			free(p0);
			return;
		}
	}

	assert(p);

#ifdef _WIN32
	OutputDebugString(p);
	OutputDebugString("\n");
#endif
	fprintf(m_fp, p);
	// print return 
	fprintf(m_fp, "\n");

	size_t sent = 0;
	MySocket.Send(p);
	MySocket.Send("\n");

	free(p);

	Flush();
}


IDebugPrint& getDebugPrint()
{
	static CDebugPrint _debugprint;
	return _debugprint;
}



