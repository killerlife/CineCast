#ifndef _SOCKETDEF_H_20080606__
#define _SOCKETDEF_H_20080606__

// The content is moved from file socket.h
// -- liang 2008.06.06

/*=========================================================================*\
* Platform specific compatibilization
\*=========================================================================*/
#ifdef _WIN32
#include "wsocket.h"
#else
#include "usocket.h"
#endif

/*=========================================================================*\
* The connect and accept functions accept a timeout and their
* implementations are somewhat complicated. We chose to move
* the timeout control into this module for these functions in
* order to simplify the modules that use them. 
\*=========================================================================*/
#if 0		// deleted by liang 2008.06.06
#include "timeout.h"		
#else		// liang
typedef long t_timeout;
typedef t_timeout *  p_timeout;
#define TO_INFINITE				((t_timeout*)(-1))
#define timeout_iszero(tm)		((void*)tm==0)
#define timeout_infinite(tm)	((void*)tm==TO_INFINITE)
#endif		// liang

/* we are lazy... */
typedef struct sockaddr SA;


#endif//_SOCKETDEF_H_20080606__