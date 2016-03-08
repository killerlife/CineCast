#include "BaseOperation.h"
#include "ini.h"
#include "../externcall/ExternCall.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <exception>
#include <iostream>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/md5.h>
#include <errno.h>

#include <log/Log.h>
#include <log/LogStruct.h>
#include <xml/GeneralXMLLib/GeneralXMLLib.h>
#include <xml/GeneralXMLLib/Element.h>

#define NETDEV_COUNT 2
#define ETH "/etc/sysconfig/network-scripts/ifcfg-enp6s"
#define ETHA "/etc/sysconfig/network-scripts/ifcfg-"
#define RESOLV "/etc/resolv.conf"

using namespace brunt;
namespace fs = boost::filesystem;
extern ILog* gLog;

NetOperation::NetOperation()
{

}

NetOperation::~NetOperation()
{

}

std::list<NETWORK_CONF>& NetOperation::GetNetConfig()
{
	ICMyini* ini = createMyini();
	char fn[1024];
	m_list.clear();
	std::string eth0, eth1;

	if (ini)
	{
		if (ini->load("/etc/CineCast/CineCast.cfg"))
		{
			std::string tmp;
			if(ini->read(" ", "ETH0", tmp))
				eth0 = tmp;
			if(ini->read(" ", "ETH1", tmp))
				eth1 = tmp;
		}
		else
		{
			eth0 = "enp6s0";
			eth1 = "enp6s1";
		}
		sprintf(fn, "/etc/sysconfig/network-scripts/ifcfg-%s", eth0.c_str());
			if(ini->load(fn))
			{
				std::string tmp;
				NETWORK_CONF nc;
			if(ini->read(" ", "BOOTPROTO", tmp))
			{
				if(tmp == "dhcp")
					nc.nDhcp = 1;
				else
					nc.nDhcp = 0;
			}
			else
					nc.nDhcp = 0;

			ini->read(" ", "DEVICE", nc.strDevName);
			ini->read(" ", "IPADDR", nc.strIp);
			if(ini->read(" ", "PREFIX", tmp))
				nc.strNetmask = prefix2mask(atoi(tmp.c_str()));
			else
			{
				if(ini->read(" ", "NETMASK", tmp))
					nc.strNetmask = tmp;
				else
					nc.strNetmask = "255.0.0.0";
			}
			ini->read(" ", "GATEWAY", nc.strGateway);
			m_list.push_back(nc);
		}
		sprintf(fn, "/etc/sysconfig/network-scripts/ifcfg-%s", eth1.c_str());
		if(ini->load(fn))
		{
			std::string tmp;
			NETWORK_CONF nc;
			if(ini->read(" ", "BOOTPROTO", tmp))
			{
				if(tmp == "dhcp")
					nc.nDhcp = 1;
				else
					nc.nDhcp = 0;
			}
			else
				nc.nDhcp = 0;
				ini->read(" ", "DEVICE", nc.strDevName);
				ini->read(" ", "IPADDR", nc.strIp);
			if(ini->read(" ", "PREFIX", tmp))
				nc.strNetmask = prefix2mask(atoi(tmp.c_str()));
			else
			{
				if(ini->read(" ", "NETMASK", tmp))
					nc.strNetmask = tmp;
				else
					nc.strNetmask = "255.0.0.0";
			}
				ini->read(" ", "GATEWAY", nc.strGateway);
				m_list.push_back(nc);
			}
		}
	return m_list;
}

REMOTE_CONF& NetOperation::GetRemoteConfig()
{
	ICMyini* ini = createMyini();
	char fn[200];
	if (ini)
	{
		sprintf(fn, "%s%d", ETH, 0);
		if(ini->load(fn))
		{
			std::string tmp;
			if(ini->read(" ", "DNS1", tmp))
				m_rc.strDns1 = tmp;
			if(ini->read(" ", "DNS2", tmp))
				m_rc.strDns2 = tmp;
		}
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			std::string tmp;
			if(ini->read(" ", "SERVER", tmp))
				m_rc.strRemote = tmp;
			if(ini->read(" ", "PORT", tmp))
				m_rc.nPort = atoi(tmp.c_str());
		}
	}
	return m_rc;
}

