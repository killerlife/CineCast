#include <thread/activeThread/activeThread.h>
#include <string>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <thread/activeThread/activeThreadManager_i.h>
#include "../netcomm/zSocket.h"

#include "tmssrv.h"
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <dvb/mytype.h>

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QByteArray>
#include <QDomProcessingInstruction>
#include <log/Log.h>

using namespace brunt;
extern ILog* gLog;

class TmsThread: public CActiveThread
{
public:
	TmsThread();
	virtual ~TmsThread();

	bool Start();
	bool Stop();

	bool isStop() {return bStop;};

	bool Read(char* buf, int rSize, int& nRead);
	bool Write(char* buf, int wSize, int& nWrite);

	bool ProcessFilter();

public:
	CZSocket m_socket;

private:
	virtual void doit();
	virtual void init();
	virtual void release();
	void cleanThread();

	bool connect_req(char* buf);
	bool content_req(char* buf);
	bool ftp_req(char* buf);
	bool end_req(char *buf);

private:
	int m_status;
	bool bStop;
	ILog* pLog;
};

class TmsServer:public ITmsServer, public CActiveThread
{
public:
	virtual int Start(int port);

	TmsServer();
	virtual ~TmsServer();

protected:
	virtual void init();
	virtual void doit();
	virtual void release();
	void cleanThread();

private:
	CZSocket m_SrvSocket;
	list<TmsThread*> m_listClient;
	TmsThread *m_pThread;
	int m_port;
	int m_status;
};

ITmsServer* getITmsServer()
{
	static TmsServer g_tmsServer;
	return &g_tmsServer;
}

TmsServer::TmsServer():m_status(0), m_pThread(0)
{
}

TmsServer::~TmsServer()
{
	cleanThread();
	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = 0;
	}
}

void TmsServer::cleanThread()
{
	m_status = STOP;
	CActiveThread::stop();

	m_SrvSocket.Destroy();
	std::list<TmsThread*>::iterator itor;
	for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
	{
		TmsThread* client = *itor;
		client->Stop();
		while(!client->isStop()) usleep(100000);
		client->stop();
		delete client;
	}
	m_listClient.clear();
}

void TmsServer::release()
{
	return CActiveThread::release();
}

void TmsServer::init()
{
	return CActiveThread::init();
}

int TmsServer::Start(int port)
{
	m_port = port;
	char str[512];
	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[TmsServer] The thread had not been ready, status = %d.", status());

		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return -1;
	}

	if (isStopped())
	{
		cleanThread();
	}

	if(!start())
	{
		sprintf(str, "[TmsServer] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return -1; 
	}
	m_status = RUN;
	return 0;
}

#ifdef THREAD
#include "../dvb/demux.h"
#include "../content/IContent.h"
vector<InfoData> gCList;
static int gMutex = 0;
#endif

