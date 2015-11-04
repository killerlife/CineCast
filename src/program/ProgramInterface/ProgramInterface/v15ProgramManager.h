#ifndef _V15_PROGRAM_MANAGER_H
#define _V15_PROGRAM_MANAGER_H

#include <ProgramManager.h>
#include <sstream>
#include <string>
#include <vector>
class CV15ProgramManager
{
public:
	CV15ProgramManager();
	~CV15ProgramManager();

	IProgramManager* getProgramManagerObject();

protected:

private:
	IProgramManager* m_programManager;
};
 



#endif