bool NetOperation::SetNetConfig(std::list<NETWORK_CONF>& m_listNetconf)
{
	ICMyini *ini = createMyini();
	char fn[1024];

	m_list.clear();
	std::list<NETWORK_CONF>::iterator itor;
	for (itor = m_listNetconf.begin(); itor != m_listNetconf.end(); ++itor)
	{
		NETWORK_CONF nc = *itor;
		m_list.push_back(nc);
		sprintf(fn, "%s%s", ETHA, nc.strDevName.c_str());
		ini->load(fn);
		DPRINTF("%s %s %s %s\n",
			nc.strIp.c_str(),
			nc.strNetmask.c_str(),
			nc.strGateway.c_str(),
			nc.strDevName.c_str());

		std::string tmp;
		if (nc.nDhcp)
			tmp = "dhcp";
		else
		{
			tmp = "none";
			if(!nc.strIp.empty())
				ini->write(" ", "IPADDR", nc.strIp.c_str());
			if(!nc.strNetmask.empty())
			{
				char buf[10];
				memset(buf, 0, 10);
				sprintf(buf, "%d", calcmask(nc.strNetmask));
				ini->write(" ", "PREFIX", buf);
			}
			if(!nc.strGateway.empty())
				ini->write(" ", "GATEWAY", nc.strGateway.c_str());
		}
		ini->write(" ", "BOOTPROTO", tmp.c_str());
		DPRINTF("%s\n", fn);

		if(nc.nDhcp)
		{
			sprintf(fn, "nmcli ");
		}
		else
		{
			sprintf(fn,
				"nmcli connection modify %s ipv4.address '%s/%d' ipv4.gateway '%s'",
				nc.strDevName.c_str(),
				nc.strIp.c_str(),
				calcmask(nc.strNetmask),
				nc.strGateway.c_str());
			DPRINTF("%s\n", fn);
			system(fn);

			sprintf(fn,
				"nmcli con up %s",
				nc.strDevName.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}
		ini->save(fn);
	}
}

int NetOperation::calcmask(std::string mask)
{
	if (mask.empty())
	{
		return 8;
	}
	char *buf = new char[mask.size() + 1];
	memset(buf, 0, mask.size() + 1);
	memcpy(buf, mask.c_str(), mask.size());

	char * pos = strstr(buf, ".");
	int count = 0;
	if (pos != NULL)
	{
		char s[10];
		memset(s, 0, 10);
		int n;
		memcpy(s, buf, (int)(pos - buf));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos++;
		char *pos2 = strstr(pos, ".");
		memset(s, 0, 10);
		memcpy(s, pos, (int)(pos2 - pos));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos2++;
		pos = strstr(pos2, ".");
		memset(s, 0, 10);
		memcpy(s, pos2, (int)(pos - pos2));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos++;
		sscanf(pos, "%d", &n);
		count += calccount(n);
		delete[] buf;
		return count;
	}
	delete[] buf;
	return 8;
}

int NetOperation::calccount(int n)
{
	if(n > 255)
		return 8;
	char c = (char)n;

	int count = 0;
	for(int i = 0; i < 8; i++)
	{
		if(c & 0x80)
		{
			count++;
			c = c<<1;
		}
		else
			break;
	}
	return count;
}

std::string NetOperation::prefix2mask(int n){
	int t = n;
	int a,b,c,d;
	a = b = c = d = 0;
	if( t <= 8)
	{
		for (int i  = 0; i < t; i++)
		{
			a |= (0x80 >> i);
		}
	}
	else
	{
		a = 0xff;
		t -= 8;
		if(t <=8 )
		{
			for (int i = 0; i < t; i++)
			{
				b |= (0x80 >> i);
			}
		}
		else
		{
			b = 0xff;
			t -= 8;
			if (t <= 8)
			{
				for(int i = 0; i < t; i++)
					c |= (0x80 >> i);
			}
			else
			{
				c = 0xff;
				t -= 8;
				for (int i = 0; i < t; i++)
				{
					d |= (0x80 >> i);
				}
			}
		}
	}
	char s[20];
	sprintf(s, "%d.%d.%d.%d", a, b, c, d);
	std::string ss = s;
	return ss;
}

bool NetOperation::SetRemoteConfig(REMOTE_CONF &m_remoteConf)
{
	char fn[200];

	ICMyini* ini = createMyini();
	if (ini)
	{
		ini->load("/etc/CineCast/CineCast.cfg");
		ini->write(" ", "SERVER", m_remoteConf.strRemote.c_str());
		sprintf(fn, "%d", m_remoteConf.nPort);
		if(m_remoteConf.nPort != 0)
		ini->write(" ", "PORT", fn);
		ini->save("/etc/CineCast/CineCast.cfg");
	}
#if 1
	ICMyini* ini2 = createMyini();
	if (ini2)
	{
		std::string tmp;
		if(ini->read(" ", "ETH0", tmp))
		{
			if (!tmp.empty())
			{
				sprintf(fn, "%s%s", ETHA, tmp.c_str());
				if(ini2->load(fn))
		{
					if(!m_remoteConf.strDns1.empty())
						ini2->write(" ", "DNS1", m_remoteConf.strDns1.c_str());
					if(!m_remoteConf.strDns2.empty())
						ini2->write(" ", "DNS2", m_remoteConf.strDns2.c_str());
					ini2->save(fn);
				}
				sprintf(fn,
					"nmcli con mod %s ipv4.dns \"%s %s\"",
					tmp.c_str(),
					m_remoteConf.strDns1.c_str(),
					m_remoteConf.strDns2.c_str());
				DPRINTF("%s\n", fn);
				system(fn);

				sprintf(fn,
					"nmcli con up %s",
					tmp.c_str());
			DPRINTF("%s\n", fn);
				system(fn);
			}
		}
		if(ini->read(" ", "ETH1", tmp))
		{
			if (!tmp.empty())
			{
				sprintf(fn, "%s%s", ETHA, tmp.c_str());
				if(ini2->load(fn))
				{
			if(!m_remoteConf.strDns1.empty())
				ini2->write(" ", "DNS1", m_remoteConf.strDns1.c_str());
			if(!m_remoteConf.strDns2.empty())
				ini2->write(" ", "DNS2", m_remoteConf.strDns2.c_str());
					ini2->save(fn);
				}
			sprintf(fn,
				"nmcli con mod %s ipv4.dns \"%s %s\"",
				tmp.c_str(),
				m_remoteConf.strDns1.c_str(),
				m_remoteConf.strDns2.c_str());
			DPRINTF("%s\n", fn);
			system(fn);

			sprintf(fn,
				"nmcli con up %s",
				tmp.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}
	}
	}
#endif
	return true;
}

SatelliteConfOperation::SatelliteConfOperation()
{
}

SatelliteConfOperation::~SatelliteConfOperation()
{

}

bool SatelliteConfOperation::ReadConfig()
{
	extern TUNER_CONF gTuner;
	ICMyini *ini = createMyini();
	if (ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			std::string tmp;
			if(ini->read(" ", "FREQ", tmp))
				gTuner.nFreq = atoi(tmp.c_str());
			if(ini->read(" ", "HIBAND", tmp))
				gTuner.nHiBand = atoi(tmp.c_str());
			if(ini->read(" ", "SR", tmp))
				gTuner.nSR = atoi(tmp.c_str());
			if (ini->read(" ", "MIS", tmp))
				gTuner.nMis = atoi(tmp.c_str());
			if(ini->read(" ", "SATNO", tmp))
				gTuner.nSatNo = atoi(tmp.c_str());
			if(ini->read(" ", "DEVNAME", tmp))
				gTuner.strDevName = tmp;
			if(ini->read(" ", "DELSYS", tmp))
				gTuner.strDelSys = tmp;
			if(ini->read(" ", "FEC", tmp))
				gTuner.strFec = tmp;
			if(ini->read(" ", "MOD", tmp))
				gTuner.strModulation = tmp;
			if(ini->read(" ", "ROLL", tmp))
				gTuner.strRollOff = tmp;
			if(ini->read(" ", "POL", tmp))
				gTuner.strPolVert = tmp;
		}	
	}

	return true;
}

bool SatelliteConfOperation::WriteConfig()
{
	extern TUNER_CONF gTuner;
	ICMyini *ini = createMyini();
	char buf[500];
	if(ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
		std::string tmp;
		sprintf(buf, "%ld", gTuner.nFreq);
		tmp = buf;
		
		if(!tmp.empty())
			ini->write(" ", "FREQ", tmp.c_str());

		sprintf(buf, "%ld", gTuner.nHiBand);
		tmp = buf;
		if(!tmp.empty())
			ini->write(" ", "HIBAND", tmp.c_str());

		sprintf(buf, "%ld", gTuner.nSR);
		tmp = buf;
		if(!tmp.empty())
			ini->write(" ", "SR", tmp.c_str());

		sprintf(buf, "%ld", gTuner.nMis);
		tmp = buf;
		if(!tmp.empty())
			ini->write(" ", "MIS", tmp.c_str());

		sprintf(buf, "%ld", gTuner.nSatNo);
		tmp = buf;
		if(!tmp.empty())
			ini->write(" ", "SATNO", tmp.c_str());

		if(!tmp.empty())
			ini->write(" ", "DEVNAME", gTuner.strDevName.c_str());

		if(!tmp.empty())
			ini->write(" ", "DELSYS", gTuner.strDelSys.c_str());

		if(!tmp.empty())
			ini->write(" ", "FEC", gTuner.strFec.c_str());

		if(!tmp.empty())
			ini->write(" ", "MOD", gTuner.strModulation.c_str());

		if(!tmp.empty())
			ini->write(" ", "ROLL", gTuner.strRollOff.c_str());

		if(!tmp.empty())
			ini->write(" ", "POL", gTuner.strPolVert.c_str());

		return ini->save("/etc/CineCast/CineCast.cfg");
	}
	}
	return false;
}

