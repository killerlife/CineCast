#include <DcpQuery/dcpQuery.h>
#include <DcpParser/DcpParser.h>
#include <ErrorCode/dcpErrorCode.h>
#include <inifile.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
namespace fs = boost::filesystem;

//#include <FtpClient.h>
//#include <ini/ini.h>
//using namespace brunt;
//using namespace nsFTP;

#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

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

class CDcpQuery : public IDcpQuery
{
public:
	CDcpQuery():m_error(no_error), m_type(DISK_QUERY)//, m_ftp(NULL)
	{}
	virtual ~CDcpQuery();
	virtual bool open(QUERYTYPE type = DISK_QUERY);
	virtual bool getDcpList(std::vector<FilePath>& dcpList);
	virtual const std::string queryCplInfo(const std::string& cplId, unsigned infoType);
	virtual bool getRootPath(std::vector<std::string>& rootPath);
	virtual void close();
	virtual int getLastError() { return m_error; }
private:
	bool ftpOpen();
	bool diskOpen();
	int getDcp();
	int getFtpDcp();
	bool getConfigInfo();
	bool checkName();
private:
	int m_error;
	std::string m_configname;
	QUERYTYPE m_type;
//	CFTPClient* m_ftp;
	std::vector<std::string> m_rootPath;
	std::vector<FilePath> m_dcpList;
};

DCPQUERY_API IDcpQuery* createDcpQuery()
{
	return new CDcpQuery;
}

DCPQUERY_API void releaseDcpQuery(IDcpQuery* dcpQuery)
{
	delete dcpQuery;
}

bool query_find_file(const fs::path& dir_path, std::vector<std::string>& childs)  
{
	try
	{
		if(!fs::exists(dir_path))
			{
				return false;
			}
		
		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(dir_path); itr != end_itr; ++itr)
		{
			if(fs::is_directory(*itr)) 
			{
				string str = itr->path().native();
				childs.push_back(str);
				cout << "child path " << str << endl;
			}
		}
	}
	catch(const fs::filesystem_error&)
	{
		return false;
	}

	return true;
}

CDcpQuery::~CDcpQuery() 
{
	close();

#if 0
	if(m_ftp)
	{
		m_ftp->Logout();
		cout << "ftp logout!\n";
		delete m_ftp;
	}
#endif
}

void CDcpQuery::close()
{
	if(!m_rootPath.empty())
		m_rootPath.clear();
	if(!m_dcpList.empty())
		m_dcpList.clear();
}

bool CDcpQuery::ftpOpen()
{
	std::vector<std::string>::iterator ite_root = m_rootPath.begin();
	stable_sort(ite_root, m_rootPath.end(), LessThan());
	for(; ite_root != m_rootPath.end(); ++ite_root)
	{

		vector<string>::iterator ite = ite_root;
		++ite;
		for(; ite != m_rootPath.end(); ++ite)
		{
			if((*ite).find(*ite_root) != string::npos)
			{
				m_error = dcp_root_path_nesting;
				return false;
			}
		}
	}
	return true;
}

bool CDcpQuery::diskOpen()
{
	std::vector<std::string>::iterator ite_root = m_rootPath.begin();
	stable_sort(ite_root, m_rootPath.end(), LessThan());
	for(; ite_root != m_rootPath.end(); ++ite_root)
	{
		try
		{
			fs::path thepath(*ite_root);
			fs::is_directory(thepath);
		}
		catch(const fs::filesystem_error&)
		{
			m_error = dcpQuery_open_error;
			return false;
		}
			
		vector<string>::iterator ite = ite_root;
		++ite;
		for(; ite != m_rootPath.end(); ++ite)
		{
			if((*ite).find(*ite_root) != string::npos)
			{
				m_error = dcp_root_path_nesting;
				return false;
			}
		}
	}
	return true;
}

