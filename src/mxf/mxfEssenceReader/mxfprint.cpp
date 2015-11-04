#include "mxfprint.h"
using namespace mxflib;

static bool DebugMode = false;
void setDebugMode(bool flag)
{
	DebugMode=flag;
}
void TaDumpDebugInfo::debug(const char * info)
{
	if(DebugMode)
	{
		 DumpDebugInfo::debug(info);
	}
}				
void TaDumpDebugInfo::warning(const char * info)
{
	DumpDebugInfo::warning(info);
}					
void TaDumpDebugInfo::error(const char * info)				
{
	DumpDebugInfo::error(info);
}
