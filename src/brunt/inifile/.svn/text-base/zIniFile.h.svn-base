#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include <assert.h>

#define ERR_MAXSIZE		256

class CZIniFile
{
public:
	typedef vector<string> CStringVector;
	typedef pair<string, CStringVector> CSectionItemPair;
	typedef map<string, CStringVector> CSectionItemMap;

	typedef map<string, string> CKeyValueMap;
	typedef pair<string, CKeyValueMap> CSectionKeyPair;
	typedef map<string, CKeyValueMap> CSectionKeyMap;

public:
	bool Load(const char* lpszFile);
	bool Save(const char* lpszFile = 0);
	int GetValue(const char* section, vector<string>& lines);
	int GetValue(const char* section, const char* key, int deftvalue);
	const char* GetValue(const char* section, const char* key, const char* deftvalue=0);
	void SetValue(const char* section, const vector<string>& lines);
	void SetValue(const char* section, const char* key, int value);
	void SetValue(const char* section, const char* key, const char* value);
	const char* GetLastError(){return m_szError;};

	CZIniFile(const char* lpszFile = 0);
	virtual ~CZIniFile(void);

protected:
	bool Parse(FILE* fp);

	CSectionItemMap		m_secitems;
	CSectionKeyMap		m_seckeys;
	string				m_filename;		// ini file name
	char				m_szError[ERR_MAXSIZE];
};
