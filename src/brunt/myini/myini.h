#ifndef __BRUNT_MYINI_H__
#define __BRUNT_MYINI_H__

#ifdef _WIN32
	#pragma warning(disable:4786)
	#pragma warning(disable:4503)
#endif

#include <string>
#include <vector>
#include <set>
#include <iostream>

#include "ini.h"

namespace brunt
{
/*
ini file like this:
[index_1]
value_1=someword
value_2=someword
[index_2]
#comments for value_1
value_1=someword
#comments for value_2
#comments for value_2
#comments for value_2
value_2=someword
*/
struct ini_value
{
	/*
    #comments for value_1
	value_1=someword
	*/	
	std::string m_name;		// =value_1
	mutable std::string m_value;	// =someword
	mutable std::vector<std::string> m_comments;	// =comments for value_1
	ini_value(const char* name = "", const char* value = ""):m_name(name), m_value(value)
	{
	}
	friend std::ostream& operator <<(std::ostream& os, const ini_value& value);
};
std::ostream& operator <<(std::ostream& os, const ini_value& value);

//template<typename T>
//T& operator <<(T& os,const ini_value& value);

bool operator <(const ini_value& v1, const ini_value& v2);

struct ini_index
{
	std::string m_name;
	mutable std::set<ini_value> m_valueset;
	ini_index(const char* name):m_name(name)
	{
	}
	friend std::ostream& operator <<(std::ostream& os, const ini_index& index);
	
};
std::ostream& operator <<(std::ostream& os, const ini_index& index);
//template<typename T>
//T& operator <<(T& os,const ini_index& index);

bool operator <(const ini_index& v1, const ini_index& v2);

struct iniStripPred : public std::unary_function <char, bool>
{
		bool operator()(const char ch) const
		{
			if(ch == ' ' || ch =='\t' || ch =='\r')
				return true;
			return false;
		}
};

class CMyini : public ICMyini
{
	mutable std::set<ini_index> m_indexset;
	iniStripPred m_pred;
public:
	CMyini();
	virtual ~CMyini();
	bool load(const std::string& fileName)const;
	bool save(const std::string& fileName);
    
    bool read(const char *index, const char *valuename, std::string& value)const;
	bool write(const char *index, const char *valuename, const char* value);
	bool readcomment(const char *index, const char *valuename, std::vector<std::string>& comments)const;
	bool writecomment(const char *index, const char *valuename, std::vector<std::string>& comments);
	
	bool del(const char *index, const char *valuename = NULL);

private:	
	bool save();
	bool is_comment(std::string& str)const;
    bool is_index(std::string& str)const;
	bool is_key(std::string& str, ini_value& value)const;

	const ini_value* findValue(const char *index, const char *valuename)const;
};
}
#endif 

