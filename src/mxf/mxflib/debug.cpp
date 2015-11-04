#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mxflib/mxflib.h>
// Debug and error messages
#include <stdarg.h>
using namespace mxflib;

static DumpDebugInfo* pDumpDebugInfo=NULL;
//baiqian add:
void DumpDebugInfo::debug(const char * info)
{
	printf(info);
}
void DumpDebugInfo::warning(const char * info)
{
	printf(info);
}					
void DumpDebugInfo::error(const char * info)
{
	printf(info);
}						

void mxflib::setDumpDebugInfo(DumpDebugInfo* dump)
{
	pDumpDebugInfo=dump;
}
#ifdef MXFLIB_DEBUG
//! Display a general debug message
void mxflib::debug(const char *Fmt, ...)
{

	if(pDumpDebugInfo)
	{
		va_list args;
		char buf[2048];
		va_start(args, Fmt);
		vsprintf(buf,Fmt, args);
		va_end(args);
		pDumpDebugInfo->debug(buf);
	}
}
#endif // MXFLIB_DEBUG

//! Display a warning message
void mxflib::warning(const char *Fmt, ...)
{
	if(pDumpDebugInfo)
	{
		va_list args;
		char buf[2048];
		va_start(args, Fmt);
		vsprintf(buf,Fmt, args);
		va_end(args);
		pDumpDebugInfo->warning(buf);
	}
}

//! Display an error message
void mxflib::error(const char *Fmt, ...)
{
	if(pDumpDebugInfo)
	{
		va_list args;
		char buf[2048];
		va_start(args, Fmt);
		vsprintf(buf,Fmt, args);
		va_end(args);
		pDumpDebugInfo->error(buf);
	}
}
