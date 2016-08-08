//#include "stdafx.h"
#include "zSocket.h"

extern "C" {
//#include "io.h"
#include "socket.h"
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXITFUNC_ERR(err)	{return err;}

#ifdef _WIN32
#define snprintf	_snprintf	
#endif


CZSocket::CZSocket(void):m_socket(SOCKET_INVALID), m_error(0)
{
	m_bInitOK = 0;
	m_nRefCount = 0;
	if(!m_bInitOK && !(m_bInitOK = socket_open()))
	{
		m_error = ZSE_SOCKETINIT;
		return;
	}
	m_nRefCount++;
}

CZSocket::~CZSocket(void)
{
	Destroy();
	assert(m_nRefCount>0);
	if(--m_nRefCount==0)
		socket_close();
}

bool CZSocket::Create(int domain, int type, int protocol)
{
	if(!m_bInitOK) return false;
	if(m_socket!=SOCKET_INVALID)
	{
		m_error = ZSE_HASBEENOPEND;		
		return false;
	}
	m_error = socket_create(&m_socket, domain, type, protocol);
	return ZSOCKET_SUCCEED(m_error);
}

void CZSocket::Destroy()
{
	if(m_socket!=SOCKET_INVALID)
	{
		socket_destroy(&m_socket);
		m_socket = SOCKET_INVALID;
	}
}

bool CZSocket::IsOpened()
{
	return m_socket!=SOCKET_INVALID;
}

bool CZSocket::Bind(const sockaddr_in *addr_in)
{
	assert(sizeof(sockaddr_in) == sizeof(SA));
	memcpy(&m_addrin, addr_in, sizeof(sockaddr_in));
	m_error = socket_bind(&m_socket, (SA*)addr_in, sizeof(SA));
	return ZSOCKET_SUCCEED(m_error);
}

bool CZSocket::Listen(int backlog)
{
	m_error = socket_listen(&m_socket, backlog);
	return ZSOCKET_SUCCEED(m_error);
}

int CZSocket::Accept(CZSocket& as, char* address, int* port, t_timeout* tm)
{
	assert(!as.IsOpened());
	SA addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	m_error = socket_accept(&m_socket, &as.m_socket, &addr, &len, tm);
	if(ZSOCKET_FAILED(m_error))
		return m_error;
	memcpy(&as.m_addrin, &addr, sizeof(as.m_addrin));
	sockaddr_in* p = (sockaddr_in*)&addr;
	if(port) *port = ntohs(p->sin_port);
	if(address) strcpy(address, inet_ntoa(p->sin_addr));
	return m_error;
}

int CZSocket::Connect(const sockaddr_in *addr_in, t_timeout* tm)
{
	m_error = socket_connect(&m_socket, (SA*)addr_in, sizeof(sockaddr_in), tm);
	return m_error;
}

int CZSocket::Send(const char* buff, int size, size_t& sent, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_sent = 0;
	socket_setblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_send(&m_socket, buff + s_sent, s, &sent, tm);
		s -= sent;
		s_sent += sent;
		sent = s_sent;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return m_error;
}

int CZSocket::Send2(const char* buff, int size, size_t& sent, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_sent = 0;
	socket_setnonblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_send(&m_socket, buff + s_sent, s, &sent, tm);
		s -= sent;
		s_sent += sent;
		sent = s_sent;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return m_error;
}

int CZSocket::Receive(char* buff, int size, size_t& getsize, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_get = 0;
	socket_setblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_recv(&m_socket, buff + s_get, s, &getsize, tm);
		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	getsize = size;
	return m_error;
}

int CZSocket::Receive2(char* buff, int size, size_t& getsize, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_get = 0;
	socket_setnonblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_recv(&m_socket, buff + s_get, s, &getsize, tm);
		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	getsize = size;
	return m_error;
}

int CZSocket::SendTo(const char* buff, int size, size_t& sent, const sockaddr_in *addr_in, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	socklen_t len = sizeof(sockaddr_in);
	size_t s = size;
	size_t s_sent = 0;
	while(s > 0)
	{
		m_error = socket_sendto(&m_socket, buff + s_sent, s, &sent, (SA*)addr_in, len, tm);
		s -= sent;
		s_sent += sent;
		sent = s_sent;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return 0;
}

int CZSocket::ReceiveFrom(char* buff, int size, size_t& getsize, const sockaddr_in *addr_in, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	socklen_t len = sizeof(sockaddr_in);
	size_t s = size;
	size_t s_get = 0;
	while(s > 0)
	{
		m_error = socket_recvfrom(&m_socket, buff+s_get, s, &getsize, (SA*)addr_in, &len, tm);
		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return m_error;
}

void CZSocket::SetBlocking(bool bBlock)
{
	if (bBlock)
		socket_setblocking(&m_socket);
	else
		socket_setnonblocking(&m_socket);
//     u_long argp = !bBlock;
// 	ioctlsocket(m_socket, FIONBIO, &argp);}

int CZSocket::Wait(int type, t_timeout* tm)
{
	return socket_waitfd(&m_socket, type, tm);
}

int CZSocket::GetLastError(char* buff, int bufsize)
{
	if(buff)
	{
		const char* s = 0;
		if(ZSOCKET_SUCCEED(m_error))
		{
			s = "succeeded";
		}
		else if(ZSE_TEST(m_error))
		{
			switch(m_error)
			{
			case ZSE_SOCKETINIT:
				s = "initialize socket failed";
				break;
			case ZSE_HASBEENOPEND:
				s = "the socket has been opend";
				break;
			case ZSE_INETADDR:
				s = "inet_addr failed";
				break;
			default:
				s = "unknown";
			}
		}
		else
		{
			s = socket_strerror(m_error);
		}
		snprintf(buff, bufsize, "zSocket: %s", s);
	}
	return m_error;
}

std::string CZSocket::GetSockName()
{
	m_sockName = socket_name(&m_socket);
	return m_sockName;
}