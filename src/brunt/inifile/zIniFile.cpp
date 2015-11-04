#include "stdafx.h"
#include "zIniFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_BUFFSIZE	512

#ifdef _WIN32
#define snprintf	_snprintf
#endif

CZIniFile::CZIniFile(const char* lpszFile)
{
	memset(m_szError, 0 , sizeof(m_szError));
	if(lpszFile)
		Load(lpszFile);
}

CZIniFile::~CZIniFile(void)
{
}

#define THROW_ERROR(s)  {snprintf(m_szError, ERR_MAXSIZE, "Parse file %s error: in line %d: %s", m_filename.c_str(), linecount, s); throw -1;}

bool CZIniFile::Parse(FILE* fp)
{
	assert(fp);

	char szBuff[MAX_BUFFSIZE+1];
	int linecount = 1;
	string secname;
	CSectionItemMap::iterator it_secitem = NULL;
	CSectionKeyMap::iterator it_seckey = NULL;

	try
	{
		// read line loop
		while(!feof(fp) && strlen(m_szError)==0)
		{
			string line;
			szBuff[MAX_BUFFSIZE] = 0;
			do{
				memset(szBuff, 0, sizeof(szBuff));
				if(!fgets(szBuff, MAX_BUFFSIZE, fp))
				{
					if(!feof(fp))
						THROW_ERROR("fgets reading file error");
				}
				line += string(szBuff);
				if(!(strlen(szBuff)==MAX_BUFFSIZE && 
						szBuff[MAX_BUFFSIZE-1]!='\n' && szBuff[MAX_BUFFSIZE-1]!='\r'))
				{
					break;
				}
			}while(1);

			if(strlen(m_szError))
				break;

			// skip pre-space and last-return
			size_t sp = line.find_last_of("\n\r");
			if(sp!=string::npos)
				line.erase(sp);
			sp = line.find_first_not_of("\t ");
			if(sp!=string::npos)
				line.erase(0, sp);
			else if(line.find_first_of("\t ")!=string::npos)		
				continue;	// space or tab line , skip
			// erase note content following '#'
			sp = line.find_first_of("#");
			if(sp!=string::npos)
				line.erase(sp);

			// skip null line
			if(line.empty())
				continue;

			if(line[0]=='<')			// section items
			{
				size_t n = line.find_first_of('>');
				if(n==string::npos)
				{
					THROW_ERROR("losting '>'");
				}
				else
				{
				//	vector<string> items;
					// get section name
					secname = line.substr(1, n-1);
					if(secname.empty())
						THROW_ERROR("null section name");
					it_secitem = m_secitems.find(secname);
					if(it_secitem != m_secitems.end())
						THROW_ERROR("the section redefined");
					// insert new section
					it_secitem = m_secitems.insert(it_secitem, CSectionItemPair(secname, vector<string>()));
					it_seckey = NULL;
				}
			}
			else if(line[0]=='[')		// section keys
			{
				size_t n = line.find_first_of(']');
				if(n==string::npos)
				{
					THROW_ERROR("losting ']'");
				}
				else
				{
					vector<string> items;
					secname = line.substr(1, n-1);
					if(secname.empty())
						THROW_ERROR("null section name");
					it_seckey = m_seckeys.find(secname);
					if(it_seckey != m_seckeys.end())
						THROW_ERROR("the section redefined");
					// insert new section
					it_seckey = m_seckeys.insert(it_seckey, CSectionKeyPair(secname, CKeyValueMap()));
					it_secitem = NULL;
				}
			}
			else
			{
				if(it_secitem==NULL && it_seckey==NULL)
					THROW_ERROR("no section defined");
				if(it_secitem!=NULL)	// insert item
				{
					assert(it_secitem!=m_secitems.end());
					it_secitem->second.push_back(line);
				}
				if(it_seckey!=NULL)	// insert key
				{
					assert(it_seckey!=m_seckeys.end());
					CKeyValueMap& keys = it_seckey->second;
					// parse key & value
					const string& s = line;
					size_t p1 = s.find_first_of('=');
					if(p1==string::npos)
						continue;
					size_t e1 = s.find_last_not_of(" \t", p1-1);
					if(e1==string::npos)
						continue;
					// get the key name & check
					string k = s.substr(0, e1+1);
					// get the value
					size_t p2 = s.find_first_not_of(" \t", p1+1);
					size_t n2 = s.find_last_not_of(" \t");
					string v = p2==string::npos?"":s.substr(p2, n2-p2+1);
					// check the same key name 
					if(keys.find(k)!=keys.end())
						THROW_ERROR("the key redefined");
					keys.insert(pair<string,string>(k, v));
				}
			}

			linecount++;
		}
	}
	catch(int&)
	{
		return false;
	}

	return true;
}

