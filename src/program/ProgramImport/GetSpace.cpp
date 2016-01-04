#ifdef WIN32
#include "stdafx.h"
#endif
#include "GetSpace.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <iostream>
#include <config/config.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>
#else
#include <fstream>
#include <sys/vfs.h>
#endif
namespace fs = boost::filesystem;
using namespace std;
unsigned int CGetSpace::getFileSize(const std::vector<std::string>& files)
{
	boost::intmax_t fileSize = 0;
	std::vector<std::string>::const_iterator ite_file;
	for(ite_file = files.begin(); ite_file != files.end(); ++ite_file)
	{
		try
		{
			fs::path filePath(*ite_file);
			fileSize += fs::file_size(filePath);
		}
		catch(const fs::filesystem_error&)
		{
			return 0;
		}
	}
	
	return (unsigned int)(fileSize / (1024 * 1024));    
}

void CGetSpace::separateString(const string& str,char ch,vector<string>& str_vec,unsigned int separatenum)
{
	str_vec.resize(0);
	string::size_type pos = 0, prev_pos = 0;
    unsigned int i = 0;
	while((pos=str.find(ch,pos)) != string::npos && i < separatenum)
	{
		if(pos - prev_pos > 0)
			str_vec.push_back(str.substr(prev_pos, pos - prev_pos));
		prev_pos = ++pos;
		++i;
	}
	if(prev_pos != string::npos)
		str_vec.push_back(str.substr(prev_pos));
}

unsigned int CGetSpace::getAvalibleSpace(const std::string path)
{
#ifdef _WIN32
	ULARGE_INTEGER freeSize;
	ULARGE_INTEGER totalSize;
	ULARGE_INTEGER avalibleSize;
	BOOL result = GetDiskFreeSpaceEx(path.c_str(),\
									 (PULARGE_INTEGER)&freeSize,\
									 (PULARGE_INTEGER)&totalSize,\
									 (PULARGE_INTEGER)&avalibleSize);
	if(!result)
		return 0;
	return (unsigned int)((avalibleSize.QuadPart) / (1024 * 1024));
#else
	unsigned int freeSpace = 0;
	struct statfs s;
	statfs("/storage", &s);
	//statfs(getConfig().getProgramRootPath().c_str(), &s);
//	freeSpace = (s.f_bsize * ((unsigned long long)s.f_bfree))/1024/1024;
	freeSpace = (s.f_bsize * ((unsigned long long)s.f_bavail))/1024/1024;
	return freeSpace;
#endif
}

string CGetSpace::getSaveFile()
{
	string str = m_sSaveFile;
	m_sSaveFile = "";
	return str;
}

// Aavan: 2008-7-3
bool CGetSpace::copy_find_file(const std::string& dpath, std::vector<std::string>& childs, bool &isdms)
{
	bool dms = false;
	try
	{
		fs::path dir_path(dpath);
		if(!fs::exists(dir_path)){
			return false;
		}
		
		fs::directory_iterator end_itr;    // default construction yields past-the-end


		for(fs::directory_iterator itr(dir_path); itr != end_itr; ++itr)
		{
			
			if(fs::is_directory(*itr))
				copy_find_file((*itr).path().native(), childs, dms);
			else
			{ 
				int pos = dpath.length() + 1;
				string str(itr->path().native());
				string name =str.substr(pos); 
				
				cout << "ؼname = " << name  <<endl;

				if (name == "id")
				{
					dms = true;
					m_sSaveFile = itr->path().native();
					cout << "**ؼļ id   |" <<  name << "| sSave = "  << m_sSaveFile <<endl;
				}
				else if(name == "describe.ini")
				{
					m_sSaveFile = itr->path().native();
					cout << "** describe.ini   | name = " << name << "| sSave = "  << m_sSaveFile <<endl;
				}
				else 
				{
					childs.push_back(itr->path().native());				
				}
			}
		}

	}
	catch(const fs::filesystem_error& e)
	{
	}

	isdms = dms;
	return !childs.empty();
}
