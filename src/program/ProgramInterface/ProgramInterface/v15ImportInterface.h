#ifndef _V15_IMPORT_INTERFACE_H
#define _V15_IMPORT_INTERFACE_H

#include <ProgramImport.h>
#include <sstream>
#include <string>
#include <vector>
class CV15ImportInterface
{
public:
	CV15ImportInterface();
	~CV15ImportInterface();

	IProgramImport* getProgramImportObject();

protected:

private:
	IProgramImport* m_programImport;
};
 



#endif



