#ifndef _ZZSOCKET_H_20080606__
#define _ZZSOCKET_H_20080606__

#include <string>
using namespace std;

#include "socketdef.h"

// CZSocket error code define ->
#define ZSOCKET_ERROR_BEGIN		50000
#define ZSOCKET_ERROR_END		50100
#define ZSE_TEST(a)				(a>=ZSOCKET_ERROR_BEGIN && a<=ZSOCKET_ERROR_END)
#define ZSE_SET(a)				(ZSOCKET_ERROR_BEGIN+a)
#define ZSE_SOCKETINIT			ZSE_SET(1)
#define ZSE_HASBEENOPEND		ZSE_SET(2)		// has been opened
#define ZSE_INETADDR			ZSE_SET(3)		// inet_addr error
// <-

#define ZSOCKET_FAILED(a)		((a)!=0)
#define ZSOCKET_SUCCEED(a)		((a)==0)

class CZSocketExcption;

class CZSocket
{
public:
	// Wait type
	enum{WAIT_READ = 1, WAIT_WRITE = 2, WAIT_EXCEPTION = 4};
	// Wait return
	enum{WAIT_R_DONE=0, WAIT_R_TIMEOUT=-1, WAIT_R_CLOSED=-2, WAIT_R_UNKNOWN=-3};

	bool IsOpened();
	bool Create(int domain, int type, int protocol);
	void Destroy();
	bool Listen(int backlog);
	bool Bind(const sockaddr_in *addr_in);
	void SetBlocking(bool bBlock);
	int Accept(CZSocket& as, char* ip, int* port, t_timeout* tm = 0);
	int Connect(const sockaddr_in *addr_in, t_timeout* tm = 0);
	int Send(const char* buff, int size, size_t& sent, t_timeout* tm = 0);
	int Send2(const char* buff, int size, size_t& sent, t_timeout* tm = 0);
	int Receive(char* buff, int size, size_t& getsize, t_timeout* tm = 0);
	int Receive2(char* buff, int size, size_t& getsize, t_timeout* tm = 0);
	int SendTo(const char* buff, int size, size_t& sent, const sockaddr_in *addr_in, t_timeout* tm = 0);
	int ReceiveFrom( char* buff, int size, size_t& getsize, const sockaddr_in *addr_in, t_timeout* tm = 0);
	int Wait(int type, t_timeout* tm = 0);
	int GetLastError(char* buff = NULL, int bufsize = 0);
	int SetSocketOpt(int level, int optname, const void* optval, socklen_t optlen);

	std::string GetSockName();

protected:
	int			m_error;
	t_socket	m_socket;
	sockaddr_in	m_addrin;

	int  m_bInitOK;
	int  m_nRefCount;		// all socket objects used count 

	std::string m_sockName;

public:
	CZSocket(void);
	virtual ~CZSocket(void);
};

class CZSocketExcption
{
	int error;
	 
public:
	CZSocketExcption(int e):error(e)
	{};
	int GetError(){return error;};
	const string& GetErrorMsg()
	{
		static string s;
		return s;
	}
};

#endif//_ZZSOCKET_H_20080606__
