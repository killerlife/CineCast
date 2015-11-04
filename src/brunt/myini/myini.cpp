#include <fstream>
#include "myini.h"
#include "mystring.h"
#include "line_iterator.h"
#include <string.h>

//using namespace brunt;
using namespace std;

#ifdef _WIN32
#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
#endif

namespace brunt
{
	ICMyini* createMyini()
	{
		return new CMyini(); 
	}
	void releaseMyini(ICMyini* pMyini)
	{
		delete pMyini;
	}
	
	const char* commentchar = "#";
	bool operator<(const ini_value& v1, const ini_value& v2)
	{
		return v1.m_name<v2.m_name;
	}
	std::ostream& operator << (std::ostream& os, const ini_value& value)
	{
		for(unsigned int i = 0; i < value.m_comments.size(); ++i)
			os << "   " << commentchar << value.m_comments[i] << endl;
		os << "   " <<value.m_name << "=" << value.m_value << endl;
		return os;
	}

	bool operator < (const ini_index& v1, const ini_index& v2)
	{
		return v1.m_name<v2.m_name;
	}
	ostream& operator << (ostream& os, const ini_index& index)
	{
		os << "[" << index.m_name << "]" << endl;
		set<ini_value>::iterator beg = index.m_valueset.begin();
		while(beg != index.m_valueset.end())
		{
			os << *beg;
			++beg;
		}
		os << endl;
		return os;
	}
    
	

	CMyini::CMyini()
	{
	}
	
	CMyini::~CMyini()
	{
	}
	
	bool CMyini::load(const std::string& fileName)const
	{
		m_indexset.clear();
		fstream f(fileName.c_str(), ios::in);
		if(!f.is_open())
			return false;
		line_iterator ifile(f), eos;
		vector<string> lines(ifile, eos);
		copy(ifile, eos, inserter(lines, lines.end()));
		
		pair<set<ini_index>::iterator, bool> currentindex;
		ini_value curvalue;
		
		for(unsigned int i = 0; i < lines.size(); ++i)
		{
			
			mystring::stripEx(lines[i],m_pred);
			if(lines[i].length() == 0)
				continue;
			if(is_comment(lines[i]))
			{
				curvalue.m_comments.push_back(lines[i]);
				continue;
			}
			else if(is_index(lines[i])) //is a index line
			{
				currentindex = m_indexset.insert(ini_index(lines[i].c_str()));
			}
			else if(currentindex.second && is_key(lines[i], curvalue)) //key line 
			{
				const ini_index& index = *currentindex.first;
				index.m_valueset.insert(curvalue);
			}
			curvalue.m_comments.clear();
		}
		return true;
	}
	
	bool CMyini::save(const std::string& fileName)
	{
		try
		{
			fstream f(fileName.c_str(), ios::out);
			if(f.is_open())
			{
				copy(m_indexset.begin(), m_indexset.end(), ostream_iterator<ini_index>(f));
				f.flush();
				f.close();
				
#ifdef SYNC_INI_FILE				
				system("sync");
#endif
				return true;
			}
		}
		catch(...)
		{

		}
		return false;
	}
	bool CMyini::read(const char *index, const char *valuename, string& value)const
	{
		if(!index || !valuename)
			return false;
		const ini_value* pvalue = findValue(index, valuename);
		if(pvalue)
		{
			value = pvalue->m_value;
		}
		return pvalue != NULL;
	}
	bool CMyini::write(const char *index, const char *valuename, const char* value)
	{
		if(!index || !valuename || !value)
			return false;
		set<ini_index>::iterator it_index = m_indexset.find(ini_index(index));
		if(it_index == m_indexset.end())
		{
			//add index and value;
			pair<set<ini_index>::iterator,bool> addpair = m_indexset.insert(ini_index(index));
			if(addpair.second)
				addpair.first->m_valueset.insert(ini_value(valuename,value));
		}
		else
		{
			set<ini_value>::iterator it_value = it_index->m_valueset.find(ini_value(valuename));
			if(it_value == it_index->m_valueset.end())
			{
				//add value;
				it_index->m_valueset.insert(ini_value(valuename,value));
			}	
			else
			{
				//modify value;
				it_value->m_value=value;
			}
		}
		return true;
	}
	
	bool CMyini::readcomment(const char *index, const char *valuename, std::vector<std::string>& comments)const
	{
		if(!index || !valuename)
			return false;
		const ini_value* pvalue = findValue(index, valuename);
		if(pvalue)
		{
			comments = pvalue->m_comments;
		}
		return pvalue != NULL;
	}
	bool CMyini::writecomment(const char *index, const char *valuename, std::vector<std::string>& comments)
	{
		if(!index || !valuename)
			return false;
		const ini_value* pvalue = findValue(index, valuename);
		if(pvalue)
		{
			pvalue->m_comments = comments;
		}
		return pvalue != NULL;
	}
	bool CMyini::del(const char *index, const char *valuename)
	{
		if(!index)
			return false;
		set<ini_index>::iterator it_index = m_indexset.find(ini_index(index));
		if(it_index == m_indexset.end())
			return false;
		if(!valuename)
			m_indexset.erase(it_index);
		else
		{
			set<ini_value>::iterator it_value = it_index->m_valueset.find(ini_value(valuename));
			if(it_value != it_index->m_valueset.end())
			{
				it_index->m_valueset.erase(it_value);
			}
			else 
				return false;
		}
		return true;
	}
	/////////////////////////////////////////////////////////////////////////////
	bool CMyini::is_comment(string& str)const
	{
		if(str.find(commentchar) == 0)
		{
			str = str.substr(strlen(commentchar));
			return true;
		}
		return false;
	}
	bool CMyini::is_index(string& str)const
	{
		string key = mystring::findStrBetween(str, "[", "]");
		if(key.length() > 0)
		{
			str = key;
			mystring::stripEx(str,m_pred);
			return true;
		}
		return false;
	}
	bool CMyini::is_key(string& str, ini_value& value)const
	{
		vector<string> strs;
		mystring::separateString(str, '=', strs, 2);
		if(strs.size() >= 2)
		{
			value.m_name = strs[0];
			value.m_value = strs[1];
			mystring::stripEx(value.m_name,m_pred);
			mystring::stripEx(value.m_value,m_pred);
			return true;
		}
		return false;
	}
	const ini_value* CMyini::findValue(const char *index, const char *valuename)const
	{
		set<ini_index>::iterator it_index = m_indexset.find(ini_index(index));
		if(it_index == m_indexset.end())
			return NULL;
		set<ini_value>::iterator it_value = it_index->m_valueset.find(ini_value(valuename));
		if(it_value == it_index->m_valueset.end())
			return NULL;
		return &(*it_value);
	}
}



