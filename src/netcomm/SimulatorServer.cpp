#include "SimulatorServer.h"
#include "UiProtocal.h"
#include "BaseOperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dvb/mytype.h>

#include <log/Log.h>
extern ILog* gLog;
extern uint32* GetDebugCommand();
extern uint32 gDebugID;

extern void print_hex(char* buf, int len);

SimulatorServer gSimulatorServer;
SimulatorServer* CreateSimulatorServer()
{
	return &gSimulatorServer;
}

void ReleaseSimulatorServer(SimulatorServer* pGui)
{

}

#define SIMULATOR_PORT (10009)

SimulatorServer::SimulatorServer():m_status(0), m_pThread(0)
{
}

SimulatorServer::~SimulatorServer()
{
	Stop();
	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = 0;
	}
}

bool SimulatorServer::Init()
{
	DPRINTF("[SimulatorServer] Init\n");
	Start();
	return true;
}

bool SimulatorServer::Start()
{
	DPRINTF("[SimulatorServer] Start\n");
	Stop();
	m_status = RUN;
	return true;
}

bool SimulatorServer::Stop()
{
	m_status = STOP;
	DPRINTF("[SimulatorServer] Stop\n");
	m_SrvSocket.Destroy();
	std::list<SimulatorThread*>::iterator itor;
	for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
	{
		SimulatorThread* client = *itor;
		client->Stop();
		while(!client->isStop()) usleep(100000);
		client->stop();
		delete client;
	}
	m_listClient.clear();
	return true;
}

void SimulatorServer::doit()
{
	char str[512];
	bool bSuccess = false;
	int i = 0;
	while(m_status != STOP && bSuccess == false)
	{
		try
		{
			if((i % 5) == 0)
			{
				//DPRINTF("Create socket\n");
				if (m_SrvSocket.Create(AF_INET, SOCK_STREAM, 0))
				{
					struct sockaddr_in addr_in;
					memset(&addr_in, 0, sizeof(sockaddr_in));
					addr_in.sin_family = AF_INET;
					addr_in.sin_port = htons(SIMULATOR_PORT);
					addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
					int error = 0;
					do 
					{
						t_timeout tm = 3000;
						if(m_SrvSocket.Bind(&addr_in))
						{
							if (!m_SrvSocket.Listen(20))
								throw -1;
							bSuccess = true;
							DPRINTF("Bind success\n");
						}
						else
							throw -2;

					} while (error);
				}
				else
				{
					//DPRINTF("Create failure\n");
				}
				i = 0;
			}
			else
			{
				i++;
				sleep(1);
			}
		}
		catch (int &a)
		{
			m_SrvSocket.Destroy();
			switch(a)
			{
			case -1:
				sprintf(str, "[SimulatorServer] Server Socket Listen error: %d.", status());
				break;
			case -2:
				sprintf(str, "[SimulatorServer] Server Socket Bind error: %d.", status());
				break;
			default:
				sprintf(str, "[SimulatorServer] Server Socket unknow error.", status());
			}
			DPRINTF("%s\n", str);
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			i++;
			sleep(1);
			//		return;
		}
	}
	DPRINTF("Run\n");
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				if (m_pThread == NULL)
				{
					m_pThread = new SimulatorThread;
				}
				t_timeout tm = 100;
				if(m_SrvSocket.Accept(m_pThread->m_socket, NULL, NULL, &tm) == 0)
				{
					m_listClient.push_back(m_pThread);
					m_pThread->Init();
					if (m_pThread->status() == brunt::thread_ready)
					{
						m_pThread->start();
						DPRINTF("Start Simlator thread\n");
					}
					m_pThread = NULL;
				}
			}
			break;
		case STOP:
			DPRINTF("[SimulatorServer] Stop in loop\n");
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

SimulatorThread::SimulatorThread():m_status(0)
{
 	pDebugCmd = GetDebugCommand();
}

SimulatorThread::~SimulatorThread()
{
	Stop();
}

