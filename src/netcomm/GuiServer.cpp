#include "GuiServer.h"
#include "UiProtocal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dvb/mytype.h>

#define GUI_PORT (10003)

GuiServer::GuiServer():m_status(0), m_pThread(0)
{

}

GuiServer::~GuiServer()
{
	Stop();
	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = 0;
	}
}

bool GuiServer::Init()
{
	DPRINTF("[GuiServer] Init\n");
	Start();
	return true;
}

bool GuiServer::Start()
{
	DPRINTF("[GuiServer] Start\n");
	Stop();
	m_status = RUN;
	return true;
}

bool GuiServer::Stop()
{
	m_status = STOP;
	DPRINTF("[GuiServer] Stop\n");
	m_SrvSocket.Destroy();
	std::list<GuiThread*>::iterator itor;
	for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
	{
		GuiThread* client = *itor;
		client->Stop();
		while(!client->isStop()) usleep(100000);
		client->stop();
		delete client;
	}
	m_listClient.clear();
	return true;
}

void GuiServer::doit()
{
	try
	{
		if (m_SrvSocket.Create(AF_INET, SOCK_STREAM, 0))
		{
			struct sockaddr_in addr_in;
			memset(&addr_in, 0, sizeof(sockaddr_in));
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(GUI_PORT);
			addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
			int error = 0;
			do 
			{
				t_timeout tm = 3000;
				if(m_SrvSocket.Bind(&addr_in))
				{
					if (!m_SrvSocket.Listen(10))
						throw -1;
				}
				else
					throw -1;

			} while (error);
		}
	}
	catch (int &)
	{
		DPRINTF("[GuiServer] Server Socket error\n");
		return;
	}
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				if (m_pThread == NULL)
				{
					m_pThread = new GuiThread;
				}
				t_timeout tm = 100;
				if(m_SrvSocket.Accept(m_pThread->m_socket, NULL, NULL, &tm) == 0)
				{
					m_listClient.push_back(m_pThread);
					m_pThread->Init();
					if (m_pThread->status() == brunt::thread_ready)
					{
						m_pThread->start();
						DPRINTF("Start Gui thread\n");
					}
					m_pThread = NULL;
				}
			}
			break;
		case STOP:
			DPRINTF("[GuiServer] Stop in loop\n");
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

GuiThread::GuiThread():m_status(0)
{

}

GuiThread::~GuiThread()
{
	Stop();
}

bool GuiThread::Init()
{
	DPRINTF("[GuiThread] Init\n");
	Start();
	return true;
}

bool GuiThread::Start()
{
	DPRINTF("[GuiThread] Start\n");
	Stop();
	m_status = RUN;
	bStop = false;
	return true;
}

bool GuiThread::Stop()
{
	m_status = STOP;
	DPRINTF("[GuiThread] Stop\n");

	usleep(100000);
	return true;
}

void GuiThread::doit()
{
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
#if 0
				char buf[255];
				memset(buf, 0, 255);
				size_t getsize;
				int error;
				if((error = m_socket.Receive(buf, 255, getsize)) != 0)
				{
					DPRINTF("[GuiThread] Receive error %d\n", error);
					m_status = STOP;
				}
				else
				{
					DPRINTF("%s %d %x\n", buf, error, this);
				}
#else
				if (!UiProcessFilter())
				{
					DPRINTF("[GuiThread] Stop - Receive error\n");
					m_status = STOP;
				}
#endif
			}
			break;
		case STOP:
			DPRINTF("[GuiThread] Stop in loop %d\n", this);
			bStop = true;
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

bool GuiThread::Read(char* buf, int rSize, int& nRead)
{
	try
	{
		int i = 0;
		int n = sizeof(KL);
		size_t getsize;
		int error = 1;
		KL * pKL;
		t_timeout tm = 1;

		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
				throw -1;
			else
			{
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else/* if(getsize == n)*/
				{
					pKL = (KL*)buf;
					if (pKL->m_pkgHead == 0x7585)
					{
						error = 0;
					}
					else
					{
						i = 0;
						n = sizeof(KL);
					}
				}
			}
		} while (error && (m_status != STOP));
		n = pKL->m_length;

		i = sizeof(KL);
		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
				throw -1;
			else
			{
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else
				{
					nRead = pKL->m_length + sizeof(KL);
					return true;
				}
			}
		} while (error && (m_status != STOP));
	}
	catch (int&)
	{
		DPRINTF("[GuiThread] Receive error\n");
		return false;
	}
}