ContentOperation::ContentOperation()
{
	m_srcList.push_back(PST_USB); //new   
	m_srcList.push_back(PST_HDD);
	m_srcList.push_back(PST_RAID);
	getIContentManager()->update(m_srcList);
}

ContentOperation::~ContentOperation()
{

}

std::vector<InfoData>& ContentOperation::GetContentList(int src)
{
	switch(src)
	{
	case PST_HDD:
		if(getIContentManager()->isReady(PST_HDD))
		{
			m_Content.clear();
			getIContentManager()->getProgramFileList(PST_HDD, TYPE_MOVIE, m_Content);
		}
		return m_Content;
	case PST_USB:
		if(getIContentManager()->isReady(PST_USB))
		{
			m_UsbContent.clear();
			getIContentManager()->getProgramFileList(PST_USB,  TYPE_MOVIE, m_UsbContent);
		}
		return m_UsbContent;
	case PST_RAID:
		if(getIContentManager()->isReady(PST_RAID))
		{
			m_RaidContent.clear();
			getIContentManager()->getProgramFileList(PST_RAID,  TYPE_MOVIE, m_RaidContent);
		}
		return m_RaidContent;
	}
	return m_Content;
}

int ContentOperation::UpdateProgramList(std::vector<int> srcList)
{
	bool bFind = false;
	for (int i = 0; i < m_srcList.size(); i++)
	{
		if(m_srcList.at(i) == srcList.at(0))
		{
			bFind = true;
			break;
		}
	}
	if (!bFind)
		m_srcList.push_back(srcList.at(0));
	    
	return getIContentManager()->update(m_srcList);
}

