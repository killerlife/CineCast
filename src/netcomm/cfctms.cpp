#include "cfctms.h"
#include "UiProtocal.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <log/Log.h>
#include <log/LogStruct.h>

#define TMS_PORT (10021)

using namespace brunt;
extern ILog* gLog;
extern TUNER_INFO gInfo;
extern RECEIVE_INFO gRecv;
extern TUNER_CONF gTuner;

CfcTmsThread gCfcTms;

CfcTmsThread* CreateCfcTms()
{
	return &gCfcTms;
}

void ReleaseCfcTms(CfcTmsThread* pCfc)
{

}

CfcTmsThread::CfcTmsThread():bConnected(false)
{

}

CfcTmsThread::~CfcTmsThread()
{
	cleanThread();
}

bool CfcTmsThread::Init(char* ip, short port)
{
	if(ip == NULL)
		tmsIp = "127.0.0.1";
	else
		tmsIp = ip;
	tmsPort = port;
}

bool CfcTmsThread::Start()
{
	char str[512];

	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[CfcTmsThread] The thread had not been ready, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false;
	}

	if (isStopped())
	{
		cleanThread();
	}

	if(!start())
	{
		sprintf(str, "[CfcTmsThread] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false; 
	}

	m_status = TMS_RUN;

	return true;
}

bool CfcTmsThread::Stop()
{
	m_status = TMS_STOP;

	usleep(100000);

	m_socket.Destroy();
}

void CfcTmsThread::cleanThread()
{
	m_status = TMS_STOP;
	m_socket.Destroy();
	CActiveThread::stop();
	m_status = STOP;
}

void CfcTmsThread::doit()
{
	char str[512];

	while(1)
	{
		switch(m_status)
		{
		case TMS_RUN:
			{
				bConnected = false;
				try
				{
					if(m_socket.Create(AF_INET, SOCK_STREAM, 0))
					{
						struct sockaddr_in addr_in;
						memset(&addr_in, 0, sizeof(sockaddr_in));
						addr_in.sin_family = AF_INET;
						addr_in.sin_port = htons(tmsPort);
						struct hostent *he;
						if((he = gethostbyname(tmsIp.c_str())) == NULL)
						{
							if(gLog)
								gLog->Write(LOG_NETCOMMU, "[CfcTmsThread] gethostbyname error.");
							throw -2;
						}
						struct in_addr **addr_list = (struct in_addr**)he->h_addr_list;
						char ip[100];
						for(int i = 0; addr_list[i] != NULL; i++)
						{
							strcpy(ip, inet_ntoa(*addr_list[i]));
							break;
						}
						sprintf(str, "[CfcTmsThread] %s resolved to %s.", tmsIp.c_str(), ip);
						if(gLog)
							gLog->Write(LOG_ERROR, str);
						addr_in.sin_addr.s_addr = inet_addr(ip);
						int error = 0;
						do
						{
							t_timeout tm = 3000;
							error = m_socket.Connect(&addr_in, &tm);
							if (error && error != CZSocket::WAIT_R_TIMEOUT)
							{
								throw -1;
							}
						}while(error);
						m_status = TMS_LOGIN;
					}
				}
				catch(int& err)
				{
					m_socket.Destroy();
					sleep(5);
					bConnected = false;
					sprintf(str, "[CfcTmsThread] Connect to login server %s:%d error.", tmsIp.c_str(), (uint16)tmsPort);
					if(gLog)
						gLog->Write(LOG_ERROR, str);
					DPRINTF("%s\n", str);
				}
			}
			break;
		case TMS_LOGIN:
			{
				bConnected = true;
 				RegSatelliteDownloadTask();
				m_status = TMS_READ;
			}
			break;
		case TMS_READ:
			{
				try
				{
					RecvFilter();
				}
				catch (int &)
				{
					DPRINTF("throw\n");
					bConnected = false;
					m_socket.Destroy();
					if(m_status == TMS_READ)
						m_status = TMS_RUN;
				}
			}
			break;
		case TMS_STOP:
			m_socket.Destroy();
			return;
		default:
			usleep(100*1000);
			break;
		}
	}
}

bool CfcTmsThread::RecvFilter()
{
	static char buf[4096];
	t_timeout tm = 300;
	size_t get_size = 0;
	int err = m_socket.Receive2(buf, 4, get_size, &tm);
	if(err == CZSocket::WAIT_R_TIMEOUT && get_size == 0)
	{
		return false;
	}
	if(err < 0 && get_size == 0)
	{
		throw -1;
	}
	int length = ((buf[0]&0xff) << 24) | ((buf[1]&0xff) << 16) | ((buf[2]&0xff) << 8) | (buf[3]&0xff);
	DPRINTF("Recv len: %d  %02x %02x %02x %02x\n", length, buf[0], buf[1], buf[2], buf[3]);
	if(length <= 0)
		return true;
	get_size = 0;
	err = m_socket.Receive2(buf + 4, length, get_size, &tm);
	if(err == CZSocket::WAIT_R_TIMEOUT && get_size == 0)
	{
		return false;
	}
	if(err < 0 && get_size == 0)
	{
		throw -1;
	}
	buf[get_size+4] = '\0';
// 	DPRINTF("Recv TMS: len %d %s\n", length, buf + 4);
// 	if(gLog)
// 	    gLog->Write(LOG_NETCOMMU, buf + 4);

	if(strstr(buf+4, "\"request_message_id\":\"10001\"") != NULL)
	{
		DPRINTF("Got it\n");
		ProcessUpdateSatelliteDownloadStatus(buf+4);
	}
	if(strstr(buf+4, "\downloadFileName\":") != NULL)
	{
		DPRINTF("Got it\n");
		ProcessUpdateSatelliteDownloadStatus(buf+4);
	}
	return true;
}

