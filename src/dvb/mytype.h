#ifndef _MYTYPE_H_
#define _MYTYPE_H_

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#ifndef NULL
#define NULL 0
#endif

#ifdef DEBUG
#define DPRINTF printf
#else
#define DPRINTF
#endif


enum FILTER_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};
 

#endif