bool ContentOperation::IsProgramListReady(int src)
{
	return getIContentManager()->isReady(src);
}

#include <sys/vfs.h>

uint64 ContentOperation::GetAvalibleSpace(int src)
{
	struct statfs s;
	uint64 freeSpace;
	switch(src)
	{
	case PST_HDD:
		statfs("/storage", &s);
		freeSpace = s.f_bsize * ((unsigned long long)s.f_bavail);
		return freeSpace;
	case PST_USB:
		statfs("/media/usb", &s);
		freeSpace = s.f_bsize * ((unsigned long long)s.f_bavail);
		return freeSpace;
	}
	return 0;
}

uint64 ContentOperation::GetTotalSpace(int src)
{
	struct statfs s;
	uint64 freeSpace;
	switch(src)
	{
	case PST_HDD:
		statfs("/storage", &s);
		freeSpace = s.f_bsize * ((unsigned long long)s.f_blocks);
		return freeSpace;
	case PST_USB:
		statfs("/media/usb", &s);
		freeSpace = s.f_bsize * ((unsigned long long)s.f_blocks);
		return freeSpace;
	}
	return 0;
}

void ContentOperation::UpdateDirList(int src)
{
	std::string root;
	switch(src)
	{
	case PST_HDD:
		root = "/storage/";
		break;
	case PST_USB:
		root = "/media/usb";
		break;
	}
	m_dir.clear();
	FindDir(root);
#if 0
	for(int i = 0; i < m_dir.size(); i++)
	{
		DPRINTF("%s\n", m_dir.at(i).c_str());
	}
#endif
}

void ContentOperation::FindDir(std::string dir)
{
	char str[512];
	fs::path p(dir);
	try
	{
		m_dir.push_back(dir);

		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(p); itr != end_itr; ++itr)
		{
			try
			{
				boost::filesystem::path* dir_name = NULL;
				if(fs::is_directory(*itr))
				{
					FindDir((*itr).path().native());
				}
			}
			catch(const fs::filesystem_error& e)
			{
				sprintf(str, "[ContentOperation] FindDir: Skip the IO error, except: %s.", e.what());
				if (gLog)
				{
					gLog->Write(LOG_ERROR, str);
				}
				DPRINTF("[ContentOperation] FindDir: Skip the IO error, except: %s.", e.what());
			}
		}
	}
	catch(const fs::filesystem_error& e)
	{
		sprintf(str, "[ContentOperation] FindDir: except: %s.", e.what());
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
	}
}

bool ContentOperation::AutoDelete(int src, std::vector<std::string>&runList)
{
	uint64 fspace = GetAvalibleSpace(src);
	if( fspace < (uint64)350*1024*1024*1024)
	{
		char str[512];
		sprintf(str, "[ContentOperation] AutoDelete: %lld free.", fspace);
		if(gLog)
		    gLog->Write(LOG_NETCOMMU, str);
		    
		UpdateDirList(src);
		switch(src)
		{
		case PST_HDD:
			{
				for (int i = 0; i < m_dir.size(); i++)
				{
					bool bFind = false;
					for(int j = 0; j < runList.size(); j++)
					{
						if(runList.at(j).find(m_dir.at(i)) != std::string::npos)
						{
							bFind = true;
							break;
						}
					}
					if(!bFind)
					{
						char cmd[512];
						sprintf(cmd, "rm -rf %s", m_dir.at(i).c_str());
						//DPRINTF("%s\n", cmd);
						system(cmd);
					}
				}
			}
			break;
		case PST_USB:
			break;
		}
	}
}
mke2fs::mke2fs():fp(NULL), m_Status(0)
{

}

mke2fs::~mke2fs()
{
	CActiveThread::stop();
}

bool mke2fs::FormatDisk(DISK_TYPE type)
{
	m_type = type;

	if(!(status() == thread_stopped || status() == thread_ready ))
	{
		DPRINTF("The thread had not been ready, status = %d.", status());
		return false;
	}

	if(isStopped())
	{
		CActiveThread::stop();
	}

	if(!start())
	{
		DPRINTF("Start thread failed, status = %d.", status());
		return false;
	}

	return true; 
}

