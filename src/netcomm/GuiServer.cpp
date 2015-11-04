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
		DPRINTF("KL Size %d\n", n);
		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
				throw -1;
			else
			{
				DPRINTF("Read %d n%d\n", getsize, n);
				DPRINTF("%x %x %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else/* if(getsize == n)*/
				{
					pKL = (KL*)buf;
					if (pKL->m_pkgHead == 0x7585)
					{
						DPRINTF("Right head\n");
						error = 0;
					}
					else
					{
						i = 0;
						n = sizeof(KL);
					}
				}
			}
		} while (error || (m_status != STOP));
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
		} while (error || (m_status != STOP));
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
		} while (error || (m_status != STOP));

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
		switch(pKL->m_keyID)
		{
		case S_GET_SATELLITE:
			//return S_GetSatellite(buf);
			break;
		}
		DPRINTF("%s %x\n", buf, this);
		if(!Write(buf, getsize, getsize))
		{
			DPRINTF("[GuiThread] Send errord\n");
			m_status = STOP;
		}
	}
	return false;
}

#include <dvb/SatelliteRecv.h>
bool GuiThread::S_GetSatellite(char* buf)
{
	extern TUNER_INFO gInfo;
	KL *pKL = (KL*)buf;
	pKL->m_length = sizeof(gInfo);
	int setsize = pKL->m_length + sizeof(KL);
	memcpy(buf + sizeof(KL), &gInfo, sizeof(gInfo));
	return Write(buf, setsize, setsize);
}