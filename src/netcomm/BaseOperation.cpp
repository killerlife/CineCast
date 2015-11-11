#include "BaseOperation.h"
#include "ini.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NETDEV_COUNT 2
#define ETH "/etc/sysconfig/network-scripts/ifcfg-enp6s"
#define ETHA "/etc/sysconfig/network-scripts/ifcfg-"
#define RESOLV "/etc/resolv.conf"

using namespace brunt;

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

	if (ini)
	{
		for (int i = 0; i < NETDEV_COUNT; i++)
		{
			sprintf(fn, "%s%d", ETH, i);
			if(ini->load(fn))
			{
				std::string tmp;
				NETWORK_CONF nc;
				ini->read(" ", "BOOTPROTO", tmp);
				if(tmp == "none")
					nc.nDhcp = 0;
				else
					nc.nDhcp = 1;
				ini->read(" ", "DEVICE", nc.strDevName);
				ini->read(" ", "IPADDR", nc.strIp);
				ini->read(" ", "PREFIX", tmp);
				nc.strNetmask = prefix2mask(atoi(tmp.c_str()));
				ini->read(" ", "GATEWAY", nc.strGateway);
				m_list.push_back(nc);
			}
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
		if(ini->load("./CineCast.cfg"))
		{
			std::string tmp;
			if(ini->read(" ", "SERVER", tmp))
				m_rc.strRemote = tmp;
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
			sprintf(fn, "sudo nmcli ");
		}
		else
		{
			sprintf(fn,
				"sudo nmcli connection modify %s ipv4.address '%s/%d' ipv4.gateway '%s'",
				nc.strDevName.c_str(),
				nc.strIp.c_str(),
				calcmask(nc.strNetmask),
				nc.strGateway.c_str());
			DPRINTF("%s\n", fn);
			system(fn);

			sprintf(fn,
				"sudo nmcli con up %s",
				nc.strDevName.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
		}
		ini->save(fn);
	}
// 	system("sudo /etc/init.d/network restart");
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
		ini->load("./CineCast.cfg");
		ini->write(" ", "SERVER", m_remoteConf.strRemote.c_str());
		ini->save("./CineCast.cfg");
	}
#if 1
	ICMyini* ini2 = createMyini();
	if (ini2)
	{
		for(int i = 0; i < NETDEV_COUNT; i++)
		{
			sprintf(fn, "%s%d", ETH, i);
			DPRINTF("%s\n", fn);
			ini2->load(fn);
			if(!m_remoteConf.strDns1.empty())
				ini2->write(" ", "DNS1", m_remoteConf.strDns1.c_str());
			if(!m_remoteConf.strDns2.empty())
				ini2->write(" ", "DNS2", m_remoteConf.strDns2.c_str());
			std::string tmp;
			ini2->read(" ", "DEVICE", tmp);
// 			ini2->save(fn);
			sprintf(fn,
				"sudo nmcli con mod %s ipv4.dns \"%s %s\"",
				tmp.c_str(),
				m_remoteConf.strDns1.c_str(),
				m_remoteConf.strDns2.c_str());
			DPRINTF("%s\n", fn);
			system(fn);

			sprintf(fn,
				"sudo nmcli con up %s",
				tmp.c_str());
			DPRINTF("%s\n", fn);
			system(fn);
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
		if(ini->load("./CineCast.cfg"))
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

		return ini->save("./CineCast.cfg");
	}
	return false;
}