void mke2fs::doit()
{
	ICMyini* ini = createMyini();
	bool bRead = false;
	std::string tmp;
	int res = 0;
	char str[512];
	if(ini)
	{
		if (ini->load("/etc/CineCast/config.cfg"))
		{
			switch(m_type)
			{
			case DISK_REMOVEABLE:
				if(ini->read(" ", "programdisk", tmp))
				{
					bRead = true;
				}
				break;
			case DISK_RAID:
				if(ini->read(" ", "raiddisk", tmp))
				{
					bRead = true;
				}
				break;
			}
		}
	}
	if (!bRead)
	{
		switch(m_type)
		{
		case DISK_REMOVEABLE:
			tmp = "/dev/sdb1";
			break;
		case DISK_RAID:
			tmp = "";
			break;
		}
	}
	releaseMyini(ini);

	switch(m_type)
	{
	case DISK_REMOVEABLE:
		chdir("/");

		res = umount("/storage");
		if(res != 0)
		{
			m_Status = 0;
			out[0] = -1;
			memcpy(out + 1, "umount disk failed.\n", 19);
			sout += (out+1);
			return;
		}

		tmp = "mkfs.ext3 " + tmp;
		fp = popen(tmp.c_str(), "r");
		setvbuf(fp, NULL, _IONBF, 0);
		sout.clear();
		memset(out, 0, 80);
		while(fgets(out+1, sizeof(out-1), fp) != NULL)
		{
			m_Status = 1;
			out[0] = 1;
			sout += (out+1);
		}
		out[0] = -1;
		pclose(fp);
		m_Status = 0;
		fp = NULL;

		MountDisk(DISK_REMOVEABLE);
		chdir("/storage");

		break;
	case DISK_RAID:
		if(tmp != "")
		{
			res = umount("/raid");
			if(res != 0)
			{
				m_Status = 0;
				out[0] = -1;
				memcpy(out + 1, "umount disk failed.\n", 19);
				sout += (out+1);
				return;
			}
	tmp = "mkfs.ext3 " + tmp;
	fp = popen(tmp.c_str(), "r");
	setvbuf(fp, NULL, _IONBF, 0);
	sout.clear();
	memset(out, 0, 80);
	while(fgets(out+1, sizeof(out-1), fp) != NULL)
	{
		m_Status = 1;
		out[0] = 1;
 		sout += (out+1);
		//out = buf;
// 		DPRINTF("%s", out.c_str());
	}
	out[0] = -1;
	pclose(fp);
	m_Status = 0;
	fp = NULL;
			MountDisk(DISK_RAID);
		}
		else
		{
			sprintf(str, "[mke2fs] doit: no raid array.");
			if(gLog)
				gLog->Write(LOG_ERROR, str);
			res = 0;
		}
		break;
	}
}

char* mke2fs::GetOutput()
{
	return out;
}

std::string& mke2fs::GetSOutput()
{
	return sout;
}

uint8 mke2fs::CheckStatus()
{
	return m_Status;
}

bool mke2fs::MountDisk(DISK_TYPE type)
{
	ICMyini* ini = createMyini();
	bool bRead = false;
	std::string tmp;
	m_type = type;
	char str[512];
	if(ini)
	{
		if (ini->load("/etc/CineCast/config.cfg"))
		{
		switch(m_type)
		{
		case DISK_REMOVEABLE:
			if(ini->read(" ", "programdisk", tmp))
			{
				bRead = true;
			}
			break;
		case DISK_RAID:
			if(ini->read(" ", "raiddisk", tmp))
			{
				bRead = true;
			}
			break;
		}
	}
	}
	if (!bRead)
	{
		sprintf(str, "[mke2fs] MountDisk: Read config.cfg error.");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		switch(m_type)
		{
		case DISK_REMOVEABLE:
			tmp = "/dev/sdb1";
			break;
		case DISK_RAID:
			tmp = "";
			break;
		}
	}
	releaseMyini(ini);
	int res;
	switch(m_type)
	{
	case DISK_REMOVEABLE:
		res = mount(tmp.c_str(), "/storage", "ext3", MS_SYNCHRONOUS, NULL);
		break;
	case DISK_RAID:
		if(tmp != "")
			res = mount(tmp.c_str(), "/raid", "ext3", MS_SYNCHRONOUS, NULL);
		else
		{
			sprintf(str, "[mke2fs] MountDisk: no raid array.");
			if(gLog)
				gLog->Write(LOG_ERROR, str);
			res = 0;
		}
		break;
	}
	if(res == 0)
		return true;
	if(errno == EBUSY)
		return true;
	sprintf(str, "[mke2fs] MountDisk: mount disk error - %d.", m_type);
	if(gLog)
		gLog->Write(LOG_ERROR, str);
	return false;
}

void System::Reboot()
{
	system("/bin/sync");
	system("/sbin/reboot");
}

void System::Shutdown()
{
	system("/bin/sync");
	system("/sbin/init 0");
}

void System::SetDateTime(char* stime)
{
	std::string strParam = "/bin/date -s ";
	strParam += stime;
	system(strParam.c_str());
	system("/sbin/clock -w");
}

#include <stdio.h>
#include <stdlib.h>