bool CZIniFile::Load(const char* lpszFile)
{
	bool result = true;
	m_szError[0] = 0;

	// clean
	m_secitems.clear();
	m_seckeys.clear();

	if(lpszFile)
	{
		m_filename = lpszFile;
		FILE* fp = fopen(lpszFile, "rt");
		if(!fp) return false;

		result = Parse(fp);

		fclose(fp);
	}

	return result;
}

bool CZIniFile::Save(const char* lpszFile)
{
	m_szError[0] = 0;
	string line;

	if(lpszFile)
		m_filename = lpszFile;
	FILE* fp = fopen(m_filename.c_str(), "r+t");
	if(!fp) return false;

	CSectionItemMap::iterator it_sec = m_secitems.begin();
	for(; it_sec!=m_secitems.end(); it_sec++)
	{
		fprintf(fp, "<%s>\n", it_sec->first.c_str());
		for(CStringVector::iterator it = it_sec->second.begin(); it!=it_sec->second.end(); it++)
		{
			fprintf(fp, "%s\n", it->c_str());
		}
		fprintf(fp, "\n");
	}

	CSectionKeyMap::iterator it_key = m_seckeys.begin();
	for(; it_key!=m_seckeys.end(); it_key++)
	{
		fprintf(fp, "[%s]\n", it_key->first.c_str());
		for(CKeyValueMap::iterator it = it_key->second.begin(); it!=it_key->second.end(); it++)
		{
			fprintf(fp, "%s=%s\n", it->first.c_str(), it->second.c_str());
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return (strlen(m_szError)==0);
}

int CZIniFile::GetValue(const char* section, vector<string>& lines)
{
	CSectionItemMap::iterator it_sec = m_secitems.find(section);
	if(it_sec!=m_secitems.end())
	{
		lines = it_sec->second;
		return (int)lines.size();
	}
	return 0;
}

const char* CZIniFile::GetValue(const char* section, const char* key, const char* deftvalue)
{
	CSectionKeyMap::iterator it_seckey = m_seckeys.find(section);
	if(it_seckey!=m_seckeys.end())
	{
		CKeyValueMap& keys = it_seckey->second;
		CKeyValueMap::iterator it = keys.find(key);
		if(it!=keys.end())
			return it->second.c_str();
	}
	return deftvalue;
}

int CZIniFile::GetValue(const char* section, const char* key, int deftvalue)
{
	const char* pv = GetValue(section, key, (const char*)NULL);
	if(pv==NULL)
		return deftvalue;
	return atoi(pv);
}

void CZIniFile::SetValue(const char* section, const vector<string>& lines)
{
	if(!section || strlen(section)==0)
		return;
	CSectionItemMap::iterator it_secitem = m_secitems.find(section);
	if(it_secitem != m_secitems.end())
		m_secitems.erase(it_secitem);
	m_secitems.insert(CSectionItemPair(section, lines));
}

void CZIniFile::SetValue(const char* section, const char* key, const char* value)
{
	if(!section || strlen(section)==0)
		return;
	assert(key && value);
	CSectionKeyMap::iterator it_seckey = m_seckeys.find(section);
	if(it_seckey == m_seckeys.end())
	{
		CKeyValueMap null;
		it_seckey = m_seckeys.insert(it_seckey, CSectionKeyPair(section, null));
	}
	CKeyValueMap& keys = it_seckey->second;
	CKeyValueMap::iterator it = keys.find(key);
	if(it != keys.end())
		keys.erase(it);
	keys.insert(pair<string,string>(string(key), string(value)));
}

void CZIniFile::SetValue(const char* section, const char* key, int value)
{
	if(!section || strlen(section)==0)
		return;
	assert(key && section);
	char buff[100];
	snprintf(buff, 100, "%d", value);
	SetValue(section, key, buff);
}


