#ifndef __BAIQIAN_MYSTRING_H__
#define __BAIQIAN_MYSTRING_H__

#include <string>
#include <vector>
#if _MSC_VER > 1000     // VC++
#pragma warning( disable : 4786 )   // disable warning debug symbol > 255...
#endif // _MSC_VER > 1000

class mystring //:public std::string
{
public:
	
	static std::string findStrBetween(const std::string& str, const char* beforstr, const char* afterstr);
	static void	strip(std::string& s, char ch=' ');
	template <typename _Predicate>
	static void stripEx(std::string& s, _Predicate pred)
	{
		int i, j = s.size() - 1;  if (!s.size()) 
			return;
		for (i = 0; pred(s[i]);i++);
		if(i)
			s.erase(0, i); 
		j = s.size();
		if (!j)
			return;
		j--;
		for (i = j; i && pred(s[i]); i--);
		if (i != j) 
			s.erase(i + 1, (size_t) - 1);
	}
	
	static const std::string& modSingleQuotesToD(std::string& s);
	static const std::string& modDoubleQuotesToS(std::string& s);
	static void	toLowerCase(std::string& s);
	static void	toUpperCase(std::string& s);

	/*
	 getValue can treate with string like 
	 str="v1=123;v2=***;v3=***";
	 getValue(str,"v1",value);
	 assert(value=="123");
	*/ 
	static void getValue(const std::string& str, const char* findname, std::string& value, const char* endstr = ";");
	static void separateString(const std::string& str, char ch, std::vector<std::string>& str_vec, unsigned int separatenum = -1);
};
#endif