//new class
void USB::USB_Mount()
{
	//mnt?????usb   /mnt/usb /media/G


/*   //old
	system("mkdir -p /media/usb");   
	system("mount -t vfat /dev/sdc1 /media/usb");
    //system("mount -t vfat /dev/sdd1 /media/usb");
	//mount -t ntfs /dev/sdc1 /mnt/usb
	//mount -t vfat /dev/sdc1 /mnt/usb
	system("ls /media");
	system("ls /media/usb");
    
    printf("USB mount Successful \n");
*/


/*
		FILE *fp;                      //  /etc/mtabļ  ȡ̷
		char buf[1024*10]={'\0'};     //   ȫ

		if((fp=fopen("/etc/mtab","rb"))==NULL)
		{
		printf("File open error:/etc/mtab");
		exit(0);
		}
		// while ((ch=fgetc(fp))!=EOF)

		char* pTXT=buf;
		while ((*pTXT=fgetc(fp))!=EOF)
		{
		   pTXT++;
		}
		*pTXT='\0';    //Ѿȡȫıݣַβ
        fclose(fp);    //ļԹر

		printf("mtab:%s\n",buf);  

        char* p=buf;   //"/dev/sdc1" "/dev/sdd1" "/dev/sdf1" ַ?
        char* ptmp;

		int i;
		//ӵ豸УҪųsda,sdb
		for(i=0;i<sizeof(buf);i++)
		{
			//д1    //Чд2
             if(*p=='/'&&*(p+1)=='d'&&*(p+2)=='e'&&*(p+3)=='v'&&*(p+4)=='/'&&*(p+5)=='s'&&*(p+6)=='d') 
			 {
                 ptmp=p;
			 }

             p++;
		}

        char str_devname[10];   //¼ջõ?/dev/sdc1"
        str_devname[9]='\0';
        memcpy(str_devname,ptmp,9);  
		printf("devname=:%s\n",str_devname);


		if(str_devname[7]=='a'||str_devname[7]=='b')   //ųsda,sdb
		{
		    printf("no usb device\n");   //жûм⵽usb?
		}

        char str_cmd[256]="mount -t vfat ";   //ƴϵͳ
        strcat(str_cmd,str_devname);
        strcat(str_cmd, " /media/usb");

		system("mkdir -p /media/usb");   
		system(str_cmd);
		//system("mount -t vfat /dev/sdd1 /media/usb");
		system("ls /media");
		system("ls /media/usb");
		printf("USB mount Successful \n");

*/
}

/*
busy 
umount /dev/sdc1umount /backup
?busy?
fuser -m /dev/sdc1
???sdb1????
?fuser -km /dev/sdc1
???/dev/sdc1????
umount /dev/sdc1umount /backup
mount /dev/sdc1 /backup
*/


void USB::USB_UnMount()
{
    system("umount /media/usb");
	//system("rm -rf /media/usb");   //ֹûжؾͱɾ
	system("ls /media");
	//system("ls /media/usb");

    printf("USB unMount Successful \n");
}

void USB::USB_UpdateSpace()     //???????????
{

}

Md5Class::Md5Class():bMd5Success(false), nRollBackLen(0)
{
}