void TmsServer::doit()
{
	char str[512];
#ifdef THREAD
	bool bSuccess = false;
	while(!bSuccess)
	{
#endif
	try
	{
		if (m_SrvSocket.Create(AF_INET, SOCK_STREAM, 0))
		{
			struct sockaddr_in addr_in;
			memset(&addr_in, 0, sizeof(sockaddr_in));
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(m_port);
			addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
			int error = 0;
			do 
			{
				t_timeout tm = 3000;
				if(m_SrvSocket.Bind(&addr_in))
				{
					if (!m_SrvSocket.Listen(10))
						throw -1;
#ifdef THREAD
						bSuccess = true;
#endif
				}
				else
					throw -2;

			} while (error);
		}
	}
	catch (int& a)
	{
			m_SrvSocket.Destroy();
		switch(a)
		{
		case -1:
			sprintf(str, "[TmsServer] Server Socket Listen error.");
			break;
		case -2:
			sprintf(str, "[TmsServer] Server Socket Bind error.");
			break;
		default:
			sprintf(str, "[TmsServer] Server Socket unknow error.");
		}
		DPRINTF("%s\n", str);
		if (gLog)
	{
			gLog->Write(LOG_ERROR, str);
		}
			sleep(5);
// 			return;
		}
#ifdef THREAD
	}
	int err = -1;
	IContentManager* pCm = getIContentManager();
	vector<int> srcList;
	srcList.push_back(PST_FTP);
#endif
	while(1)
	{
#ifdef THREAD
		vector<InfoData> m_cList;
#endif
		switch(m_status)
		{
		case RUN:
			{
#ifdef THREAD
				if(err != 0)
					err = pCm->update(srcList);
#endif
				if (m_pThread == NULL)
				{
					m_pThread = new TmsThread;
				}
				t_timeout tm = 2000;
				if(m_SrvSocket.Accept(m_pThread->m_socket, NULL, NULL, &tm) == 0)
				{
					m_listClient.push_back(m_pThread);
					m_pThread->Start();
					m_pThread = NULL;
				}
#ifdef THREAD
				if(err == 0)
				{
					if(pCm->isReady(PST_FTP))
					{
						pCm->getProgramFileList(PST_FTP, 0, m_cList);
						while(gMutex);
						gMutex = 1;
						gCList.clear();
						gCList = m_cList;
						gMutex = 0;
						err = -1;
					}
				}
				DPRINTF("Accept timeout\n");
#endif
			}
			break;
		case STOP:
			DPRINTF("[TmsServer] Stop in loop\n");
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

TmsThread::TmsThread():m_status(0),bStop(true), pLog(0)
{
	pLog = CreateLog();
}

TmsThread::~TmsThread()
{
	cleanThread();
	if (pLog)
	{
		ReleaseLog(pLog);
	}
}

void TmsThread::cleanThread()
{
	m_status = STOP;
	CActiveThread::stop();

	m_socket.Destroy();
}

void TmsThread::release()
{
	return CActiveThread::release();
}

void TmsThread::init()
{
	return CActiveThread::init();
}

bool TmsThread::Start()
{
	char str[512];
	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[TmsThread] The thread had not been ready, status = %d.", status());
		DPRINTF("%s\n", str);
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return false;
	}

	if (isStopped())
	{
		cleanThread();
	}

	if(!start())
	{
		sprintf(str, "[TmsThread] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return false; 
	}
	m_status = RUN;
	return true;
}

bool TmsThread::Stop()
{
	m_status = STOP;
	usleep(100000);
	return true;
}

void TmsThread::doit()
{
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				if (!ProcessFilter())
				{
					DPRINTF("[TmsThread] Stop - Receive error\n");
					m_status = STOP;
				}
			}
			break;
		case STOP:
			DPRINTF("[TmsThread] Stop in loop %d\n", this);
			bStop = true;
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

#pragma pack(1)
typedef struct _TMSCMD{
public:
	uint8 preamble;
	uint16 cmd;
	uint32 length;

	_TMSCMD()
	{
		preamble = 0x55;
	}
} TMSCMD;

enum {
	CONNECT_REQ = 0x11,
	CONNECT_ACK,

	CONTENT_REQ = 0x21,
	CONTENT_ACK,
	CONTENT_FAIL,
	FTP_REQ,
	FTP_ACK,
	FTP_FAIL,
	END_REQ,
	END_ACK,
};

bool TmsThread::Read(char* buf, int rSize, int& nRead)
{
	try
	{
		int i = 0;
		int n = sizeof(TMSCMD);
		size_t getsize;
		int error = 1;
		TMSCMD* pKL;
		t_timeout tm = 1;

		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
			{
				DPRINTF("[TmsThread] Read pkg head error i = %d, n = %d\n", i, n);
				throw -1;
			}
			else
			{
				DPRINTF("[TmsThread] Read pkg head n = %d, size = %d\n", n, getsize);
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else/* if(getsize == n)*/
				{
					pKL = (TMSCMD*)buf;
					if (pKL->preamble == 0x55)
					{
						error = 0;
					}
					else
					{
						i = 0;
						n = sizeof(TMSCMD);
					}
				}
			}
		} while (error && (m_status != STOP));
		n = pKL->length + 4;

		i = sizeof(TMSCMD);
		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
			{
				DPRINTF("[TmsThread] Read pkg data error i = %d, n = %d\n", i, n);
				throw -1;
			}
			else
			{
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else
				{
					nRead = pKL->length + sizeof(TMSCMD) + 4;
					return true;
				}
			}
		} while (error && (m_status != STOP));
	}
	catch (int&)
	{
		DPRINTF("[TmsThread] Receive error\n");
		return false;
	}
}

bool TmsThread::Write(char* buf, int wSize, int& nWrite)
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
		DPRINTF("[TmsThread] Send error\n");
		return false;
	}
}