bool CDcpQuery::getConfigInfo()
{
//	ICMyini* pIniFile = createMyini();
	CZIniFile ini;
	if(!ini.Load(m_configname.c_str()))
		return false;

	{ 
		int num = 0;
		std::string key;

#if 0
		if(m_type == FTP_QUERY)
		{
			m_ftp = new CFTPClient();
			key = "FTPDCP";
			std::string strHostname, strUsername, strPassword;
			pIniFile->read("RemoteServer", "host", strHostname);
			pIniFile->read("RemoteServer", "username", strUsername);
			pIniFile->read("RemoteServer", "password", strPassword);
			nsFTP::CLogonInfo li(strHostname, nsFTP::DEFAULT_FTP_PORT, strUsername, strPassword);
			if(!m_ftp->Login(li))
			{
				m_error = ftp_login_error;
				return false;
			}
			cout << "login successful!\n";
		}
#endif

		if(m_type == DISK_QUERY)
			key = "DCP";
		else if(m_type == USB_QUERY)
			key = "USBDCP";

//		readIni(*pIniFile, key.c_str(), "storagePathNum", num);
		num = ini.GetValue(key.c_str(), "storagePathNum", 0);
		for(int i = 0; i < num; ++i)
		{
			string path;
			std::ostringstream os;
			os << "storagePath_" << i;
//			pIniFile->read(key.c_str(), os.str().c_str(), path);
			path = ini.GetValue(key.c_str(), os.str().c_str(), "");
			if(!path.empty())
				m_rootPath.push_back(path);
		}
	}

	if(m_rootPath.empty())
	{
		m_error = invalid_root_path_error;
		return false;
	}

	return true;
}

bool CDcpQuery::open(QUERYTYPE type)
{
	m_type = type;
	close();
#ifdef WIN32
	m_configname = "config.dat";
#else
	m_configname = ".config.dat";
#endif
	if(!getConfigInfo())
		return false;
	if(m_type == DISK_QUERY || m_type == USB_QUERY)
		return diskOpen();
	else if(m_type == FTP_QUERY)
		return ftpOpen();
	return false;
}

bool CDcpQuery::getRootPath(std::vector<std::string>& rootPath)
{
	rootPath = m_rootPath;
	return true;
}

class CDcpParserRef
{
public:
	 CDcpParserRef():m_dcpParser(NULL)
	 {
		m_dcpParser = CreateDcpParser();
	 }
	 ~CDcpParserRef()
	 {
		 if(m_dcpParser)
			ReleaseDcpParser(&m_dcpParser);
	 }
	 IDcpParser* operator->()
	 {
		return m_dcpParser;
	 }
	 bool isValid()
	 {
		return m_dcpParser != NULL;
	 }
private:
	IDcpParser* m_dcpParser;
};

const std::string CDcpQuery::queryCplInfo(const std::string& cplId, unsigned int infoType)
{
	std::vector<FilePath> dcpList;
	if(!getDcpList(dcpList))
		return "";

	std::vector<FilePath>::iterator ite = dcpList.begin();
	for(; ite != dcpList.end(); ++ite)
	{
		CDcpParserRef dcpParser;
		if(!dcpParser.isValid())
		{
			m_error = get_dcpParser_object_error;
			return "";
		}
		if(!dcpParser->open((*ite).path))
		{
			//m_error = dcpParser->getLastError();
			//return "";
			continue;
		}

		for (int pklIndex = 0; pklIndex < dcpParser->packingListCount(); pklIndex++)
		{
			int cplCount = dcpParser->compositionCount(pklIndex);
			if(cplCount < 1)
			{
				//m_error = get_cpl_uuid_error;
				continue;
			}

			for(int count = 0; count < cplCount; count++)
			{
				std::string uuid;
				const CCompositionPlayList* cplList = dcpParser->composition(pklIndex, count);
				if(!cplList)
				{
					m_error = dcpParser->getLastError();
					return "";
				}

				uuid = cplList->GetUUIDElement();
				if(uuid == cplId)
				{
					if(infoType == COMPOSITION_FILE)
						return dcpParser->compositionFile(pklIndex, count);
					if(infoType == PACKINGLIST_FILE)
						return dcpParser->packingListFile(pklIndex);
					if(infoType == DCP_PATH)
						return (*ite).path;
				}
			}
		}
	}
	m_error = get_dcp_list_error;
	return "";
}