bool SimulatorThread::Init()
{
	DPRINTF("[SimulatorThread] Init\n");
	Start();
	return true;
}

bool SimulatorThread::Start()
{
	DPRINTF("[SimulatorThread] Start\n");
	Stop();
	m_status = RUN;
	bStop = false;
	return true;
}

bool SimulatorThread::Stop()
{
	m_status = STOP;
	DPRINTF("[SimulatorThread] Stop\n");

	usleep(100000);
	return true;
}

void SimulatorThread::doit()
{
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				try {
					UiProcessFilter();
				}
				catch (int&)
				{
					DPRINTF("throw\n");
					m_socket.Destroy();
					m_status = STOP;
				}
			}
			break;
		case STOP:
			DPRINTF("[SimulatorThread] Stop in loop %d\n", this);
			bStop = true;
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

bool SimulatorThread::Read(char* buf, int rSize, int& nRead)
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
		DPRINTF("[SimulatorThread] Receive error\n");
		return false;
	}
}

bool SimulatorThread::Write(char* buf, int wSize, int& nWrite)
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
		DPRINTF("[SimulatorThread] Send error\n");
		return false;
	}
}

bool SimulatorThread::UiProcessFilter()
{
	char buf[4096];
	memset(buf, 0, 4096);
	size_t getsize;

// 	DPRINTF("Read Head\n");
	int err = m_socket.Receive(buf, sizeof(KL), getsize);
	if(err < 0)
		throw -1;

		KL* pKL = (KL*)buf;
// 	DPRINTF("len: %d kind:%03x\n", pKL->m_length, pKL->m_keyID);
// 	print_hex(buf, sizeof(KL));
	getsize = 0;
		if (pKL->m_pkgHead == 0x7585)
		{
// 		DPRINTF("Read Body %d\n", pKL->m_length);
		err = m_socket.Receive(buf + sizeof(KL), pKL->m_length, getsize);
// 		DPRINTF("Read Finish\n");
		if(err < 0)
			throw -1;
			switch(pKL->m_keyID)
			{
			case 0x1ff:
				return NotifyProc(buf);
			case 0x2ff:
				return StartProc(buf);
			case 0x3ff:
				return FinishProc(buf);
			case 0x4ff:
				return CancelProc(buf);
			case 0xfe:
				return PatProc(buf);
			case 0x20:
				return PmtProc(buf);
			default:
				return DataProc(buf);
			}
		}
		else
			return ErrorProtocol(buf);
	return false;
}

char SimDataBuf[10][4096];
int SimBufPos = 0;

#include <dvb/SatelliteRecv.h>
bool SimulatorThread::ErrorProtocol(char* buf)
{
	char str[512];
	KL *pKL = (KL*)buf;
// 	if (gLog)
// 	{
// 		sprintf(str, "[SimulatorThread] Received error protocol: %04X.", pKL->m_pkgHead);
// 		gLog->Write(LOG_ERROR, str);
// 	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = -1;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::NotifyProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x1ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
// 	print_hex(buf + sizeof(KL), pKL->m_length);
// 	print_hex(pos, pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}
	
	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::StartProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x2ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::FinishProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x3ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
	DPRINTF("SimPos=%d\n", SimBufPos);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::CancelProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x4ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::PatProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0xfe;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::PmtProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x20;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	print_hex(pos, pKL->m_length);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
}

bool SimulatorThread::DataProc(char* buf)
{
	KL *pKL = (KL*)buf;
	if(pKL->m_keyID > 0x20)
	{
		char *pos = &SimDataBuf[SimBufPos%10][0];
		*((uint16*)pos) = pKL->m_keyID;
		pos += sizeof(uint16);
		memcpy(pos, buf + sizeof(KL), pKL->m_length);
		SimBufPos = (SimBufPos+1)%10;
// 		DPRINTF("SimPos=%d\n", SimBufPos);
	}
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	pKL->m_length = 0;
	size_t setsize = pKL->m_length + sizeof(KL);
	return m_socket.Send(buf, setsize, setsize);
// 	return true;
}