bool Md5Class::Md5Parser(char *buf)
{
	int step = 0;
	try
	{
		efl.clear();
		std::string rootName("EncodeInfo");
		CElement prjElement(rootName.c_str(), buf, BUFFER_FLAG);
		step = 1;

		CElement EncodeFileList = prjElement.GetChildByNameIndex("EncodeFileList");
		efl.Id = atoi(EncodeFileList.GetAttribute("ID").c_str());
		efl.Name = EncodeFileList.GetAttribute("Name");
		efl.ChallengeCode = EncodeFileList.GetChildElementText("ChallengeCode");

// 		DPRINTF("efl.id = %u Name = %s\n", efl.Id, efl.Name.c_str());
// 		DPRINTF("efl.ChallengeCode = %s\n", efl.ChallengeCode.c_str());
		
		int count = EncodeFileList.GetChildCountByName("EncodeFile");
		for(int i = 0; i < count; i++)
		{
// 			DPRINTF("EncoderFile:%u\n", i);
			CElement EncodeFile = EncodeFileList.GetChildByNameIndex("EncodeFile", i);
			ENCODEFILE ef;
			ef.Name = EncodeFile.GetAttribute("Name");
			ef.Id = atoi(EncodeFile.GetAttribute("ID").c_str());
			if(EncodeFile.GetAttribute("EncodeCount") == "0")
				ef.Encode = false;
			else
				ef.Encode = true;
			if(EncodeFile.GetAttribute("MD5Count") == "0")
				ef.Md5 = false;
			else
				ef.Md5 = true;
			ef.SubPath = EncodeFile.GetAttribute("SubPath");
// 			DPRINTF("\nName:%s ID:%u EC:%u MD:%u SP:%s\n", 
// 				ef.Name.c_str(), ef.Id, ef.Encode, ef.Md5, ef.SubPath.c_str());

			if(ef.Encode)
			{
				CElement EncodeBlockList = EncodeFile.GetChildByNameIndex("EncodeBlockList");
				int c1 = EncodeBlockList.GetChildCountByName("EncodeBlock");
				int ii;
				for (ii = 0; ii < c1; ii++)
				{
					ENCODEBLOCK eb;
					CElement e = EncodeBlockList.GetChildByNameIndex("EncodeBlock", ii);
					eb.EncodeBegin = atoll(e.GetChildElementText("EncodeBeginPoint").c_str());
					eb.EncodeEnd = atoll(e.GetChildElementText("EncodeEndPoint").c_str());
					ef.EncodeBlockList.push_back(eb);
// 					DPRINTF("EC Begin:%08X End:%08X\n", eb.EncodeBegin, eb.EncodeEnd);
				}
			}

			if(ef.Md5)
			{
				CElement MD5BlockList = EncodeFile.GetChildByNameIndex("MD5BlockList");
				int c1 = MD5BlockList.GetChildCountByName("MD5Block");
				int a = 0;
				for (a = 0; a < c1; a++)
				{
					MD5BLOCK mb;
					CElement e = MD5BlockList.GetChildByNameIndex("MD5Block", a);
					mb.Md5Begin = atoll(e.GetChildElementText("MD5BeginPoint").c_str());
					mb.Md5End = atoll(e.GetChildElementText("MD5EndPoint").c_str());
					std::string s = e.GetChildElementText("MD5");
// 					DPRINTF("%s\n", s.c_str());
					char hex[20];
					sscanf(s.c_str(),
						"%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
						(uint8*)(hex+0), (uint8*)(hex+1), (uint8*)(hex+2), (uint8*)(hex+3),
						(uint8*)(hex+4), (uint8*)(hex+5), (uint8*)(hex+6), (uint8*)(hex+7),
						(uint8*)(hex+8), (uint8*)(hex+9), (uint8*)(hex+10), (uint8*)(hex+11),
						(uint8*)(hex+12), (uint8*)(hex+13), (uint8*)(hex+14), (uint8*)(hex+15));
					memcpy(mb.md5, hex, 16);
					ef.Md5BlockList.push_back(mb);

// 					DPRINTF("MB Begin:%08X End:%08X\n", mb.Md5Begin, mb.Md5End);
// 					DPRINTF("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n",
// 						(uint8)mb.md5[0], (uint8)mb.md5[1], (uint8)mb.md5[2], (uint8)mb.md5[3],
// 						(uint8)mb.md5[4], (uint8)mb.md5[5], (uint8)mb.md5[6], (uint8)mb.md5[7],
// 						(uint8)mb.md5[8], (uint8)mb.md5[9], (uint8)mb.md5[10], (uint8)mb.md5[11],
// 						(uint8)mb.md5[12], (uint8)mb.md5[13], (uint8)mb.md5[14], (uint8)mb.md5[15]);
				}
			}
			efl.EncodeFileList.push_back(ef);
		}
	}
	catch (std::exception&)
	{
		char str[512];
		std::string errStr;
		switch(step)
		{
		case 0:
			errStr = "XML Parse failed.";
			break;
		case 1:
			errStr = "XML Element failed.";
			break;
		}
		sprintf(str, "[BaseOperation] Md5Parser: %s", errStr.c_str());
		if(gLog)
			gLog->Write(LOG_ERROR, str);
		DPRINTF("%s\n", str);
		return false;
	}
	return true;
}

bool Md5Class::GetMd5(FILE* fp, MD5BLOCK mb)
{
	if(fp <= 0)
		return false;

	MD5_CTX c;
	MD5_Init(&c);

	char buf[1024*1024];
	int count = (mb.Md5End - mb.Md5Begin)/1024/1024;
	DPRINTF("/:%lld\n", count);
	fseek(fp, mb.Md5Begin, SEEK_SET);

	for(int i = 0; i < count; i++)
	{
		fread(buf, 1024*1024, 1, fp);
		MD5_Update(&c, buf, 1024*1024);
	}
	count = (mb.Md5End - mb.Md5Begin)%(1024*1024);
	DPRINTF("%%:%lld\n", count);
	if (count > 0)
	{
		fread(buf, count, 1, fp);
		MD5_Update(&c, buf, count);
	}
	DPRINTF("Start:%lld end:%lld\n", mb.Md5Begin, mb.Md5End);

	char md[16];
	memset(md, 0, 16);
	MD5_Final((unsigned char*)md, &c);
	if (memcmp(md, mb.md5, 16) == 0)
	{
		return true;
	}
	DPRINTF("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n",
		(uint8)md[0], (uint8)md[1], (uint8)md[2], (uint8)md[3],
		(uint8)md[4], (uint8)md[5], (uint8)md[6], (uint8)md[7],
		(uint8)md[8], (uint8)md[9], (uint8)md[10], (uint8)md[11],
		(uint8)md[12], (uint8)md[13], (uint8)md[14], (uint8)md[15]);
	DPRINTF("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n",
		(uint8)mb.md5[0], (uint8)mb.md5[1], (uint8)mb.md5[2], (uint8)mb.md5[3],
		(uint8)mb.md5[4], (uint8)mb.md5[5], (uint8)mb.md5[6], (uint8)mb.md5[7],
		(uint8)mb.md5[8], (uint8)mb.md5[9], (uint8)mb.md5[10], (uint8)mb.md5[11],
		(uint8)mb.md5[12], (uint8)mb.md5[13], (uint8)mb.md5[14], (uint8)mb.md5[15]);
	DPRINTF("MD5 failed\n\n", mb.Md5Begin, mb.Md5End);
	if(gLog)
	    gLog->Write(LOG_ERROR, "[Md5Class] GetMd5: Get hash failed.");
	return false;
}

