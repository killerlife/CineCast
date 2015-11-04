#include "mystring.h"
#include <functional>
using namespace std;
string mystring::findStrBetween(const string& str, const char* beforstr, const char* afterstr)
{
    if(!beforstr || !afterstr)
		return "";
	string returnstr(str), finedstr(str), bstr(beforstr), astr(afterstr);
	mystring::toLowerCase(finedstr);
	mystring::toLowerCase(bstr);
	mystring::toLowerCase(astr);
	
	string::size_type pos;
	pos = finedstr.find(bstr);
	if(pos != string::npos)
	{
		finedstr.erase(0, pos + bstr.length());
		returnstr.erase(0, pos + bstr.length());
	}
	else return "";
	if(astr.length() > 0)
	{
		pos = finedstr.find(astr);
		if(pos != string::npos)
		{
			//finedstr.erase(pos);
			returnstr.erase(pos);
		}
		else 
			return "";
	}
	return returnstr;
}
void mystring::strip(string& s, char ch)
{
	int i, j = s.size() - 1;
	if (!s.size()) 
		return;

	for (i = 0; s[i] == ch; i++);
	if (i) 
		s.erase(0, i);
	j = s.size(); 

	if (!j) 
		return; 
	j--;
	
	for (i = j; i && s[i] == ch; i--);
	if (i != j) 
		s.erase(i+1,(size_t) - 1);
}

const string& mystring::modSingleQuotesToD(string& s)
{
  //单引号变为双引号
  for(unsigned int i = 0; i < s.size(); ++i)
  {
	  if(s[i] == '\'')
		  s[i] = '"';
  }
  return s;
}

const string& mystring::modDoubleQuotesToS(string& s)
{
	//双引号变为单引号
	for(unsigned int i = 0; i < s.size(); ++i)
	{
		if(s[i] == '"')
			s[i] = '\'';
	}
	return s;
}

void mystring::toLowerCase(string& s)
{
	//change to lowcase;
	//_strlwr((char*)s.c_str());

	for(unsigned int i = 0; i < s.size(); ++i)
	{
		if(s[i] >= 'A' && s[i] <= 'Z')
			s[i] -= ('A'-'a');
	}
	
}
void mystring::toUpperCase(string& s)
{
	//change to uppercase;
	//_strupr((char*)s.c_str());
	for(unsigned int i = 0; i < s.size(); ++i)
	{
		if(s[i] >= 'a' && s[i] <= 'z')
			s[i] += ('A'-'a');
	}
}

template <class T>
struct letterornumber : public unary_function <T, bool>
{
	bool operator()(const T& ch) const
	{
		if(ch >= 'A' && ch <= 'Z')
			return true;
		if(ch >= 'a' && ch <= 'z')
			return true;
		if(ch >= '0' && ch <= '9')
			return true;
		return false;
	}
};

void mystring::getValue(const string& str, const char* findname, string& value, const char* endstr)
{
	value = mystring::findStrBetween(str, findname, endstr);
	if(value.empty())
		value = mystring::findStrBetween(str, findname, "");
	stripEx(value, not1(letterornumber<char>()));
}
void mystring::separateString(const string& str, char ch, vector<string>& str_vec, unsigned int separatenum)
{
	str_vec.resize(0);
	string::size_type pos = 0, prev_pos = 0;
    unsigned int i = 0;
	while((pos = str.find(ch,pos)) != string::npos && i < separatenum)
	{
		str_vec.push_back(str.substr(prev_pos, pos - prev_pos));
		prev_pos = ++pos;
		++i;
	}
	if(prev_pos != string::npos)
		str_vec.push_back(str.substr(prev_pos));
}