bool GuiThread::Write(char* buf, int wSize, int& nWrite)
{
	try 
	{
		int error = 1;
		int i = 0;
		int n = wSize;
		size_t writesize;
		t_timeout tm = 1;
		do 
		{
			if(m_socket.Send(buf + i, n, writesize, &tm) != 0)
				throw -1;
			else
			{
				if (n > writesize)
				{
					n -= writesize;
					i += writesize;
				}
				else
				{
					nWrite = wSize;
					return true;
				}
			}
		} while (error && (m_status != STOP));

	}
	catch(int &)
	{
		DPRINTF("[GuiThread] Send error\n");
		return false;
	}
}

bool GuiThread::UiProcessFilter()
{
	char buf[2048];
	memset(buf, 0, 2048);
	int getsize;
	if (Read(buf, 2048, getsize))
	{
		KL* pKL = (KL*)buf;
		if (pKL->m_pkgHead == 0x7585)
		{
		switch(pKL->m_keyID)
		{
		case S_GET_SATELLITE:
				return S_GetSatellite(buf);
			case S_GET_RECEIVE:
				return S_GetReceive(buf);
			case S_GET_CONFIG:
				return C_GetTuner(buf);
			case S_SET_CONFIG:
				return C_SetTuner(buf);
			case N_GET_CONFIG:
				return N_GetConfig(buf);
			case N_SET_CONFIG:
				return N_SetConfig(buf);
			case N_GET_REMOTE:
				return N_GetRemote(buf);
			case N_SET_REMOTE:
				return N_SetRemote(buf);
		}
		}
		else
			return ErrorProtocol(buf);
	}
	return false;
}

#include <dvb/SatelliteRecv.h>
bool GuiThread::ErrorProtocol(char* buf)
{
	KL *pKL = (KL*)buf;
	pKL->m_length = 1;
	buf[sizeof(KL)] = -1;
	int setsize = pKL->m_length + sizeof(KL);
	return Write(buf, setsize, setsize);
}

bool GuiThread::S_GetSatellite(char* buf)
{
	extern TUNER_INFO gInfo;
	KL *pKL = (KL*)buf;
	pKL->m_length = sizeof(gInfo);
	int setsize = pKL->m_length + sizeof(KL);
	memcpy(buf + sizeof(KL), &gInfo, sizeof(gInfo));
	return Write(buf, setsize, setsize);
}

bool GuiThread::S_GetReceive(char* buf)
{
	extern RECEIVE_INFO gRecv;

	SL FN, Uuid, IssueDate, Issuer, Creator;
	FN.m_sID = RECEIVE_FILM_NAME;
	FN.m_length = gRecv.strFilmName.size();

	Uuid.m_sID = RECEIVE_FILM_UUID;
	Uuid.m_length = gRecv.strUuid.size();

	IssueDate.m_sID = RECEIVE_FILM_ISSUE_DATE;
	IssueDate.m_length = gRecv.strIssueDate.size();

	Issuer.m_sID = RECEIVE_FILM_ISSUER;
	Issuer.m_length = gRecv.strIssuer.size();

	Creator.m_sID = RECEIVE_FILM_CREATOR;
	Creator.m_length = gRecv.strCreator.size();

	int datalen = sizeof(uint64) * 6
		+ sizeof(uint32)
		+ sizeof(uint16);

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos, &gRecv, datalen);
	pos += datalen;

	memcpy(pos, &FN, sizeof(FN));
	pos += sizeof(FN);
	memcpy(pos, gRecv.strFilmName.c_str(), FN.m_length);
	pos += FN.m_length;

	memcpy(pos, &Uuid, sizeof(Uuid));
	pos += sizeof(Uuid);
	memcpy(pos, gRecv.strUuid.c_str(), Uuid.m_length);
	pos += Uuid.m_length;

	memcpy(pos, &IssueDate, sizeof(IssueDate));
	pos += sizeof(IssueDate);
	memcpy(pos, gRecv.strIssueDate.c_str(), IssueDate.m_length);
	pos += IssueDate.m_length;

	memcpy(pos, &Issuer, sizeof(Issuer));
	pos += sizeof(Issuer);
	memcpy(pos, gRecv.strIssuer.c_str(), Issuer.m_length);
	pos += Issuer.m_length;

	memcpy(pos, &Creator, sizeof(Creator));
	pos += sizeof(Creator);
	memcpy(pos, gRecv.strCreator.c_str(), Creator.m_length);
	pos += Creator.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::C_GetTuner(char* buf)
{
	extern TUNER_CONF gTuner;

	SL DevName, DelSys, Fec, Mod, Roll, Pol;
	
	DevName.m_sID = TUNER_DEV_NAME;
	DevName.m_length = gTuner.strDevName.size();

	DelSys.m_sID = TUNER_DEL_SYS;
	DelSys.m_length = gTuner.strDelSys.size();

	Fec.m_sID = TUNER_FEC;
	Fec.m_length = gTuner.strFec.size();

	Mod.m_sID = TUNER_MOD;
	Mod.m_length = gTuner.strModulation.size();

	Roll.m_sID = TUNER_ROLL_OFF;
	Roll.m_length = gTuner.strRollOff.size();

	Pol.m_sID = TUNER_POL;
	Pol.m_length = gTuner.strPolVert.size();

	int datalen = sizeof(unsigned int) * 5;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos, &gTuner, datalen);
	pos += datalen;

	memcpy(pos, &DevName, sizeof(DevName));
	pos += sizeof(DevName);
	memcpy(pos, gTuner.strDevName.c_str(), DevName.m_length);
	pos += DevName.m_length;

	memcpy(pos, &DelSys, sizeof(DelSys));
	pos += sizeof(DelSys);
	memcpy(pos, gTuner.strDelSys.c_str(), DelSys.m_length);
	pos += DelSys.m_length;

	memcpy(pos, &Fec, sizeof(Fec));
	pos += sizeof(Fec);
	memcpy(pos, gTuner.strFec.c_str(), Fec.m_length);
	pos += Fec.m_length;

	memcpy(pos, &Mod, sizeof(Mod));
	pos += sizeof(Mod);
	memcpy(pos, gTuner.strModulation.c_str(), Mod.m_length);
	pos += Mod.m_length;

	memcpy(pos, &Roll, sizeof(Roll));
	pos += sizeof(Roll);
	memcpy(pos, gTuner.strRollOff.c_str(), Roll.m_length);
	pos += Roll.m_length;

	memcpy(pos, &Pol, sizeof(Pol));
	pos += sizeof(Pol);
	memcpy(pos, gTuner.strPolVert.c_str(), Pol.m_length);
	pos += Pol.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

