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
#include "NetCommThread.h"

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
#if 0
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

			ini->read(" ", "NAME", nc.strDevName);
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
			ini->read(" ", "NAME", nc.strDevName);
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
#endif
		NETWORK_CONF nc;
		nc = GetNetConfig(eth0);
		if(nc.strDevName != "")
			m_list.push_back(nc);
		NETWORK_CONF nc2;
		nc2 = GetNetConfig(eth1);
		if(nc2.strDevName != "")
			m_list.push_back(nc2);
		}
	return m_list;
}

std::list<NETWORK_STATUS>& NetOperation::GetNetStatus()
{
	ICMyini* ini = createMyini();
	char fn[1024];
	m_slist.clear();
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
		NETWORK_STATUS nc;
		nc = GetNetStatus(eth0);
		if(nc.strDevName != "")
			m_slist.push_back(nc);
		NETWORK_STATUS nc2;
		nc2 = GetNetStatus(eth1);
		if(nc2.strDevName != "")
			m_slist.push_back(nc2);

	}
	return m_slist;
}

NETWORK_CONF NetOperation::GetNetConfig(std::string dev)
{
	IExternCall* pEc = CreateExternCall();
	char cmd[200];
	NETWORK_CONF nc;
	if(dev.size() > 10)
	{
		DPRINTF("Invaild network device %s\n", dev.c_str());
		nc.strConnected = "NODEV";
		nc.strDevName = "NODEV";
		usleep(100*1000);
		return nc;
	}
	sprintf(cmd, "nmcli device show %s", dev.c_str());
	pEc->RunCommand(cmd);
	DPRINTF("%s\n", cmd);

	std::string s;
	while(1)
	{
		if(pEc->IsFinish())
		{
			s = pEc->GetOutput();
			size_t pos;
			if((pos = s.find("GENERAL.状态:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				std::string ss = buf;
				if(ss.find("100") != std::string::npos)
					nc.strConnected = "TRUE";
				else
					nc.strConnected = "FALSE";
			}
			if((pos = s.find("GENERAL.STATE:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				std::string ss = buf;
				if(ss.find("100") != std::string::npos)
					nc.strConnected = "TRUE";
				else
					nc.strConnected = "FALSE";
			}
			DPRINTF("%s\n", nc.strConnected.c_str());
			break;
		}
		else
			usleep(200000);
	}
	if (nc.strConnected == "" || nc.strConnected.empty())
	{
		DPRINTF("%s\n", nc.strConnected.c_str());
		nc.strConnected = "NODEV";
		nc.strDevName = "NODEV";
		usleep(100*1000);
		return nc;
	}

	nc.strDevName = dev.c_str();
	usleep(100*1000);
	//return nc;

	sprintf(cmd, "nmcli con show %s", dev.c_str());
	pEc->RunCommand(cmd);

	DPRINTF("%s\n", cmd);
	while(1)
	{
		if (pEc->IsFinish())
		{
			s = pEc->GetOutput();
			size_t pos;
			if ((pos = s.find("ipv4.method:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				if(strncmp(buf, "auto", 4) == 0)
					nc.nDhcp = 1;
				else
					nc.nDhcp = 0;
				DPRINTF("%s\n", buf);
			}
#if 0
			if ((pos = s.find("connection.id:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				nc.strDevName = dev;
			}
#endif
			if ((pos = s.find("ipv4.addresses:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				std::string ss = buf;
				if(ss.find("ipv4") != std::string::npos)
				{
					if ((pos = s.find("IP4.ADDRESS[1]:")) != std::string::npos)
					{
						char buf[50];
						sscanf(s.c_str() + pos + 40, "%s", buf);
						char buff[50];
						memset(buff, 0, 50);
						int i;
						for (i = 0; i < strlen(buf); i++)
						{
							if(buf[i] != '/')
								buff[i] = buf[i];
							else
							{
								nc.strIp = buff;
								DPRINTF("%s\n", buff);
								break;
							}
						}
						memset(buff, 0, 50);
						int j = 0;
						i++;
						for(;i < strlen(buf); i++, j++)
						{
							buff[j] = buf[i];
							nc.strNetmask = prefix2mask(atoi(buff));
							DPRINTF("%s\n", nc.strNetmask.c_str());
						}
					}
					else if ((pos = s.find("IP4.地址[1]:")) != std::string::npos)
					{
						char buf[50];
						sscanf(s.c_str() + pos + 40, "%s", buf);
						char buff[50];
						memset(buff, 0, 50);
						int i;
						for (i = 0; i < strlen(buf); i++)
						{
							if(buf[i] != '/')
								buff[i] = buf[i];
							else
							{
								nc.strIp = buff;
								DPRINTF("%s\n", buff);
								break;
							}
						}
						memset(buff, 0, 50);
						int j = 0;
						i++;
						for(;i < strlen(buf); i++, j++)
						{
							buff[j] = buf[i];
							nc.strNetmask = prefix2mask(atoi(buff));
							DPRINTF("%s\n", nc.strNetmask.c_str());
						}
					}
				}
				else
				{
					char buff[50];
					memset(buff, 0, 50);
					int i;
					for (i = 0; i < strlen(buf); i++)
					{
						if(buf[i] != '/')
							buff[i] = buf[i];
						else
						{
							nc.strIp = buff;
							DPRINTF("%s\n", buff);
							break;
						}
					}
					memset(buff, 0, 50);
					int j = 0;
					i++;
					for(;i < strlen(buf); i++, j++)
					{
						buff[j] = buf[i];
						nc.strNetmask = prefix2mask(atoi(buff));
						DPRINTF("%s\n", nc.strNetmask.c_str());
					}
				}
			}
			else{
			}
			if ((pos = s.find("ipv4.gateway:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				nc.strGateway = buf;
				if(nc.strGateway.find("--") != std::string::npos)
				{
					nc.strGateway = "";
					if ((pos = s.find("IP4.GATEWAY:")) != std::string::npos)
					{
						char buf[50];
						sscanf(s.c_str() + pos + 40, "%s", buf);
						nc.strGateway = buf;
						DPRINTF("%s\n", buf);
					}
					else if ((pos = s.find("IP4.网关:")) != std::string::npos)
					{
						char buf[50];
						sscanf(s.c_str() + pos + 40, "%s", buf);
						nc.strGateway = buf;
						DPRINTF("%s\n", buf);
					}
					if(nc.strGateway.find("IP") != std::string::npos)
					{
						nc.strGateway = "";
					}
				}
				DPRINTF("%s\n", buf);
			}
			if ((pos = s.find("ipv4.dns:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				std::string ss = buf;
				if(ss.find("ipv4.dns-search:") == std::string::npos)
				{
					char buff[50];
					memset(buff, 0, 50);
					for(int i = 0; i < strlen(buf); i++)
					{
						if(buf[i] != ',')
						{
							buff[i] = buf[i];
						}
						else
							break;
					}
					nc.strDns1 = buff;
					DPRINTF("%s\n", buff);
				}
				else if ((pos = s.find("IP4.DNS[1]:")) != std::string::npos)
				{
					char buf[50];
					sscanf(s.c_str() + pos + 40, "%s", buf);
					nc.strDns1 = buf;
					DPRINTF("%s\n", buf);
				}
			}
			if(nc.nDhcp == 1)
			{
				if ((pos = s.find("IP4.DNS[1]:")) != std::string::npos)
				{
					char buf[50];
					sscanf(s.c_str() + pos + 40, "%s", buf);
					nc.strDns1 = buf;
					DPRINTF("%s\n", buf);
				}
			}

			if ((pos = s.find("IP4.DNS[2]:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				nc.strDns2 = buf;
				DPRINTF("%s\n", buf);
			}
			usleep(100*1000);
			return nc;
		}
		else
			usleep(100*1000);
	}
	usleep(100*1000);
	return nc;
}

NETWORK_STATUS NetOperation::GetNetStatus(std::string dev)
{
	IExternCall* pEc = CreateExternCall();
	char cmd[200];
	NETWORK_STATUS nc;
	if(dev.size() > 10)
	{
		DPRINTF("Invaild network device %s\n", dev.c_str());
		nc.strConnected = "NODEV";
		usleep(100*1000);
		return nc;
	}
	sprintf(cmd, "nmcli device show %s", dev.c_str());
	pEc->RunCommand(cmd);
	DPRINTF("%s\n", cmd);

	std::string s;
	while(1)
	{
		if(pEc->IsFinish())
		{
			s = pEc->GetOutput();
			size_t pos;
			if((pos = s.find("GENERAL.状态:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				DPRINTF("%s\n", buf);
				std::string ss = buf;
				if(ss.find("100") != std::string::npos)
					nc.strConnected = "TRUE";
				else
					nc.strConnected = "FALSE";
				nc.strDevName = dev;
			}
			if((pos = s.find("GENERAL.STATE:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				DPRINTF("%s\n", buf);
				std::string ss = buf;
				if(ss.find("100") != std::string::npos)
					nc.strConnected = "TRUE";
				else
					nc.strConnected = "FALSE";
				nc.strDevName = dev;
			}
			break;
		}
		else
			usleep(200000);
	}
	usleep(100*1000);
	if (nc.strConnected == "" || nc.strConnected.empty())
	{
		nc.strConnected = "NODEV";
		return nc;
	}
	return nc;
}


REMOTE_CONF NetOperation::GetDns(std::string dev)
{
	IExternCall* pEc = CreateExternCall();
	char cmd[200];
	REMOTE_CONF nc;
	if(dev.size() > 10)
	{
		usleep(100*1000);
		return nc;
	}
	sprintf(cmd, "nmcli con show %s", dev.c_str());
	pEc->RunCommand(cmd);
	std::string s;

	while(1)
	{
		if (pEc->IsFinish())
		{
			s = pEc->GetOutput();
			size_t pos;
		
			if ((pos = s.find("IP4.DNS[1]:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				nc.strDns1 = buf;
				DPRINTF("%s\n", buf);
			}
			if ((pos = s.find("IP4.DNS[2]:")) != std::string::npos)
			{
				char buf[50];
				sscanf(s.c_str() + pos + 40, "%s", buf);
				nc.strDns2 = buf;
				DPRINTF("%s\n", buf);
			}
			usleep(100*1000);
			return nc;
		}
		else
			usleep(200000);
	}
}

REMOTE_CONF& NetOperation::GetRemoteConfig()
{
	ICMyini* ini = createMyini();
	char fn[200];
	if (ini)
	{
		std::string eth0, eth1;
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
		REMOTE_CONF nc1 = GetDns(eth0);
		if(nc1.strDns1 != "")
			m_rc.strDns1 = nc1.strDns1;
		if(nc1.strDns2 != "")
			m_rc.strDns2 = nc1.strDns2;
		
		REMOTE_CONF nc2 = GetDns(eth1);
		if(nc2.strDns1 != "")
			m_rc.strDns1 = nc2.strDns1;
		if(nc2.strDns2 != "")
			m_rc.strDns2 = nc2.strDns2;

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

extern NetCommThread *pNetComm;

bool NetOperation::SetNetConfig(std::list<NETWORK_CONF>& m_listNetconf)
{
#if 0
	ICMyini *ini = createMyini();
#endif
	char fn[1024];

	//////////////////////////////////////////////////////////////////////////
	if(pNetComm)
	pNetComm->CloseConnect();
	//////////////////////////////////////////////////////////////////////////

	m_list.clear();
	std::list<NETWORK_CONF>::iterator itor;
	for (itor = m_listNetconf.begin(); itor != m_listNetconf.end(); ++itor)
	{
		NETWORK_CONF nc = *itor;
		m_list.push_back(nc);
#if 0
		sprintf(fn, "%s%s", ETHA, nc.strDevName.c_str());
		ini->load(fn);
#endif
		DPRINTF("%s %s %s %s\n",
			nc.strIp.c_str(),
			nc.strNetmask.c_str(),
			nc.strGateway.c_str(),
			nc.strDevName.c_str());

		std::string tmp;
		#if 0
		if (nc.nDhcp)
		{
			ini->write(" ", "BOOTPROTO", "dhcp");
			ini->write(" ", "IPADDR", " ");
			ini->write(" ", "PREFIX", " ");
			ini->write(" ", "GATEWAY", " ");
		}
		else
		{
			ini->write(" ", "BOOTPROTO", "none");
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
		ini->save(fn);
		#endif

		if(nc.nDhcp)
		{
			sprintf(fn, "nmcli connection modify %s ipv4.method auto ipv4.address '' ipv4.gateway ''", nc.strDevName.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}
		else
		{
			sprintf(fn,
				"nmcli connection modify %s ipv4.method manual ipv4.address '%s/%d' ipv4.gateway '%s'",
				nc.strDevName.c_str(),
				nc.strIp.c_str(),
				calcmask(nc.strNetmask),
				nc.strGateway.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}
		sprintf(fn,
			"nmcli con mod %s ipv4.dns \"%s 8.8.8.8\"",
			nc.strDevName.c_str(),
			nc.strDns1.c_str());//,
//			nc.strDns2.c_str());
		DPRINTF("%s\n", fn);
		system(fn);

			sprintf(fn,
				"nmcli con up %s",
				nc.strDevName.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}

	//////////////////////////////////////////////////////////////////////////
	//
	if(pNetComm)
	pNetComm->StartConnect();
	//////////////////////////////////////////////////////////////////////////
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
#if 0
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
	//////////////////////////////////////////////////////////////////////////
	if(pNetComm)
	pNetComm->CloseConnect();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	if(pNetComm)
	pNetComm->StartConnect();
	//////////////////////////////////////////////////////////////////////////
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
		sprintf(str, "[ContentOperation] FindDir: %s.", dir.c_str());
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}

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
						//Don't delete /storage/ftp, it's vsftp root directory.
						if(!(m_dir.at(i) == "/storage/ftp" || m_dir.at(i) == "storage/recv"))
						{
						char cmd[512];
						sprintf(cmd, "rm -rf %s", m_dir.at(i).c_str());
						if (gLog)
						{
							gLog->Write(LOG_NETCOMMU, cmd);
						}
						system(cmd);
					}
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

	m_Status = 1;
	sout.clear();
	memset(out, 0, 1024);

	return true; 
}

void mke2fs::doit()
{
	ICMyini* ini = createMyini();
	bool bRead = false;
	std::string tmp, cmd;
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
		system("fuser -ck /storage");
		sleep(2);
		res = umount("/storage");
		if(res != 0)
		{
			m_Status = 0;
			out[0] = -1;
			memcpy(out + 1, "umount disk failed.\n", 19);
			sout += (out+1);
			return;
		}

		cmd = "/sbin/mkfs.ext3 " + tmp;
		fp = popen(cmd.c_str(), "r");
		setvbuf(fp, NULL, _IONBF, 0);
		sout.clear();
		memset(out, 0, 1024);
		m_Status = 1;
		while(fgets(out+1, sizeof(out-1), fp) != NULL)
		{
			m_Status = 1;
			out[0] = 1;
			sout += (out+1);
// 			printf("%s", out+1);
		}
		out[0] = -1;
		pclose(fp);
		m_Status = 0;
		fp = NULL;
#if 0
		cmd = "/sbin/tune2fs -O ^has_journal " + tmp;
		system(cmd.c_str());
#endif
		MountDisk(DISK_REMOVEABLE);
		chdir("/storage");
		system("mkdir /storage/ftp");
		system("mkdir /storage/recv");

		break;
	case DISK_RAID:
		if(tmp != "")
		{
			system("fuser -ck /raid");
			res = umount("/raid");
			if(res != 0)
			{
				m_Status = 0;
				out[0] = -1;
				memcpy(out + 1, "umount disk failed.\n", 19);
				sout += (out+1);
				return;
			}
			cmd = "/sbin/mkfs.ext3 " + tmp;
			fp = popen(cmd.c_str(), "r");
	setvbuf(fp, NULL, _IONBF, 0);
	sout.clear();
			memset(out, 0, 1024);
			m_Status = 1;
	while(fgets(out+1, sizeof(out-1), fp) != NULL)
	{
		m_Status = 1;
		out[0] = 1;
 		sout += (out+1);
		//out = buf;
// 				 		printf("%s", out+1);
	}
	out[0] = -1;
	pclose(fp);
	m_Status = 0;
	fp = NULL;
#if 0
			cmd = "/sbin/tune2fs -O ^has_journal " + tmp;
			system(cmd.c_str());
#endif
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
	
	//try ext4 filesystem first
	switch(m_type)
	{
	case DISK_REMOVEABLE:
		res = mount(tmp.c_str(), "/storage", "ext4", MS_NOATIME|MS_NODIRATIME, NULL);
		break;
	case DISK_RAID:
		if(tmp != "")
			res = mount(tmp.c_str(), "/raid", "ext4", MS_NOATIME|MS_NODIRATIME, NULL);
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

	//try ext3 filesystem
	switch(m_type)
	{
	case DISK_REMOVEABLE:
		res = mount(tmp.c_str(), "/storage", "ext3", MS_NOATIME|MS_NODIRATIME, NULL);
		break;
	case DISK_RAID:
		if(tmp != "")
			res = mount(tmp.c_str(), "/raid", "ext3", MS_NOATIME|MS_NODIRATIME, NULL);
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

	//try ext2 filesystem
	switch(m_type)
	{
	case DISK_REMOVEABLE:
		res = mount(tmp.c_str(), "/storage", "ext2", MS_NOATIME|MS_NODIRATIME, NULL);
		break;
	case DISK_RAID:
		if(tmp != "")
			res = mount(tmp.c_str(), "/raid", "ext2", MS_NOATIME|MS_NODIRATIME, NULL);
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

void System::ClearSystem()
{
	system("rm -rf /var/log/audit/audit.log.*");
	system("rm -rf /var/log/cups/access_log-*");
	system("rm -rf /var/log/cups/error_log-*");
	system("rm -rf /var/log/*.gz");
	system("rm -rf /var/log/CineCast/*");
	system("rm -rf /tmp/*");
	system("rm -rf /var/tmp/*");
}

#include <stdio.h>
#include <stdlib.h>

//new class
bool USB::USB_Mount()
{
	m_dir.clear();
	std::string path = "/var/run/media/cinecast";
	bMount = find_dir(path);
	if(!bMount)
		bCopy = false;
	return bMount;
}

bool USB::find_dir(std::string dir)
{
	char str[512];
	fs::path p(dir);
	try
	{
// 		sprintf(str, "[USB] find_dir: %s", dir.c_str());
// 		if (gLog)
// 		{
// 			gLog->Write(LOG_NETCOMMU, str);
// 		}

		if(!fs::exists(p))
		{
			DPRINTF("path no exists\n");
			return false;
		}
		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(p); itr != end_itr; ++itr)
		{
			try
			{
				if(fs::is_directory(*itr))
				{
#if 0
					sprintf(str, "[USB] MountDir: %s", (*itr).path().native().c_str());
					if (gLog)
		{
						gLog->Write(LOG_NETCOMMU, str);
					}
#endif
					m_dir.push_back((*itr).path().native().c_str());
					return true;
				}
		}
			catch(const fs::filesystem_error& e)
			{
				sprintf(str, "[USB] FindDir: Skip the IO error, except: %s.", e.what());
				if (gLog)
		{
					gLog->Write(LOG_ERROR, str);
				}
				DPRINTF("[USB] FindDir: Skip the IO error, except: %s.\n", e.what());
		}
		}
	}
	catch(const fs::filesystem_error& e)
		{
		sprintf(str, "[USB] FindDir: except: %s.", e.what());
		if (gLog)
			 {
			gLog->Write(LOG_ERROR, str);
		}
		DPRINTF("[USB] FindDir: except: %s.\n", e.what());
			 }
	return false;
		}

bool USB::USB_UnMount()
		{
		}

std::string USB::GetMountPoint()
{
	std::string s;
	if(m_dir.size() > 0)
		return m_dir.at(0);
	else
		return s;
}

bool USB::ReadyUpdate()
{
	char cmd[500];
	if(bCopy)
		return true;
	else
	{
		if(bMount)
		{
			std::string src = m_dir.at(0);
			src += "/leonisupdate.zt";
			fs::path src_path(src);
			if(!fs::exists(src_path))
			{
#if 0
				sprintf(cmd, "[USB] no src: %s", src.c_str());
				if (gLog)
{
					gLog->Write(LOG_ERROR, cmd);
				}
#endif
				DPRINTF("[USB] no src: %s\n", src.c_str());
				return false;
}
			//20160612: Fix pathname with space
			sprintf(cmd, "cp -f \"%s\" /home/leonis/update/leonisupdate.zt", src.c_str());
			//---------------------------------
			system(cmd);
			std::string dest = "/home/leonis/update/leonisupdate.zt";
			fs::path dest_path(dest);
			if(fs::exists(dest_path))
{
				bCopy = true;
				return true;
			}
		}
	}
	return false;
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
		else
			usleep(200000);
	}
}

extern bool gShutdownAfterFinish;
void System::SetShutdownFlag(bool bFlag)
{
#if 0
	ICMyini* ini = createMyini();
	std::string tmp;
	if(ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			if(bFlag)
			{
				ini->write(" ", "ShutdownFlag", "TRUE");
			}
			else
			{
				ini->write(" ", "ShutdownFlag", "FALSE");
			}
			ini->save("/etc/CineCast/CineCast.cfg");
			releaseMyini(ini);
			DPRINTF("Save cfg\n");
			gShutdownAfterFinish = bFlag;
		}
	}
#else
	gShutdownAfterFinish = bFlag;
#endif
}

bool System::GetShutdownFlag()
{
#if 0
	ICMyini* ini = createMyini();
	std::string tmp;
	if(ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			if(ini->read(" ", "ShutdownFlag", tmp))
			{
				if (tmp == "TRUE")
				{
					gShutdownAfterFinish = true;
				}
			}
			else
				gShutdownAfterFinish = false;
		}
		releaseMyini(ini);
	}
#endif
	return gShutdownAfterFinish;
}