bool CfcTmsThread::RegSatelliteDownloadTask()
{
	if(gRecv.nFileID <= 0)
		return false;
	//Generate ID
	strId = "10000";
	//
	char buf[4096];
	sprintf(buf+4, "{\"message_id\":\"%s\",\"message_type\":\"request\","
		"\"message_source\":\"satellite\","
		"\"method_name\":\"tms.regSatelliteDownloadTask\","
		"\"method_params\":{\"taskUUID\":\"%d\","
		"\"contentUUID\":\"%s\",\"contentTitle\":\"%s\","
		"\"contentKind\":\"%s\",\"contentDuration\":\"%d\","
		"\"contentSize\":\"%lld\",\"contentEncryption\":\"%d\"}}",
		strId.c_str(),
		gRecv.nFileID, gRecv.strUuid.c_str(), gRecv.strFilmName.c_str(),
		"UNSUPPORTED", -1, gRecv.nFileLength,
		1);
	int length = strlen(buf+4);
	buf[0] = (length >> 24) & 0xff;
	buf[1] = (length >> 16) & 0xff;
	buf[2] = (length >> 8) & 0xff;
	buf[3] = (length) & 0xff;
// 	DPRINTF("JSON CMD:\nlen:%d\n%s\n", length, buf+4);
// 	if(gLog)
// 	    gLog->Write(LOG_NETCOMMU, buf+4);
	size_t send_size = 0;
	t_timeout tm = 10000;
	if(m_socket.Send(buf, length+4, send_size, &tm) < 0)
	{
		bConnected = false;
		m_socket.Destroy();
		if(m_status == TMS_READ)
			m_status = TMS_RUN;
		return false;
	}
	#if 0
	FILE *fp = fopen("/root/data.bin", "wb");
	if(fp!=NULL)
	{
	    DPRINTF("write tp file\n");
	    fwrite(&buf, length+4, 1, fp);
	    fclose(fp);
	}
	#endif
	return true;
}

