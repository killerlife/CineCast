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
	return true;
}

void SimulatorServer::doit()
{
	char str[512];
	bool bSuccess = false;
	int i = 0;
	int socket_fd;
	struct sockaddr_in myaddr, sender;
	int so_broadcast = 1;
	while(m_status != STOP && bSuccess == false)
	{
		try
		{
			myaddr.sin_family = AF_INET;
			myaddr.sin_port = htons(SIMULATOR_PORT);
			myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
				throw -1;
			if(setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast))!=0)
				throw -2;
			if(bind(socket_fd, (struct sockaddr*)&myaddr, sizeof(sockaddr_in)) != 0)
				throw -3;
			bSuccess = true;
			break;
		}
		catch (int &a)
		{
			close(socket_fd);
			switch(a)
			{
			case -1:
				sprintf(str, "[SimulatorServer] UDP Socket error.");
				break;
			case -2:
				sprintf(str, "[SimulatorServer] UDP SetSocketOpt error.");
				break;
			default:
				sprintf(str, "[SimulatorServer] UDP Bind error.");
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
	char buf[4096];
	int pos;
	int nRead, n;
	socklen_t size;
	int nCount = 0;
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				memset(buf, 0, 4096);
				pos = 0;
				nRead = sizeof(KL);
				while (nRead > 0)
				{
					size = sizeof(sender);
					n = recvfrom(socket_fd, buf + pos, nRead, 0, (struct sockaddr *)&sender, &size);
					if(n > 0)
					{
						pos += n;
						nRead -= n;
					}
					else
						break;
				}

// 				size = sizeof(sender);
// 				if(recvfrom(socket_fd, buf, sizeof(KL), 0, (struct sockaddr *)&sender, &size) == sizeof(KL))
				{
					KL* pKL = (KL*)buf;
					if (pKL->m_pkgHead == 0x7585)
					{
						nCount++;
						pos = 0;
						nRead = pKL->m_length;
						while (nRead > 0)
						{
							size = sizeof(sender);
							n = recvfrom(socket_fd, buf + sizeof(KL) + pos, nRead, 0, (struct sockaddr *)&sender, &size);
							if(n > 0)
							{
								pos += n;
								nRead -= n;
							}
							else
								break;
						}
						switch(pKL->m_keyID)
						{
						case 0x1ff:
							NotifyProc(buf);
							break;
						case 0x2ff:
							StartProc(buf);
							break;
						case 0x3ff:
							FinishProc(buf);
							break;
						case 0x4ff:
							CancelProc(buf);
							DPRINTF("Counts = %d\n", nCount);
							nCount = 0;
							break;
						case 0xfe:
							PatProc(buf);
							break;
						case 0x20:
							PmtProc(buf);
							break;
						default:
							DataProc(buf);
						}
					}
					else
						DPRINTF("error\n");
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

char SimDataBuf[10][4096];
int SimBufPos = 0;

#include <dvb/SatelliteRecv.h>
bool SimulatorServer::ErrorProtocol(char* buf)
{
	char str[512];
	KL *pKL = (KL*)buf;
// 	if (gLog)
// 	{
// 		sprintf(str, "[SimulatorThread] Received error protocol: %04X.", pKL->m_pkgHead);
// 		gLog->Write(LOG_ERROR, str);
// 	}
	return true;
}

bool SimulatorServer::NotifyProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x1ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
//  	print_hex(buf + sizeof(KL), pKL->m_length);
// 	print_hex(pos, pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
	uint32* pDebugCmd = GetDebugCommand();
	*pDebugCmd = 0x10000000;
// 	DPRINTF("Notify SimPos=%d\n", SimBufPos);
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}
	
	return true;
}

bool SimulatorServer::StartProc(char* buf)
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

	return true;
}

bool SimulatorServer::FinishProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x3ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	uint32* pDebugCmd = GetDebugCommand();
// 	*pDebugCmd = 0;
// 	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	return true;
}

bool SimulatorServer::CancelProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0x4ff;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
// 	uint32* pDebugCmd = GetDebugCommand();
// 	*pDebugCmd = 0;
//if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	return true;
}

bool SimulatorServer::PatProc(char* buf)
{
	KL *pKL = (KL*)buf;
	char *pos = &SimDataBuf[SimBufPos%10][0];
	*((uint16*)pos) = 0xfe;
	pos += sizeof(uint16);
	memcpy(pos, buf + sizeof(KL), pKL->m_length);
	SimBufPos = (SimBufPos+1)%10;
// 	DPRINTF("SimPos=%d\n", SimBufPos);
	if (gLog)
// 	{
// 		char str[512];
// 		sprintf(str, "[SimulatorThread] Received protocol: %04X.", pKL->m_keyID);
// 		gLog->Write(LOG_ERROR, str);
// 	}

	return true;
}

bool SimulatorServer::PmtProc(char* buf)
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

	return true;
}

bool SimulatorServer::DataProc(char* buf)
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

	return true;
}