bool TmsThread::ProcessFilter()
{
	char buf[1024*1024];
	memset(buf, 0, 2048);
	int getsize;
	if (Read(buf, 2048, getsize))
	{
		DPRINTF("Read success %d\n", getsize);
		TMSCMD* pKL = (TMSCMD*)buf;
		if (pKL->preamble == 0x55)
		{
			DPRINTF("preamble OK\n");
			switch(pKL->cmd)
			{
			case CONNECT_REQ:
				return connect_req(buf);
			case CONTENT_REQ:
				return content_req(buf);
			case FTP_REQ:
				return ftp_req(buf);
			case END_REQ:
				DPRINTF("end_req\n");
				return end_req(buf);
			}
		}
	}
	DPRINTF("Read failed\n");
	return false;
}

#include "../dvb/demux.h"
#include "../content/IContent.h"

bool TmsThread::connect_req(char* buf)
{
	TMSCMD *pKL = (TMSCMD*)buf;
	pKL->cmd = CONNECT_ACK;
	pKL->length = 0;
	void *pos = buf + sizeof(TMSCMD);
	uint32 crc = calc_crc32((uint8*)buf, sizeof(TMSCMD));
	memcpy(pos, &crc, sizeof(crc));

	int setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);

	if(pLog)
		pLog->Write(LOG_TMS, "TMS Login OK.");

	return Write(buf, setsize, setsize);
}

bool TmsThread::content_req(char* buf)
{
#ifndef THREAD
	IContentManager* pCm = getIContentManager();
	vector<int> srcList;
	vector<InfoData> m_cList;
	srcList.push_back(PST_FTP);
#endif

	TMSCMD *pKL = (TMSCMD*)buf;
	uint32 crc;
	int setsize;

	if(pLog)
		pLog->Write(LOG_TMS, "TMS Request DCP List.");

	try 
	{
#ifndef THREAD
		if(pCm->update(srcList) == 0)
		{
			while(1)
			{
				if(pCm->isReady(PST_FTP) == true)
				{
					pCm->getProgramFileList(PST_FTP, 0, m_cList);
					InfoData info;
					std::vector<InfoData>::iterator itor;
					if(m_cList.size() > 0)
#else
		InfoData info;
		vector<InfoData>::iterator itor;
		while(gMutex);
		gMutex = 1;
		if(gCList.size() > 0)
#endif
					{
						QDomDocument doc;
						QDomProcessingInstruction instruction;  
						instruction = 
							doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");  
						doc.appendChild(instruction);


						QDomElement resp = doc.createElement("response");
						doc.appendChild(resp);

						resp.setAttribute("status", "OK");
						resp.setAttribute("version", "2");
						QDomElement list = doc.createElement("dcp_list");
						resp.appendChild(list);

						int dcp_cnt = 0;
#ifndef THREAD
						for (itor = m_cList.begin(); itor != m_cList.end(); ++itor)
#else
						for (itor = gCList.begin(); itor != gCList.end(); ++itor)
#endif
						{
							InfoData info = *itor;
							if (info.pData[2] == "100")
							{
								QDomElement dcp = doc.createElement("dcp");

								QDomText txt;
								QDomElement item;
								if (info.pData[0] == "")
									continue;
								item = doc.createElement("uuid");
								dcp.appendChild(item);
								txt = doc.createTextNode(info.pData[0].c_str());
								item.appendChild(txt);

								if (info.pData[9] == "")
									continue;
								item = doc.createElement("issueDate");
								dcp.appendChild(item);
								txt = doc.createTextNode(info.pData[9].c_str());
								item.appendChild(txt);

								if (info.pData[8] == "")
									continue;
								item = doc.createElement("issuer");
								dcp.appendChild(item);
								txt = doc.createTextNode(info.pData[8].c_str());
								item.appendChild(txt);

								if (info.pData[15] == "")
									continue;
								item = doc.createElement("creator");
								dcp.appendChild(item);
								txt = doc.createTextNode(info.pData[15].c_str());
								item.appendChild(txt);

								list.appendChild(dcp);
								dcp_cnt++;
							}
						}
#ifdef THREAD
						gMutex = 0;
#endif
						if (dcp_cnt == 0)
						{
							if (pLog)
								pLog->Write(LOG_TMS, "Device Response No DCP.");
							throw -1;
						}
						QString s = doc.toString();
						pKL->cmd = CONTENT_ACK;
						pKL->length = s.toStdString().size();
						void *pos = buf + sizeof(TMSCMD);
						memcpy(pos, s.toStdString().c_str(), pKL->length);
						crc = calc_crc32((uint8*)buf, sizeof(TMSCMD) + pKL->length);
						memcpy(pos + pKL->length, &crc, sizeof(crc));
						setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);

						s.sprintf("Device Response %d DCP(s) to TMS", dcp_cnt);
						if (pLog)
							pLog->Write(LOG_TMS, s.toStdString().c_str());
						return Write(buf, setsize, setsize);
					}
					else
					{
						if (pLog)
							pLog->Write(LOG_TMS, "Device Response No DCP.");
						throw -1;
					}
#ifndef THREAD
				}
			}
		}
		else
		{
			if(pLog)
				pLog->Write(LOG_TMS, "Device Response DCP Not Ready.");
			throw -1;
		}
#endif
	}
	catch(int&)
	{
#ifdef THREAD
		gMutex = 0;
#endif
		pKL->cmd = CONTENT_FAIL;
		pKL->length = 0;
		void *pos = buf + sizeof(TMSCMD);
		crc = calc_crc32((uint8*)buf, sizeof(TMSCMD));
		memcpy(pos, &crc, sizeof(crc));
		setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);
		return Write(buf, setsize, setsize);
	}
}