bool CfcTmsThread::UpdateSatelliteDownloadStatus(int speed, bool cancel)
{
	if (gRecv.nFileID <= 0)
		return false;
	//Generate ID
	strId = "10001";
	//
	char buf[4096];
	std::string desc;
	int status;
	if(cancel)
	{
		status = 3;
		desc = "Canceled";
	}
	else
	{
		switch (gRecv.nReceiveStatus&0xffff)
		{
		case 20:
			status = 1;
			desc = "Upload DCP to RAID";
			break;
		case 11:
			status = 4;
			desc = "Finish";
			break;
		case 10:
			status = 1;
			desc = "Verify Success";
			break;
		case 9:
			status = 1;
			desc = "Verifying";
			break;
		case 8:
			status = 1;
			desc = "Received MD5";
			break;
		case 7:
			status = 1;
			desc = "Asking MD5";
			break;
		case 6:
			status = 1;
			desc = "Uploaded Lost Information";
			break;
		case 5:
			status = 1;
			desc = "Uploading Lost Information";
			break;
		case 3:
			status = 1;
			desc = "analyzing Lost Information";
			break;
		case 2:
			status = 1;
			desc = "Round Finish";
			break;
		case 1:
			status = 1;
			desc = "Transferring";
			break;
		case 0:
			status = 2;
			desc = "Idle";
			break;
		case 4:
		default:
			status = -1;
			desc = "Reserved";
			break;
		}
	}
	if(gRecv.nTotalSegment > 0)
	{
		sprintf(buf+4, "{\"message_id\":\"%s\",\"message_type\":\"request\","
			"\"message_source\":\"satellite\","
			"\"method_name\":\"tms.updateSatelliteDownloadStatus\","
			"\"method_params\":{\"taskUUID\":\"%d\","
			"\"downloadFileName\":\"%s\",\"downloadfileSize\":\"%lld\","
			"\"downloadProgress\":\"%d\",\"downloadSpeed\":\"%d\","
			"\"downloadStatus\":\"%d\",\"downloadStatus_desc\":\"%s\"}}",
			strId.c_str(),
			gRecv.nFileID, gRecv.strFilmName.c_str(), gRecv.nReceiveLength,
			gRecv.nReceiveSegment*100/gRecv.nTotalSegment,
			speed, status,
			desc.c_str());
	}
	else
	{
		sprintf(buf+4, "{\"message_id\":\"%s\",\"message_type\":\"request\","
			"\"message_source\":\"satellite\","
			"\"method_name\":\"tms.updateSatelliteDownloadStatus\","
			"\"method_params\":{\"taskUUID\":\"%d\","
			"\"downloadFileName\":\"%s\",\"downloadfileSize\":\"%lld\","
			"\"downloadProgress\":\"0\",\"downloadSpeed\":\"%d\","
			"\"downloadStatus\":\"%d\",\"downloadStatus_desc\":\"%s\"}}",
			strId.c_str(),
			gRecv.nFileID, gRecv.strFilmName.c_str(), gRecv.nReceiveLength,
			speed, status,
			desc.c_str());
	}
	int length = strlen(buf+4);
	buf[0] = (length >>24)&0xff;
	buf[1] = (length >> 16) & 0xff;
	buf[2] = (length >> 8) & 0xff;
	buf[3] = (length) & 0xff;
// 	DPRINTF("JSON CMD:\nlen %d\n%s\n", length, buf+4);
// 	if(gLog)
// 	    gLog->Write(LOG_NETCOMMU, buf+4);
	size_t send_size = 0;
	t_timeout tm = 10000;
	if(m_socket.Send(buf, length + 4, send_size, &tm) < 0)
	{
		bConnected = false;
		m_socket.Destroy();
		if(m_status == TMS_READ)
			m_status = TMS_RUN;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Requirement for CFCTMS
	// Keep Send finish satellite download to TMS after finish receive session
	if(gRecv.nReceiveSegment != gRecv.nTotalSegment)
		strFinish.clear();
	else if(!strFinish.empty())
		finishSatelliteDownload(strFinish);
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool CfcTmsThread::ImportContentUsbLocalToTms(std::string path)
{
	if(gRecv.nFileID <= 0)
		return false;
	//Generate ID
	strId = "10002";
	//
	char buf[4096];
	sprintf(buf+4, "{\"message_id\":\"%s\",\"message_type\":\"request\","
		"\"message_source\":\"satellite\","
		"\"method_name\":\"tms.importContentUsbLocalToTms\","
		"\"method_params\":{\"verifyType\":\"0\","
		"\"importOrder\":\"1\",\"contents\":[\"%s\"]}}",
		strId.c_str(), path.c_str());
	int length = strlen(buf);
	buf[0] = (length >> 24) & 0xff;
	buf[1] = (length >> 16) & 0xff;
	buf[2] = (length >> 8) & 0xff;
	buf[3] = (length) & 0xff;
// 	DPRINTF("JSON CMD:\nlen %d\n%s\n", length, buf+4);
// 	if(gLog)
// 	    gLog->Write(LOG_NETCOMMU, buf+4);
	size_t send_size = 0;
	t_timeout tm = 10000;
	if(m_socket.Send(buf, length + 4, send_size, &tm) < 0)
	{
		bConnected = false;
		m_socket.Destroy();
		if(m_status == TMS_READ)
			m_status = TMS_RUN;
		return false;
	}
	return true;
}

bool CfcTmsThread::finishSatelliteDownload(std::string buf)
{
	if(gRecv.nFileID <= 0)
		return false;
	strId = "10003";
	char buffer[4096];
	sprintf(buffer, "{\"message_id\":\"%s\",\"message_type\":\"request\","
		"\"message_source\":\"satellite\","
		"\"method_name\":\"tms.finishSatelliteDownload\","
		"\"method_params\":{\"taskUUID\":\"%d\","
		,
		strId.c_str(), gRecv.nFileID);
	std::string ss = buffer;
	if(strFinish != buf)
		strFinish = buf;
	ss += strFinish;
	ss += "}}";
	int length = ss.size();
	buffer[0] = (length >> 24) & 0xff;
	buffer[1] = (length >> 16) & 0xff;
	buffer[2] = (length >> 8) & 0xff;
	buffer[3] = (length) & 0xff;
// 	DPRINTF("JSON CMD:\nlen %d\n%s\n", length, ss.c_str());
// 	if(gLog)
// 	    gLog->Write(LOG_NETCOMMU, ss.c_str());
	size_t send_size = 0;
	t_timeout tm = 10000;
	if(m_socket.Send(buffer, 4, send_size, &tm) < 0)
	{
	    bConnected = false;
	    m_socket.Destroy();
	    if(m_status == TMS_READ)
		m_status = TMS_RUN;
	    return false;
	}
	send_size = 0;
	
	if(m_socket.Send(ss.c_str(), length, send_size, &tm) < 0)
	{
		bConnected = false;
		m_socket.Destroy();
		if(m_status == TMS_READ)
			m_status = TMS_RUN;
		return false;
	}
	return true;
	
}

bool CfcTmsThread::ProcessUpdateSatelliteDownloadStatus(char* buf)
{
	if(strstr(buf, "\"return_status\":false") != NULL)
		return RegSatelliteDownloadTask();
	return true;
}
