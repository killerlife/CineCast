#ifndef __BRUNT_IMYINI_H__
#define __BRUNT_IMYINI_H__

#ifdef _WIN32
	#pragma warning(disable:4786)
	#pragma warning(disable:4503)
#endif

#include <string>
#include <vector>
#include <sstream>
#ifdef _WIN32

#ifdef INI_EXPORTS
#define INI_API __declspec(dllexport)
#else
#define INI_API __declspec(dllimport)
#endif

#else 
#define INI_API
#endif

namespace brunt
{
	class INI_API ICMyini 
	{
		public:
			virtual ~ICMyini(){};
			virtual  bool load(const std::string& fileName)const = 0;
			virtual  bool save(const std::string& fileName) = 0;
		    
		    virtual bool read(const char *index, const char *valuename , std::string& value)const = 0;
			virtual bool write(const char *index, const char *valuename , const char* value) = 0;
		
			virtual  bool readcomment(const char *index, const char *valuename, std::vector<std::string>& comments)const = 0;
			virtual  bool writecomment(const char *index, const char *valuename, std::vector<std::string>& comments) = 0;
			
			virtual  bool del(const char *index, const char *valuename = NULL) = 0;
	};
	template< typename T>
	bool readIni( ICMyini& ini, const char *index, const char *valuename, T& value)
	{
			std::string strvalue;
			if(ini.read(index,valuename, strvalue))
			{
				std::istringstream ins(strvalue);
				ins >> value;
				return true;
			}
			return false;
	}
	template< typename T>
	bool writeIni(ICMyini& ini, const char *index, const char *valuename, const T& value)
	{
			std::ostringstream os;
			os << value;
			return ini.write(index, valuename , os.str().c_str());
	}
    INI_API ICMyini* createMyini();
	INI_API void releaseMyini(ICMyini*);
}
#endif 

