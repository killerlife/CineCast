// config.cpp : 定义 DLL 应用程序的入口点。
//
#ifdef WIN32
#include "stdafx.h"
#endif
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#include "ini.h"
using namespace brunt;

#define CONFIGFILE	"config.ini"

#ifdef _WIN32


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif//_WIN32


class CConfig: public IConfig
{
public:
	// interface of IConfig
	virtual const string getRemoteUrl(){
		return m_strRemoteUrl;
	}

	virtual const int getRemotePort(){
		return m_nRemotePort;
	}

	virtual const string getProgramRootPath(){
		return m_strProgramRoot;
	}

	virtual const string getUsbRootPath(){
		return m_strUsbRoot;
	}

	virtual const string getBinRootPath(){
		return m_strRoot;
	}

	virtual const string getLogPath(){
		return m_strLogRoot;
	}

	virtual const string getLogOutPath(){
		return m_strLogOutRoot;
	}

	virtual const string getBreakPointPath(){
		return m_strBreakPointPath;
	}
	
	virtual const unsigned long getMachineId(){
		return m_nMachineId;
	}

	////////////////////
	CConfig();
	virtual ~CConfig();

private:
	bool init();
	bool save();

	string m_strRemoteUrl;
	string m_strProgramRoot;
	string m_strImportRoot;
	string m_strUsbRoot;
	string m_strConfigFile;
	string m_strRoot;
	string m_strKdmKey;
	string m_strVideoDecGUID;
	string m_strImportKdmKeyRoot;
	string m_strLogRoot;
	string m_strLogOutRoot;
	string m_strBreakPointPath;
	string m_strSplPath;
	vector<string> m_listImportPath;
	bool m_bHdcpEnable;
	bool m_bCheckLicenseDate;
	int m_nRemotePort;
	unsigned long m_nMachineId;
};

static CConfig config;

IConfig* getConfig()
{
	return &config;
}

CConfig::CConfig()
{
	string root;
#ifdef _WIN32
	m_strProgramRoot = "e:\\program";
	m_strImportRoot = "i:";
	m_strUsbRoot = "i:";

	HKEY hk = 0;
	char value[MAX_PATH];
	DWORD len=MAX_PATH;
	if (RegOpenKey(HKEY_LOCAL_MACHINE, KEY_V15,&hk)!=ERROR_SUCCESS)
	{
		::MessageBox(NULL, "Registry doesn't initialize!", "config", MB_OK|MB_ICONERROR);
		return;
	}
	if(RegQueryValueEx(hk, "root", NULL, NULL, (BYTE*)value, &len)==ERROR_SUCCESS)
	{
		root = value;
		root += "\\";
	}
	RegCloseKey(hk);

#else
	m_strProgramRoot = "/hdisk/program";
	m_strImportRoot = "/hdisk/usb";
	m_strUsbRoot = "/hdisk/usb";
	root = "./";
#endif//_WIN32

	m_strRoot = root;
	m_strConfigFile = m_strRoot + CONFIGFILE;
	m_strKdmKey = m_strRoot + ".key.pem";
	m_bHdcpEnable = false;
	m_bCheckLicenseDate = true;
	m_strBreakPointPath = root;

	init();
}

CConfig::~CConfig()
{
//	save();
}

bool CConfig::init()
{
	bool result = false;
	ICMyini* ini = createMyini();
	string tmp;
	if(ini)
	{
		if(ini->load(m_strConfigFile))
		{
			std::string strValue;
			ini->read("remote", "url", m_strRemoteUrl);
			ini->read("remote", "port", tmp);
			m_nRemotePort = atoi(tmp.c_str());
			ini->read("path", "programroot", m_strProgramRoot);
			ini->read("path", "importroot", m_strImportRoot);
			ini->read("path", "usbroot", m_strUsbRoot);
			ini->read("path", "kdmKey", m_strImportKdmKeyRoot);
			ini->read("path", "logroot", m_strLogRoot);
			ini->read("path", "logoutroot", m_strLogOutRoot);
			ini->read("path", "breakpoint", m_strBreakPointPath);
			
			ini->read("config", "machine_id", tmp);
			m_nMachineId = atol(tmp.c_str());
			
			ini->read("playctrl", "hdcp", strValue);
			ini->read("path", "splroot", m_strSplPath);
			m_bHdcpEnable = (strValue=="1"?true:false);
			ini->read("playctrl", "checkdate", strValue);
			m_bCheckLicenseDate = (strValue=="0"?false:true);
			result = true;

			// import path list
			for(int i=1; i<=10; i++)
			{
				char name[100];
				string strValue;
				sprintf(name, "path%02d", i);
				ini->read("import", name, strValue);
				if(strValue.empty()) continue;
				m_listImportPath.push_back(strValue);
			}
		}
		releaseMyini(ini);
	}
	return result;
}

bool CConfig::save()
{
	bool result = false;
	ICMyini* ini = createMyini();
	if(ini)
	{
		ini->write("path", "programroot", m_strProgramRoot.c_str());
		ini->write("path", "importroot", m_strImportRoot.c_str());
		ini->write("path", "usbroot", m_strUsbRoot.c_str());
		ini->write("path", "kdmKey", m_strImportKdmKeyRoot.c_str());
		ini->write("path", "logroot", m_strLogRoot.c_str());
		//printf("%s\n", m_strLogRoot.c_str());
		ini->write("path", "logoutroot", m_strLogOutRoot.c_str());
		ini->write("path", "breakpoint", m_strBreakPointPath.c_str());
		ini->write("playctrl", "hdcp", m_bHdcpEnable?"1":"0");
		ini->write("playctrl", "vd", m_strVideoDecGUID.c_str());
		for(int i=0; i<m_listImportPath.size(); i++)
		{
			char name[100];
			sprintf(name, "path%02d", i);
			ini->write("import", name, m_listImportPath[i].c_str());
		}
		if(ini->save(m_strConfigFile))
			result = true;
		releaseMyini(ini);
	}
	return result;
}