#include "BaseOperation.h"

bool GuiThread::C_SetTuner(char* buf)
{
	extern TUNER_CONF gTuner;
	extern char bTunerChange;
	int datalen = sizeof(unsigned int) * 5;
	memcpy(&gTuner, buf + sizeof(KL), datalen);

	KL *pKL = (KL*) buf;
	int len = pKL->m_length - datalen;
	int pos = sizeof(KL) + datalen;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
		std::string tmp;
		pos += sizeof(SL);
		len -= sizeof(SL);
		if (sl->m_length > 0)
		{
			char *s = new char[sl->m_length+1];
			memset(s, 0, sl->m_length+1);
			memcpy(s, buf+pos, sl->m_length);
			tmp = s;
			pos += sl->m_length;
			len -= sl->m_length;
			delete[] s;
		}
		else
		{
			tmp = "";
		}

		switch(sl->m_sID)
		{
		case TUNER_DEV_NAME:
			gTuner.strDevName = tmp;
			break;
		case  TUNER_DEL_SYS:
			gTuner.strDelSys = tmp;
			break;
		case TUNER_FEC:
			gTuner.strFec = tmp;
			break;
		case TUNER_MOD:
			gTuner.strModulation = tmp;
			break;
		case TUNER_ROLL_OFF:
			gTuner.strRollOff = tmp;
			break;
		case TUNER_POL:
			gTuner.strPolVert = tmp;
			break;
		}
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	SatelliteConfOperation sco;
	sco.WriteConfig();
	bTunerChange = 1;
	return Write(buf, setsize, setsize);
}

#include "BaseOperation.h"

