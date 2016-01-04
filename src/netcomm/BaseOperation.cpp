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
		ini->write(" ", "PORT", fn);
		ini->save("/etc/CineCast/CineCast.cfg");
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
	return false;
}

ContentOperation::ContentOperation()
{
	std::vector<int> srcList;
	srcList.push_back(PST_USB); //new   
	srcList.push_back(PST_HDD);
	getIContentManager()->update(srcList);
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
	}
	return m_Content;
}

int ContentOperation::UpdateProgramList(std::vector<int> srcList)
{
	return getIContentManager()->update(srcList);
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

mke2fs::mke2fs():fp(NULL), m_Status(0)
{

}

mke2fs::~mke2fs()
{
	CActiveThread::stop();
}

bool mke2fs::FormatDisk()
{
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
	fp = popen("mke2fs /dev/sdb1", "r");
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

void System::Reboot()
{
	system("/sbin/reboot");
}

void System::Shutdown()
{
	system("/sbin/init 0");
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