bool TmsThread::ftp_req(char* buf)
{
	TMSCMD *pKL = (TMSCMD*)buf;

	QString errStr;
	int errLine;
	int errColumn;

	QString uuidText;
	QString msg;

	QDomDocument doc;
	try
	{
		if (doc.setContent(QByteArray(buf+sizeof(TMSCMD), pKL->length),
			true, &errStr, &errLine, &errColumn))
		{
			QDomElement response = doc.firstChildElement("response");
			QString status = response.attribute("status", "ERROR");
			QDomElement uuid = response.firstChildElement("uuid");
			if(!uuid.isNull())
				uuidText = uuid.text();
#ifndef THREAD
			IContentManager* pCm = getIContentManager();
			vector<int> srcList;
			vector<InfoData> m_cList;
			srcList.push_back(PST_FTP);
#endif
			std::string path;
#ifndef THREAD
			if(pCm->update(srcList) == 0)
			{
				while(1)
				{
					if(pCm->isReady(PST_FTP) == true)
					{
						pCm->getProgramFileList(PST_FTP, 0, m_cList);
#endif
						InfoData info;
						bool bFind = false;
#ifndef THREAD
						for(int i = 0; i < m_cList.size(); i++)
#else
						while(gMutex);
						gMutex = 1;
						for(int i = 0; i < gCList.size(); i++)
#endif
						{
#ifndef THREAD
							if (m_cList[i].pData[0] == uuidText.toStdString())
#else
							if (gCList[i].pData[0] == uuidText.toStdString())
#endif
							{
#ifndef THREAD
								path = m_cList[i].pData[10];
#else
								path = gCList[i].pData[10];
#endif
								bFind = true;
								break;
							}
						}
#ifdef THREAD
						gMutex = 0;
#endif
						if (!bFind)
						{
							QString s;
							s.sprintf("Device Response %s Not Found.", uuidText.toStdString().c_str());
							if(pLog)
								pLog->Write(LOG_TMS, s.toStdString().c_str());
							throw -1;
						}
						QDomDocument doc;
						QDomProcessingInstruction instruction;  
						instruction = 
							doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");  
						doc.appendChild(instruction);

						QDomElement resp = doc.createElement("response");
						doc.appendChild(resp);
						resp.setAttribute("status", "OK");
						resp.setAttribute("version", "2");

						QDomText txt;
						QDomElement item;

						item = doc.createElement("asset_type");
						resp.appendChild(item);
						txt = doc.createTextNode("PKL");
						item.appendChild(txt);

						item = doc.createElement("source");
						resp.appendChild(item);

						std::string ip = m_socket.GetSockName().c_str();
						QString source;
						source.sprintf("ftp://%s/", ip.c_str());
						txt = doc.createTextNode(source);
						item.appendChild(txt);

						item = doc.createElement("username");
						resp.appendChild(item);
						txt = doc.createTextNode("leonis");
						item.appendChild(txt);

						item = doc.createElement("password");
						resp.appendChild(item);
						txt = doc.createTextNode("leonis62670467");
						item.appendChild(txt);

						item = doc.createElement("path");
						resp.appendChild(item);
						txt = doc.createTextNode(path.c_str() + strlen("/storage/ftp/"));
						item.appendChild(txt);

						QString s = doc.toString();
						pKL->cmd = FTP_ACK;
						pKL->length = s.toStdString().size();
						void *pos = buf + sizeof(TMSCMD);
						memcpy(pos, s.toStdString().c_str(), pKL->length);
						int crc = calc_crc32((uint8*)buf, sizeof(TMSCMD) + pKL->length);
						memcpy(pos + pKL->length, &crc, sizeof(crc));
						int setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);

						s.sprintf("Device Response %s's FTP URL to TMS", uuidText.toStdString().c_str());
						if (pLog)
						{
							pLog->Write(LOG_TMS, s.toStdString().c_str());
							s.sprintf("FTP Source: %s", source.toStdString().c_str());
							pLog->Write(LOG_TMS, s.toStdString().c_str());
							s.sprintf("FTP Path: %s", path.c_str() + strlen("/storage/ftp/"));
							pLog->Write(LOG_TMS, s.toStdString().c_str());
						}

						return Write(buf, setsize, setsize);
#ifndef THREAD
					}
				}
			}
#endif
		}
		else
		{
			if(pLog)
			{
				pLog->Write(LOG_ERROR, "[TmsThread] Device Parse FTP Request error.");
				pLog->Write(LOG_TMS, "Device Response FTP Not Ready.");
			}
			throw -1;
		}
	}
	catch(int&)
	{
		pKL->cmd = FTP_FAIL;
		pKL->length = 0;
		void *pos = buf + sizeof(TMSCMD);
		int crc = calc_crc32((uint8*)buf, sizeof(TMSCMD));
		memcpy(pos, &crc, sizeof(crc));
		int setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);
		return Write(buf, setsize, setsize);
	}
}