int CDcpQuery::getFtpDcp()
{
#if 0
	vector<string>::iterator ite_path = m_rootPath.begin();
	m_dcpList.clear();

	for(; ite_path != m_rootPath.end(); ++ite_path)
	{
		nsFTP::TSpFTPFileStatusVector list;
		if(!m_ftp->List((*ite_path).c_str(), list))
		{
			m_error = dcpQuery_getDcp_error;
			return m_error;
		}
		
		nsFTP::TSpFTPFileStatusVector::iterator ite = list.begin();
		for(; ite != list.end(); ++ite)
		{
			if((*ite)->Name() != "." && (*ite)->Name() != "..")
			{

				if((*ite)->IsCwdPossible())
				{
					FilePath filePath;
					filePath.name = (*ite)->Name();
					filePath.path = (*ite)->Path() + "/" + (*ite)->Name();
					m_dcpList.push_back(filePath);
				}
			}
		}
	}
#endif
	return no_error;
}

bool CDcpQuery::getDcpList(std::vector<FilePath>& dcpList)
{
	dcpList = m_dcpList;
	if(m_type == FTP_QUERY)
	{
		if(getFtpDcp() < 0)
			return false;
	}
	else if(m_type == DISK_QUERY || m_type == USB_QUERY)
	{
		if(getDcp() < 0)
			return false;
	}

	if(!checkName())
		return false;
	dcpList = m_dcpList;
	return true;
}

bool CDcpQuery::checkName()
{
	if(m_dcpList.empty())
		return false;
	vector<FilePath>::iterator ite_name = m_dcpList.begin();
	for(; ite_name != m_dcpList.end(); ++ite_name)
	{
		vector<FilePath>::iterator ite = ite_name;
		++ite;
		for(; ite != m_dcpList.end(); ++ite)
		{
			if((*ite).name != "" && (*ite_name).name == (*ite).name)
			{
				m_error = exist_same_dcp_name;
				return false;
			}
		}
	}
	return true;
}

int CDcpQuery::getDcp()
{
	vector<string>::iterator ite_path = m_rootPath.begin();
	m_dcpList.clear();
	try
	{
		for(; ite_path != m_rootPath.end(); ++ite_path)
		{
			fs::path thepath(*ite_path);
			vector<string> childFiles;
			FilePath filePath;
			if(query_find_file(thepath, childFiles))
			{
				childFiles.push_back(thepath.native());
				vector<string>::iterator ite_child = childFiles.begin();
				for(; ite_child != childFiles.end(); ++ite_child)
				{
					IDcpParser* dcpParser = CreateDcpParser();
					if(dcpParser == NULL)
					{
						m_error = get_dcpParser_object_error;
						return m_error;
					}
					try
					{
						if(dcpParser->open(*ite_child))
						{
							std::string tempstr;
							std::string uuid;
							int pklCount = dcpParser->packingListCount();
							for (int i = 0; i < pklCount; i++)
							{
								int cplCount = dcpParser->compositionCount(i);
								if(cplCount < 1)
								{
									m_error = get_cpl_uuid_error;
								}

								for(int count = 0; count < cplCount; count++)
								{
									const CCompositionPlayList* cplList = dcpParser->composition(i, count);
									if(!cplList)
									{
										m_error = dcpParser->getLastError();
										break;
									}
									uuid = cplList->GetUUIDElement();
									tempstr = (cplList->GetContentTitleTextElement()).strValue;
									if(!tempstr.empty())
									{
										filePath.name = tempstr;
										filePath.path = *ite_child;
										m_dcpList.push_back(filePath);
										break;
									}
								}
							}
						}
					}
					catch(const fs::filesystem_error&)
					{
					}
					ReleaseDcpParser(&dcpParser);
				}
			}
		}
	}
	catch(const fs::filesystem_error& e)
	{
		cout << "error " << e.what() << endl;
		m_error = dcpQuery_getDcp_error;
		return no_error;
	}
	return no_error;
}