bool Md5Class::Md5Verify(uint32 filmId)
{
	std::string fn;
	uint64 rollback = 0;
	char str[512];
	for (int i = 0; i < efl.EncodeFileList.size(); i++)
	{
		ENCODEFILE ef = efl.EncodeFileList.at(i);
		if(ef.Md5)
		{
			//convert windows path "\xxx\xxx\xxx" to
			//linux path "/xxx/xxx/xxx"
			std::string s;
			for(int j = 0; j < ef.SubPath.size(); j++)
			{
				if (ef.SubPath.at(j) == '\\')
					s.push_back('/');
				else
					s.push_back(ef.SubPath.at(j));
			}
			if(s.at(s.size() - 1) == '/')
				fn = "/storage/" + s + ef.Name;
			else
				fn = "storage/" + s + "/" + ef.Name;
			//convert end

			FILE *fp = fopen(fn.c_str(), "rb");
			if(fp <= 0)
			{
				if(gLog)
				{
					sprintf(str, "[Md5Class] Md5Verify: Open %s failed.", fn.c_str());
					gLog->Write(LOG_ERROR, str);
				}
				DPRINTF("Open %s failed\n", fn.c_str());
				bMd5Success = false;
				nRollBackLen = rollback;
				SaveResult(filmId);
				return bMd5Success;
			}
			else
			{
				DPRINTF("Open %s success\n", fn.c_str());
				for (int j = 0; j < ef.Md5BlockList.size(); j++)
				{
					MD5BLOCK mb = ef.Md5BlockList.at(j);
					if(GetMd5(fp, mb))
						rollback += (mb.Md5End - mb.Md5Begin);
					else
					{
						bMd5Success = false;
						nRollBackLen = rollback;
						fclose(fp);
						SaveResult(filmId);
						return bMd5Success;
					}
				}
				fclose(fp);
			}
		}
	}
	bMd5Success = true;
	SaveResult(filmId);
	return bMd5Success;
}

void Md5Class::SaveResult(uint32 filmId)
{
    ICMyini *ini = createMyini();
    if(ini)
    {
	char str[512];
	if(bMd5Success)
	    ini->write(" ", "SUCCESS", "true");
	else
	    ini->write(" ", "SUCCESS", "false");
	sprintf(str, "%lld", nRollBackLen);
	ini->write(" ", "LENGTH", str);
	sprintf(str, "/tmp/film%ld.md5", filmId);
	ini->save(str);
	releaseMyini(ini);
    }
}

static Md5Class gMd5Class;

Md5Class* CreateMd5Class()
{
	return &gMd5Class;
}

void ReleaseMd5Class(Md5Class* pMd5)
{
}

void RaidDetailParser::DetailParser(std::string strDetail)
{
	strRaidLevel = "";
	strState = "";
	nRaidDevices = nActiveDevices = nWorkingDevices = nFailedDevice = 0;
	nArraySize = nUsedSize = 0;
	strDevState.clear();
	size_t pos;
	if ((pos = strDetail.find("Raid Level")) != std::string::npos)
	{
		char buf[50];
		sscanf(strDetail.c_str() + pos + 13, "%s", buf);
		strRaidLevel = buf;
		if((pos = strDetail.find("State : ")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 8, "%s", buf);
			strState = buf;
		}
		if((pos = strDetail.find("Array Size")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 13, "%lld", &nArraySize);
		}
		if((pos = strDetail.find("Used Dev Size")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 16, "%lld", &nUsedSize);
		}
		if((pos = strDetail.find("Raid Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 15, "%d", &nRaidDevices);
		}
		if ((pos = strDetail.find("Active Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 17, "%d", &nActiveDevices);
		}
		if ((pos = strDetail.find("Working Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 18, "%d", &nWorkingDevices);
		}
		if ((pos = strDetail.find("Failed Devices")) != std::string::npos)
		{
			sscanf(strDetail.c_str() + pos + 17, "%d", &nFailedDevice);
		}
		if ((pos = strDetail.find("Number")) != std::string::npos)
		{
			int i = 0;
			for(i = 0;; i++)
			{
				if(strDetail.at(pos + i) == '\n')
					break;
			}
			i++;
			pos += i;
			for (i = 0; i < nRaidDevices; i++)
			{
				int n, major, minor, raid;
				char state1[20], state2[20], dev[40];
				int j = 0;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &n);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &major);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &minor);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%d", &raid);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", state1);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", state2);
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) == ' ')
						break;
				}
				pos += j;
				for (j = 0;; j++)
				{
					if(strDetail.at(pos + j) != ' ')
						break;
				}
				pos += j;
				sscanf(strDetail.c_str() + pos, "%s", dev);
				for(j = 0;; j++)
				{
					if(strDetail.at(pos + j) == '\n')
						break;
				}
				pos += (j+1);
				std::string sState = dev;
				sState += "|";
				sState +=state1;
				sState += " ";
				sState += state2;
				strDevState.push_back(sState);
			}
		}
	}
}

void RaidDetailParser::RunRaidManager()
{
	IExternCall* pEc = CreateExternCall();
	pEc->RunCommand("mdadm -D /dev/md0");
	while(1)
	{
		if (pEc->IsFinish())
		{
			DetailParser(pEc->GetOutput());
			break;
		}
	}
}
