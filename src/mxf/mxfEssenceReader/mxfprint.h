#include <mxflib/mxflib.h>

void setDebugMode(bool flag);
class TaDumpDebugInfo : public mxflib::DumpDebugInfo
{
public:
	virtual void debug(const char * info);						
	virtual void warning(const char * info);					
	virtual void error(const char * info);						
	
};