bool GuiThread::N_GetConfig(char* buf)
{
	NetOperation no;

	std::list<NETWORK_CONF> m_list = no.GetNetConfig();

	SL Dhcp, DevName, Ip, Netmask, Gateway;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	
	std::list<NETWORK_CONF>::iterator itor;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		NETWORK_CONF nc = *itor;

		Dhcp.m_sID = NET_DHCP;
		Dhcp.m_length = sizeof(nc.nDhcp);

		DevName.m_sID = NET_DEV_NAME;
		DevName.m_length = nc.strDevName.size();

		Ip.m_sID = NET_IP;
		Ip.m_length = nc.strIp.size();

		Netmask.m_sID = NET_NETMASK;
		Netmask.m_length = nc.strNetmask.size();

		Gateway.m_sID = NET_GATEWAY;
		Gateway.m_length = nc.strGateway.size();

		memcpy(pos, &Dhcp, sizeof(Dhcp));
		pos += sizeof(Dhcp);
		memcpy(pos, &nc.nDhcp, Dhcp.m_length);
		pos += Dhcp.m_length;

		memcpy(pos, &DevName, sizeof(DevName));
		pos += sizeof(DevName);
		memcpy(pos, nc.strDevName.c_str(), DevName.m_length);
		pos += DevName.m_length;

		memcpy(pos, &Ip, sizeof(Ip));
		pos += sizeof(Ip);
		memcpy(pos, nc.strIp.c_str(), Ip.m_length);
		pos += Ip.m_length;

		memcpy(pos, &Netmask, sizeof(Netmask));
		pos += sizeof(Netmask);
		memcpy(pos, nc.strNetmask.c_str(), Netmask.m_length);
		pos += Netmask.m_length;

		memcpy(pos, &Gateway, sizeof(Gateway));
		pos += sizeof(Gateway);
		memcpy(pos, nc.strGateway.c_str(), Gateway.m_length);
		pos += Gateway.m_length;

	}

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::N_SetConfig(char* buf)
{
	std::list<NETWORK_CONF> m_list;

	KL *pKL = (KL*) buf;

	int len = pKL->m_length;
	int pos = sizeof(KL);
	NETWORK_CONF nc;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
		std::string tmp;
		pos += sizeof(SL);
		len -= sizeof(SL);
		if (sl->m_length > 0)
		{
			char *s = new char[sl->m_length+1];
			memset(s, 0, sl->m_length+1);
			memcpy(s, buf+pos, sl->m_length);
			tmp = s;
			pos += sl->m_length;
			len -= sl->m_length;
			delete[] s;
		}
		else
		{
			tmp = "";
		}

		switch(sl->m_sID)
		{
		case NET_DHCP:
			memcpy(&nc.nDhcp, buf + pos - sl->m_length, sl->m_length);
			break;
		case  NET_DEV_NAME:
			nc.strDevName = tmp;
			break;
		case NET_IP:
			nc.strIp = tmp;
			break;
		case NET_NETMASK:
			nc.strNetmask = tmp;
			break;
		case NET_GATEWAY:
			nc.strGateway = tmp;
			m_list.push_back(nc);
			break;
		}
	}

	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	bool res = Write(buf, setsize, setsize);
	NetOperation no;
	no.SetNetConfig(m_list);
	return res;
}

bool GuiThread::N_GetRemote(char* buf)
{
	NetOperation no;

	REMOTE_CONF rc = no.GetRemoteConfig();

	SL Dns1, Dns2, Remote;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);

	Dns1.m_sID = REMOTE_DNS;
	Dns1.m_length = rc.strDns1.size();

	Dns2.m_sID = REMOTE_DNS;
	Dns2.m_length = rc.strDns2.size();

	Remote.m_sID = REMOTE_SERVER;
	Remote.m_length = rc.strRemote.size();

	memcpy(pos, &Dns1, sizeof(Dns1));
	pos += sizeof(Dns1);
	memcpy(pos, rc.strDns1.c_str(), Dns1.m_length);
	pos += Dns1.m_length;

	memcpy(pos, &Dns2, sizeof(Dns2));
	pos += sizeof(Dns2);
	memcpy(pos, rc.strDns2.c_str(), Dns2.m_length);
	pos += Dns2.m_length;

	memcpy(pos, &Remote, sizeof(Remote));
	pos += sizeof(Remote);
	memcpy(pos, rc.strRemote.c_str(), Remote.m_length);
	pos += Remote.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::N_SetRemote(char* buf)
{
	REMOTE_CONF rc;

	KL *pKL = (KL*) buf;
	int len = pKL->m_length;
	int pos = sizeof(KL);
	int dns = 0;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
		std::string tmp;
		pos += sizeof(SL);
		len -= sizeof(SL);
		if (sl->m_length > 0)
		{
			char *s = new char[sl->m_length+1];
			memset(s, 0, sl->m_length+1);
			memcpy(s, buf+pos, sl->m_length);
			tmp = s;
			pos += sl->m_length;
			len -= sl->m_length;
			delete[] s;
		}
		else
		{
			tmp = "";
		}

		switch(sl->m_sID)
		{
		case REMOTE_DNS:
			if(dns == 0)
			{
				rc.strDns1 = tmp;
				dns++;
			}
			else
				rc.strDns2 = tmp;
			break;
		case  REMOTE_SERVER:
			rc.strRemote = tmp;
			break;
		}
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	bool res = Write(buf, setsize, setsize);
	NetOperation no;
	no.SetRemoteConfig(rc);
	return res;
}