bool TmsThread::end_req(char *buf)
{
	TMSCMD *pKL = (TMSCMD*)buf;

	QString errStr;
	int errLine;
	int errColumn;

	QString uuidText;
	QString msg;

	QDomDocument doc;
	if (doc.setContent(QByteArray(buf+sizeof(TMSCMD), pKL->length),
		true, &errStr, &errLine, &errColumn))
	{
		QDomElement response = doc.firstChildElement("response");
		QString status = response.attribute("status", "ERROR");
		QDomElement uuid = response.firstChildElement("uuid");
		if(!uuid.isNull())
			uuidText = uuid.text();
		if(status == "OK")
			msg = "Download OK";
		else
		{
			QDomElement err = response.firstChildElement("error");
			if (!err.isNull())
			{
				msg = err.text();
			}
		}
	}
	else
	{
		if(pLog)
		{
			pLog->Write(LOG_ERROR, "[TmsThread] Device Parse END Request error.");
		}
	}
	QString ss = uuidText + " -> " + msg;
	DPRINTF("[TmsThread] %s\n", ss.toStdString().c_str());
	if(pLog)
		pLog->Write(LOG_TMS, ss.toStdString().c_str());
	pKL->cmd = END_ACK;
	pKL->length = 0;
	void *pos = buf + sizeof(TMSCMD);
	uint32 crc = calc_crc32((uint8*)buf, sizeof(TMSCMD));
	memcpy(pos, &crc, sizeof(crc));

	int setsize = pKL->length + sizeof(TMSCMD) + sizeof(crc);
	return Write(buf, setsize, setsize);
}