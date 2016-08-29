#include "NetCommThread.h"
#include "message.h"
#include "ini.h"
#include "BaseOperation.h"
#include "../dvb/PATDataProcess.h"

#include <stdio.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <log/Log.h>
#include <log/LogStruct.h>

using namespace brunt;
extern ILog* gLog;
extern TUNER_INFO gInfo;
extern RECEIVE_INFO gRecv;
extern TUNER_CONF gTuner;
extern char *gMd5;
extern PATDataThread* pPat;

#pragma pack(1)
struct INET_DESCRIPTOR
{
	uint8 preamble;
	uint8 flag;
	uint32 reserved;
	uint16 command;
	uint16 subCommand;
	uint32 payloadLength;
	INET_DESCRIPTOR()
	{
		preamble = 0x55;
		reserved = 0x00000000;
		flag = 0;
	}
};

enum ENCRYPTION
{
	ENC_NONE = 0,
	ENC_HARDKEY,
	ENC_SESSIONKEY
};

char gVersion[16];

#include <time.h>
string getDateTime()
{
	time_t t;
	time(&t);
	struct tm *pTm;
	pTm = localtime(&t);
	
	char tt[20];
	sprintf(tt,
		"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
		pTm->tm_year + 1900,
		pTm->tm_mon + 1,
		pTm->tm_mday,
		pTm->tm_hour,
		pTm->tm_min,
		pTm->tm_sec);
	string s = tt;
	return s;
}

void print_hex(char* buf, int len) {
	//Print results:
	for(int i=0;i<len;i++) {
		DPRINTF("%02X ",(unsigned char)buf[i]);
		if(15 == i%16)
			DPRINTF("\n");
	}
	DPRINTF("\n");
}

int getTime()
{
	time_t t;
	time(&t);
	return t;
}

void aes_encrypt(uint8 *key, uint8 *in, uint8 *out, uint32 size)
{
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	EVP_CIPHER_CTX ctx;
	const EVP_CIPHER *cipher;
	EVP_CIPHER_CTX_init(&ctx);
	cipher = EVP_aes_128_ecb();
	EVP_EncryptInit_ex(&ctx, cipher, NULL, key, NULL);

	int outlen = 0, tmplen;
	EVP_EncryptUpdate(&ctx, out+outlen, &outlen, in, size);
	//EVP_EncryptFinal_ex(&ctx, out + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

void aes_decrypt(uint8* key, uint8* in, uint8* out, uint32 size)
{
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

// 	DPRINTF("decrypt size 0x%04x\n", size);
// 	DPRINTF("Key %02x %02x %02x %02x \n", key[0], key[1], key[2], key[3]);
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	int outlen, tmplen;
	outlen = 0;
// 	print_hex((char*)in, size);
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);
	EVP_DecryptUpdate(&ctx, out + outlen, &outlen, in, size);
// 	DPRINTF("outlen %02x \n", outlen);
	//EVP_DecryptFinal_ex(&ctx, out + outlen, &tmplen);
// 	DPRINTF("templen %02x \n", tmplen);
// 	print_hex((char*)out, size);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

void save_proto(const char *headbuf, int headsize, int ID, const char* bodybuf, int bodysize)
{
#ifdef SAVE_PROTO
	char fn[256];
	sprintf(fn, "/var/log/CineCast/%s_%02d.pro", getDateTime().c_str(), ID);
	FILE *fp = fopen(fn, "wb");
	if(fp)
	{
		fwrite(headbuf, headsize, 1, fp);
		if(bodybuf != NULL)
			fwrite(bodybuf, bodysize, 1, fp);
		fclose(fp);
	}
#endif
}

class HeartThread: public brunt::CActiveThread
{
public:
    HeartThread();
    virtual ~HeartThread();
    bool Init(NetCommThread* pNetCom);
    bool Start();
    bool Stop();
    
private:
    virtual void doit();
    void cleanThread();

private:
    NetCommThread* pNetCom;
	bool bExit;
};

HeartThread::HeartThread()
{
    pNetCom = NULL;
}

HeartThread::~HeartThread()
{
	Stop();
    cleanThread();
}

bool HeartThread::Init(NetCommThread* pNetCom)
{
    this->pNetCom = pNetCom;
    return true;
}

bool HeartThread::Start()
{
	char str[512];

	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[HeartThread] The thread had not been ready, status = %d.", status());
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
		sprintf(str, "[HeartThread] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false; 
	}
	bExit = false;
	return true;
}

void HeartThread::cleanThread()
{
	CActiveThread::stop();
}

bool HeartThread::Stop()
{
	bExit = true;
}

void HeartThread::doit()
{
	char str[512];

	int i = 0;
	while(!bExit)
	{
		i++;
		if(i > 120*1000)
		{
			if(pNetCom)
			{
				pNetCom->HeartBreat();
			}
			i = 0;
		}
		else
			usleep(1000);
	}
}

NetCommThread gNetComm;
NetCommThread* CreateNetComm()
{
	return &gNetComm;
}
void ReleaseNetComm(NetCommThread* pNet)
{

}

NetCommThread::NetCommThread():m_mutex(0), bConnected(false), bPkgSendStart(false), pHeartThread(NULL), bPause(false), nHeartCount(0), nDNSCount(0)
{
	string s = getDateTime();
	memset(m_loginReq.startupTime, 0, 20);
	strcpy((char*)m_loginReq.startupTime, s.c_str());
	m_loginReq.loginCount = 0;
	nRecvLength = 0;
	
	ICMyini* ini = createMyini();
	bool bRead = false;
	if(ini)
	{
		memset(gVersion, 0, 16);
		if (ini->load("/etc/CineCast/Version"))
		{
			std::string tmp;
			if(ini->read("General", "Version", tmp))
			{
				memcpy(gVersion, tmp.c_str(), tmp.size());
				bRead = true;
			}
		}
	}
	if (!bRead)
	{
		memcpy(m_loginReq.version, "0.0.0.0", 7);
		gLog->Write(LOG_NETCOMMU, "[NetCommThread] read version failed");
	}

	pHeartThread = new HeartThread;
	pHeartThread->Init(this);
}

NetCommThread::~NetCommThread()
{
	if(pHeartThread)
	{
		pHeartThread->Stop();
		delete pHeartThread;
	}
		
	cleanThread();
}

void NetCommThread::StartRecvTask()
{
	m_strTaskTime = getDateTime();
}

void NetCommThread::StartRoundRecv()
{
	m_strRoundTime = getDateTime();
}

bool NetCommThread::Init(bool bLeonis)
{
	bLeonisCfg = bLeonis;
	return true;
}

bool NetCommThread::Start()
{
	char str[512];

	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[NetCommThread] The thread had not been ready, status = %d.", status());
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
		sprintf(str, "[NetCommThread] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false; 
	}

	m_status = NET_RUN;

	pHeartThread->Start();
	return true;
}

void NetCommThread::cleanThread()
{
	m_status = NET_STOP;
	m_loginSocket.Destroy();
	m_authSocket.Destroy();
	CActiveThread::stop();
	m_status = STOP;
}

bool NetCommThread::Stop()
{
	m_status = NET_STOP;

	usleep(100000);

	m_loginSocket.Destroy();
	m_authSocket.Destroy();
}

void NetCommThread::GetMutex()
{
	while(m_mutex) usleep(1000);
	m_mutex = 1;
}

void NetCommThread::ReleaseMutex()
{
	m_mutex = 0;
}

bool NetCommThread::Login()
{
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
#if 1
	inet_descriptor.flag = ENC_HARDKEY;
#else
	inet_descriptor.flag = ENC_NONE;
#endif
	inet_descriptor.command = NET_LOGIN_REQ;
	inet_descriptor.subCommand = 0x0000;
	inet_descriptor.payloadLength = sizeof(struct L_LOGIN_REQ);

	//construct login request
	m_loginReq.machineID = m_nMachineId;
	memset(m_loginReq.loginTime, 0, 20);
	strcpy((char*)m_loginReq.loginTime, getDateTime().c_str());
	m_loginReq.loginCount++;

	memset(m_loginReq.version, 0, 16);
	ICMyini* ini = createMyini();
	bool bRead = false;
	if(ini)
	{
		if (ini->load("/etc/CineCast/Version"))
		{
			std::string tmp;
			if(ini->read("General", "Version", tmp))
			{
				memcpy(m_loginReq.version, tmp.c_str(), tmp.size());
// 				if(gLog)
// 					gLog->Write(LOG_NETCOMMU, tmp.c_str());
				bRead = true;
			}
		}
	}
	if (!bRead)
	{
		memcpy(m_loginReq.version, "0.0.0.0", 7);
		gLog->Write(LOG_NETCOMMU, "[NetCommThread] read version failed");
	}

	memcpy(gVersion, m_loginReq.version, 16);

	m_loginReq.crc32 = calc_crc32((uint8*)&m_loginReq,
		sizeof(L_LOGIN_REQ) - 4);

	//Encrypt login request with AES128 ECB
	struct L_LOGIN_REQ req;
	memcpy(&req, &m_loginReq, sizeof(L_LOGIN_REQ));
	if(inet_descriptor.flag == ENC_HARDKEY)
	{
		aes_encrypt((uint8*)m_hardKey, 
			(uint8*)&req+sizeof(uint32), 
			(uint8*)&req+sizeof(uint32),
			(uint32)sizeof(struct L_LOGIN_REQ)-sizeof(uint32));
	}

	//Send login request to remote
	size_t send_size = 0;
	t_timeout tm = 8000;

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&req, sizeof(struct L_LOGIN_REQ));

	char str[512];

	int reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Login send head\n");
		int err = m_loginSocket.Send2((char*)&inet_descriptor,
			sizeof(struct INET_DESCRIPTOR),
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Login WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
// 				bConnected = false;
// 				m_loginSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Login WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
// 			bConnected = false;
// 			m_loginSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	send_size = 0;
	tm = 8000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Login send body\n");
		int err = m_loginSocket.Send2((char*)&req,
			sizeof(struct L_LOGIN_REQ),
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Login WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
// 				bConnected = false;
// 				m_loginSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Login WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
// 			bConnected = false;
// 			m_loginSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	//Read login response
	size_t get_size = 0;
	tm = 10000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Login read head\n");
		int err = m_loginSocket.Receive2((char*)&inet_descriptor,
			(int)sizeof(struct INET_DESCRIPTOR),
			get_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Login WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 5)
			{
// 				bConnected = false;
// 				m_loginSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Login ReadError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
// 			bConnected = false;
// 			m_loginSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	get_size = 0;
	tm = 10000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Login read Body\n");
		int err = m_loginSocket.Receive2((char*)&m_loginRep,
			(int)sizeof(struct R_LOGIN_REP),
			get_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Login WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 5)
			{
// 				bConnected = false;
// 				m_loginSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Login ReadError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
// 			bConnected = false;
// 			m_loginSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

//	m_loginSocket.SetBlocking(true);

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&m_loginRep, sizeof(struct R_LOGIN_REP));

	if(inet_descriptor.subCommand != 0)
	{
		sprintf(str, "[NetCommThread] Login receive %x from server", (uint16)inet_descriptor.subCommand);
		DPRINTF("%s\n", str);
		gLog->Write(LOG_ERROR, str);
		return false;
	}

	//decrypt login response
	if(inet_descriptor.flag == ENC_HARDKEY)
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)&m_loginRep,
		(uint8*)&m_loginRep,
		sizeof(struct R_LOGIN_REP));

	uint32 crc32 = calc_crc32((uint8*)&m_loginRep,
		sizeof(struct R_LOGIN_REP) - 4);
	if(crc32 == m_loginRep.crc32)
	{
		sprintf(str,
			"\"%04u-%02u-%02u %02u:%02u:%02u\"",
			m_loginRep.sys_year,
			m_loginRep.sys_month,
			m_loginRep.sys_date,
			m_loginRep.sys_hour,
			m_loginRep.sys_minute,
			m_loginRep.sys_second);
		DPRINTF("Received remote date %s", str);
		System sys;
		sys.SetDateTime(str);
		return true;
	}
	sprintf(str, "[NetCommThread] Login receive crc error");
	DPRINTF("%s\n", str);
	gLog->Write(LOG_ERROR, str);
	DPRINTF("CRC %08x %08x\n", crc32, m_loginRep.crc32);
	return false;
}

bool NetCommThread::Auth()
{
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
#if 1
	inet_descriptor.flag = ENC_HARDKEY;
#else
	inet_descriptor.flag = ENC_NONE;
#endif
	inet_descriptor.command = NET_AUTH_REQ;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_AUTH_REQ);

	struct L_AUTH_REQ m_authReq;

	//construct auth request
	m_authReq.machineID = m_nMachineId;

	//Get and set Version number
	memset(m_authReq.version, 0, 16);
	ICMyini* ini = createMyini();
	bool bRead = false;
	if(ini)
	{
		if (ini->load("/etc/CineCast/Version"))
		{
			std::string tmp;
			if(ini->read("General", "Version", tmp))
			{
				memcpy(m_authReq.version, tmp.c_str(), tmp.size());
				if(gLog)
					gLog->Write(LOG_NETCOMMU, tmp.c_str());
				bRead = true;
			}
		}
	}
	if (!bRead)
	{
		memcpy(m_authReq.version, "0.0.0.0", 7);
		gLog->Write(LOG_NETCOMMU, "[NetCommThread] read version failed");
	}

	memset(m_authReq.loginTime, 0, 20);
	strcpy((char*)m_authReq.loginTime, getDateTime().c_str());
	m_authReq.logMode = 2; //do not upload log file
	m_authReq.heartCycle = 120; //120 seconds, 2 minutes
	m_authReq.connectMode = 1; //always connect.
	m_authReq.diskNum = 1;

	ContentOperation co;
	m_authReq.diskCapacity = co.GetTotalSpace(0);
	m_authReq.reserved_1 = m_authReq.reserved_2 = 0;
	m_authReq.crc32 = calc_crc32((uint8*)&m_authReq,
		sizeof(L_AUTH_REQ) - 4);
	//Use new struct for 16 bytes PAD
	//Original: Data CRC32 PAD
	//New: Data PAD CRC32

	int leng = sizeof(L_AUTH_REQ);
	uint8 *buf = new uint8[leng+300];
	memset(buf, 0, leng+300);
	char str[512];
	if(buf == NULL)
	{
		sprintf(str, "[NetCommThread] Auth new buffer failed");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false;
	}
	memcpy(buf, &m_authReq, leng);

#if 0
	leng = ((leng+15)/16)*16;
	*(uint32*)(buf + leng) = calc_crc32((uint8*)buf, leng);
#endif
	//Encrypt login request with AES128 ECB
	if(inet_descriptor.flag == ENC_HARDKEY)
	{
	#if 0
		inet_descriptor.payloadLength = ((sizeof(struct L_AUTH_REQ)-sizeof(uint32)+15)/16)*16 + sizeof(uint32);
	#endif
		inet_descriptor.payloadLength = leng;
		aes_encrypt((uint8*)m_hardKey, 
			(uint8*)buf+sizeof(uint32), 
			(uint8*)buf+sizeof(uint32),
			inet_descriptor.payloadLength);
	}

	//Send login request to remote
	size_t send_size = 0;

	t_timeout tm = 8000;

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)buf, inet_descriptor.payloadLength);

	int reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Auth send head\n");
		int err = m_authSocket.Send2((char*)&inet_descriptor,
			sizeof(struct INET_DESCRIPTOR),
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Auth WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
				delete[] buf;
// 				bConnected = false;
// 				m_authSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Auth WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			delete[] buf;
// 			bConnected = false;
// 			m_authSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	send_size = 0;
	tm = 8000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Auth send body\n");
		int err = m_authSocket.Send2((char*)buf,
			inet_descriptor.payloadLength,
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Auth WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
				delete[] buf;
// 				bConnected = false;
// 				m_authSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Auth WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			delete[] buf;
// 			bConnected = false;
// 			m_authSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	//Read auth response
	size_t get_size = 0;
	tm = 10000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Auth Get head\n");
		int err = m_authSocket.Receive2((char*)&inet_descriptor,
			sizeof(struct INET_DESCRIPTOR),
			get_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Auth WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 5)
			{
				delete[] buf;
// 				bConnected = false;
// 				m_authSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Auth WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			delete[] buf;
// 			bConnected = false;
// 			m_authSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

	get_size = 0;
	tm = 10000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Auth Get body\n");
		int err = m_authSocket.Receive2((char*)buf,
			inet_descriptor.payloadLength,
			get_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] Auth WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 5)
			{
				delete[] buf;
// 				bConnected = false;
// 				m_authSocket.Destroy();
// 				if(m_status == NET_READ)
// 					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] Auth WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			delete[] buf;
// 			bConnected = false;
// 			m_authSocket.Destroy();
// 			if(m_status == NET_READ)
// 				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

//	m_authSocket.SetBlocking(true);
	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)buf, inet_descriptor.payloadLength);

// 	DPRINTF("auth %04x\n", inet_descriptor.payloadLength);
// 	print_hex((char*)buf, inet_descriptor.payloadLength);
	//decrypt auth response
	if(inet_descriptor.flag == ENC_HARDKEY)
	{
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)buf,
		(uint8*)buf,
		inet_descriptor.payloadLength);
	}
	memcpy(&m_authRep, buf, sizeof(R_AUTH_REP));
// 	print_hex((char*)&m_authRep, sizeof(m_authRep));

// 	DPRINTF("Calc CRC, size %04x\n", sizeof(m_authRep) - sizeof(uint32));
	uint32 crc32 = calc_crc32((uint8*)buf,
		sizeof(R_AUTH_REP) - sizeof(uint32));
// 	DPRINTF("CRC %08x\n", crc32);
	if(crc32 == m_authRep.crc32)
	{
		sprintf(str,
			"[NetCommThread] Auth get session key: %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X",
			m_authRep.sessionKey[0], m_authRep.sessionKey[1], m_authRep.sessionKey[2], m_authRep.sessionKey[3], 
			m_authRep.sessionKey[4], m_authRep.sessionKey[5], m_authRep.sessionKey[6], m_authRep.sessionKey[7], 
			m_authRep.sessionKey[8], m_authRep.sessionKey[9], m_authRep.sessionKey[10], m_authRep.sessionKey[11], 
			m_authRep.sessionKey[12], m_authRep.sessionKey[13], m_authRep.sessionKey[14], m_authRep.sessionKey[15]);
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		delete[] buf;
// 		DPRINTF("Free buf success\n");
		return true;
	}
	if (gLog)
		gLog->Write(LOG_ERROR, "[NetCommThread] Auth crc error");
	delete[] buf;
	return false;
}

bool NetCommThread::GetMD5File(uint32 filmId)
{
	if(!bConnected)
	{
		char str[512];
		sprintf(str, "[NetCommThread] GetMD5File: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_MD5_REQ;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_MD5_KEY_REQ);

	bMd5Rep = false;

	struct L_MD5_KEY_REQ m_md5Req;
	//construct md5 request
	m_md5Req.filmID = filmId;
	m_md5Req.crc32 = calc_crc32((uint8*)&m_md5Req,
		sizeof(L_MD5_KEY_REQ) - 4);
	
	//Encrypt md5 request with AES128 ECB
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&m_md5Req, 
		(uint8*)&m_md5Req,
		sizeof(struct L_MD5_KEY_REQ));

	//Send md5 request to remote
	size_t send_size = 0;

	t_timeout tm = 10000;

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&m_md5Req, sizeof(struct L_MD5_KEY_REQ));

	GetMutex();
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&m_md5Req,
		sizeof(struct L_MD5_KEY_REQ),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;
		return false;
	}
	ReleaseMutex();

	//Update state to md5 request
	gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 0x07;

// 	int t1 = getTime();
// 	while(!bMd5Rep)
// 	{
// 		if(getTime() > (t1 + 10))
// 			return false;
// 	}
	return true;
}

bool NetCommThread::ReportLost(char* buf, int nSize, int nLeoSize)
{
	if(!bConnected || (m_status != NET_READ))
	{
		char str[512];
		sprintf(str, "[NetCommThread] ReportLost: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	if(bPkgSendStart)
	{
		char str[512];
		sprintf(str, "[NetCommThread] ReportLost: still recive data from remote.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_LOST_UPLOAD;
	inet_descriptor.subCommand = 0x00;

	if(bLeonisCfg)
	{
		uint32 crc32 = calc_crc32((uint8*)buf, nLeoSize - sizeof(uint32));
		memcpy(buf + nLeoSize - sizeof(uint32), &crc32, sizeof(uint32));
		inet_descriptor.payloadLength = nLeoSize;
	}
	else
	{
	#if 0
		uint32 crc32 = calc_crc32((uint8*)buf, nSize - sizeof(uint32));
		memcpy(buf + nSize - sizeof(uint32), &crc32, sizeof(uint32));
		inet_descriptor.payloadLength = nSize;
	#endif
		*(uint32*)(buf + nSize - sizeof(uint32)) = calc_crc32((uint8*) buf, nSize - sizeof(uint32));
		DPRINTF("CRC %08X\n", *(uint32*)(buf + nSize - sizeof(uint32)));
		inet_descriptor.payloadLength = nSize;
	}
//	print_hex((char*)buf, 0x40);
	
	if (inet_descriptor.flag != ENC_NONE)
	{
		aes_encrypt((uint8*)m_authRep.sessionKey, 
			(uint8*)buf, 
			(uint8*)buf,
			inet_descriptor.payloadLength);
		#if 0
		inet_descriptor.payloadLength = ((inet_descriptor.payloadLength + 15)/16)*16;
		#endif
	}

	size_t send_size = 0;
	t_timeout tm = 10000;

	save_proto((char*)&inet_descriptor, sizeof(INET_DESCRIPTOR), inet_descriptor.command,
		(char*)buf, inet_descriptor.payloadLength);

	GetMutex();
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)buf,
		inet_descriptor.payloadLength,
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	ReleaseMutex();
	//Update state to upload lost info
	gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) | 0x05;

	return true;
}

bool NetCommThread::HeartBreat()
{
	static uint32 dataRate = 80;
	DPRINTF("Heartbreat\n");
	if(!bConnected)
	{
		char str[512];
		sprintf(str, "[NetCommThread] HeartBreat: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false;
	}
	if(m_status != NET_READ)
	{
		char str[512];
		sprintf(str, "[NetCommThread] HeartBreat: not auth from remote.");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false;
	}

	L_HEART_INFO_REPORT m_sHeart;
	m_sHeart.machineID = m_nMachineId;
	m_sHeart.temperature = 30;
	m_sHeart.CPUUsage = 10;
	m_sHeart.memTotal = 4096;
	m_sHeart.memIdle = 1024;
	m_sHeart.networkRate = 2;
	m_sHeart.agc = gInfo.nAGC;
	m_sHeart.snr = gInfo.nSNR;
	m_sHeart.interfaceRate = gTuner.nSR/1000;
	
        m_sHeart.dataRate = (gRecv.nReceiveLength - nRecvLength)*8/1000/1000/120;
	nRecvLength = gRecv.nReceiveLength;
	
	if(m_sHeart.dataRate < 80)
	m_sHeart.dataRate = 80;
    
	m_sHeart.reserved;
	m_sHeart.filmID = gRecv.nFileID;
	m_sHeart.filmNameLength = gRecv.strFilmName.size();
	m_sHeart.filmLength = gRecv.nFileLength;
	m_sHeart.filmSegment = gRecv.nTotalSegment;
	m_sHeart.recvRound = gRecv.nReceiveStatus >> 16;
	memcpy(m_sHeart.taskStartTime, m_strTaskTime.c_str(), m_strTaskTime.size());
	memcpy(m_sHeart.recvStartTime, m_strRoundTime.c_str(), m_strRoundTime.size());
	m_sHeart.filmRecvState = gRecv.nReceiveStatus & 0xffff;
	m_sHeart.reserved2 = 0;
	m_sHeart.recvLength = gRecv.nReceiveLength;
	
	m_sHeart.lostSegment = gRecv.nLostSegment;

	int leng = sizeof(L_HEART_INFO_REPORT) - sizeof(uint8*) + m_sHeart.filmNameLength;
	uint8* buf = new uint8[leng+100];
	memset(buf, 0, leng+100);


	for(int i = 0; i < leng+100; i++)
	    buf[i] = i;

	char str[512];
	if(buf == NULL)
	{
		sprintf(str, "[NetCommThread] HeartBreat new buffer failed");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false;
	}

	uint8* pos = buf;
	memcpy(pos, &m_sHeart.machineID, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.temperature, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.CPUUsage, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.memTotal, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.memIdle, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.networkRate, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.agc, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.snr, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.interfaceRate, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.dataRate, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.reserved, 16);
	pos += 16;

	memcpy(pos, &m_sHeart.filmID, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.filmNameLength, sizeof(uint32));
	pos += sizeof(uint32);

	if(m_sHeart.filmNameLength > 0)
	{
		memcpy(pos, gRecv.strFilmName.c_str(), m_sHeart.filmNameLength);
		pos += m_sHeart.filmNameLength;
	}

	memcpy(pos, &m_sHeart.filmLength, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.filmSegment, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.recvRound, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.taskStartTime, 20);
	pos += 20;

	memcpy(pos, &m_sHeart.recvStartTime, 20);
	pos += 20;

	memcpy(pos, &m_sHeart.filmRecvState, sizeof(uint32));
	pos += sizeof(uint32);

	memcpy(pos, &m_sHeart.reserved2, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.recvLength, sizeof(uint64));
	pos += sizeof(uint64);

	memcpy(pos, &m_sHeart.lostSegment, sizeof(uint64));
	pos += sizeof(uint64);

	//pos = ((pos - buf + 15) / 16) * 16 + buf;
	//*(uint32*)(pos - sizeof(uint32)) = calc_crc32(buf, (pos - buf - sizeof(uint32)));
	*(uint32*)(pos) = calc_crc32(buf, (pos - buf));
	pos += sizeof(uint32);
	//uint32 crc = calc_crc32(buf, (pos - buf));
	//memcpy(pos, &crc, sizeof(crc));
	//pos +=sizeof(crc);

	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
// 	inet_descriptor.flag = ENC_NONE;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_HEARTBEAT_UPLOAD;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = pos - buf;

//  	print_hex((char*)buf, inet_descriptor.payloadLength);
	//Encrypt request with AES128 ECB
	if(inet_descriptor.flag != ENC_NONE)
	{
		aes_encrypt((uint8*)m_authRep.sessionKey, 
			(uint8*)buf, 
			(uint8*)buf,
			inet_descriptor.payloadLength);
// 		inet_descriptor.payloadLength = ((inet_descriptor.payloadLength+15)/16)*16;
	}

	//Send md5 request to remote
	size_t send_size = 0;
// 	DPRINTF("Send head\n");

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)buf, inet_descriptor.payloadLength);
	
	GetMutex();
	t_timeout tm = 8000;
	int reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Heartbreat send head\n");
		int err = m_authSocket.Send2((char*)&inet_descriptor,
			sizeof(struct INET_DESCRIPTOR),
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] HeartBreat WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
				ReleaseMutex();
				delete[] buf;
				bConnected = false;
				m_authSocket.Destroy();
				if(m_status == NET_READ)
					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] HeartBreat WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			ReleaseMutex();
			delete[] buf;
			bConnected = false;
			m_authSocket.Destroy();
			if(m_status == NET_READ)
				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}

// 	DPRINTF("Send data\n");
	send_size = 0;
	tm = 8000;
	reTry = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Heartbreat send body\n");
		int err = m_authSocket.Send2((char*)buf,
			inet_descriptor.payloadLength,
			send_size,
			&tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] HeartBreat WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 3)
			{
				ReleaseMutex();
				delete[] buf;
				bConnected = false;
				m_authSocket.Destroy();
				if(m_status == NET_READ)
					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] HeartBreat WriteError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			ReleaseMutex();
			delete[] buf;
			bConnected = false;
			m_authSocket.Destroy();
			if(m_status == NET_READ)
				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}
	ReleaseMutex();
	nHeartCount = 10;
#if 0
// 	DPRINTF("delete buf\n");
	tm = 10000;
	reTry = 0;
	struct INET_DESCRIPTOR m_inetDesc;
	size_t get_size = 0;
	for(;;)
	{
		reTry++;
		DPRINTF("Heartbreat Recv head\n");
		int err = m_authSocket.Receive2((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), get_size, &tm);
		if (err == CZSocket::WAIT_R_TIMEOUT)
		{
			sprintf(str, "[NetCommThread] HeartBreat RECV WAIT_R_TIMEOUT");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			if(reTry > 5)
			{
				delete[] buf;
				bConnected = false;
				m_authSocket.Destroy();
				if(m_status == NET_READ)
					m_status = NET_RUN;
				return false;
			}
		}
		else if(err < 0)
		{
			sprintf(str, "[NetCommThread] HeartBreat ReadError");
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			delete[] buf;
			bConnected = false;
			m_authSocket.Destroy();
			if(m_status == NET_READ)
				m_status = NET_RUN;
			return false;
		}
		else
			break;
	}
	if(m_inetDesc.command == NET_HEARTBEAT_CONFIRM)
	{
		DPRINTF("Receive heartbreat confirm\n");
	}
	else
	{
		DPRINTF("Receive unknow confirm %04x\n", m_inetDesc.command);		
	}
#endif
	delete[] buf;
	return true;
}

void NetCommThread::GetHardKey(const std::string hardkey)
{
	std::string skey = hardkey;
	char buf[2];
	memset(m_hardKey, 0, 16);
	int length;
	if(skey.size() > 32)
		length = 32/2;
	else
		length = skey.size()/2;
	for(int i = 0; i < length; i++)
	{
		buf[0] = skey.at(i*2);
		buf[1] = skey.at(i*2+1);
		char a;
		sscanf(buf, "%02X", m_hardKey+i);
	}
}

void NetCommThread::doit()
{
	char str[512];

	while(1)
	{
		if(bPause)
		{
			usleep(100000);
		}
		else
		{
			switch(m_status)
			{
			case NET_RUN:
				{
					bConnected = false;
					ICMyini* ini = createMyini();
					std::string tmp;

					std::string url;
					int port;
					if(ini)
					{
						if (ini->load("/etc/CineCast/CineCast.cfg"))
						{
							if(bLeonisCfg)
							{
								if(ini->read(" ", "LEONIS-SERVER", tmp))
								{
									url = tmp;
								}
								else
								{
									url = "";
									if(gLog)
										gLog->Write(LOG_NETCOMMU, "[NetCommThread] read leonis server failed");
								}
								if (ini->read(" ", "LEONIS-PORT", tmp))
								{
									port = atoi(tmp.c_str());
								}
								else
								{
									if(gLog)
										gLog->Write(LOG_NETCOMMU, "[NetCommThread] read leonis port failed");
									port = 0;
								}
							}
							else
							{
								if(ini->read(" ", "SERVER", tmp))
								{
									url = tmp;
								}
								else
								{
									url = "";
									if(gLog)
										gLog->Write(LOG_NETCOMMU, "[NetCommThread] read server failed");
								}
								if (ini->read(" ", "PORT", tmp))
								{
									port = atoi(tmp.c_str());
								}
								else
								{
									if(gLog)
										gLog->Write(LOG_NETCOMMU, "[NetCommThread] read port failed");
									port = 0;
								}
							}
						}
						if (ini->load("/etc/CineCast/ID"))
						{
							if (ini->read("ID_HardKey", "ID", tmp))
							{
								m_nMachineId = atoi(tmp.c_str());
							}
							else
							{
								if(gLog)
									gLog->Write(LOG_NETCOMMU, "[NetCommThread] read MachineId failed");
								m_nMachineId = 0;
							}
							if(ini->read("ID_HardKey", "HardKey", tmp))
							{
								GetHardKey(tmp);
							}
							else
							{
								if(gLog)
									gLog->Write(LOG_NETCOMMU, "[NetCommThread] read HardKey failed");
							}
						}
						releaseMyini(ini);
					}
					try
					{
						if(m_loginSocket.Create(AF_INET, SOCK_STREAM, 0))
						{
							if(nDNSCount > 5)
							{
								url = "123.127.110.181";
								nDNSCount = 0;
							}
							struct sockaddr_in addr_in;
							memset(&addr_in, 0, sizeof(sockaddr_in));
							addr_in.sin_family = AF_INET;
							addr_in.sin_port = htons(port);
							struct hostent *he;
							if((he = gethostbyname(url.c_str())) == NULL)
							{
								if(gLog)
									gLog->Write(LOG_NETCOMMU, "[NetCommThread] gethostbyname error.");
								throw -2;
							}
							struct in_addr **addr_list = (struct in_addr**)he->h_addr_list;
							char ip[100];
							for(int i = 0; addr_list[i] != NULL; i++)
							{
								strcpy(ip, inet_ntoa(*addr_list[i]));
								break;
							}
							sprintf(str, "[NetCommThread] %s resolved to %s.", url.c_str(), ip);
							if(gLog)
								gLog->Write(LOG_ERROR, str);
							addr_in.sin_addr.s_addr = inet_addr(ip);
							int error = 0;
							do
							{
								t_timeout tm = 3000;
								error = m_loginSocket.Connect(&addr_in, &tm);
								if (error && error != CZSocket::WAIT_R_TIMEOUT)
								{
									throw -1;
								}
							}while(error);
							m_status = NET_LOGIN;
						}
					}
					catch(int& err)
					{
						m_loginSocket.Destroy();
						sleep(5);
						bConnected = false;
						sprintf(str, "[NetCommThread] Connect to login server %s:%d error.", url.c_str(), (uint16)port);
						if(gLog)
							gLog->Write(LOG_ERROR, str);
						DPRINTF("%s\n", str);
						if(err == -2)
							nDNSCount++;
					}
				}
				break;
			case NET_LOGIN:
				if(Login() == false)
				{
					bConnected = false;
					m_loginSocket.Destroy();
					m_status = NET_RUN;
					sleep(5);
				}
				else
				{
					m_loginSocket.Destroy();
					m_status = NET_AUTH;
				}
				break;
			case NET_AUTH:
				try
				{
					if(m_authSocket.Create(AF_INET, SOCK_STREAM, 0))
					{
						DPRINTF("server ip: %s:%d\n", m_loginRep.serverIP, m_loginRep.port);
						struct sockaddr_in addr_in;
						memset(&addr_in, 0, sizeof(sockaddr_in));
						addr_in.sin_family = AF_INET;
						addr_in.sin_port = htons(m_loginRep.port);
						addr_in.sin_addr.s_addr = inet_addr((char*)m_loginRep.serverIP);
						int error = 0;
						do
						{
							t_timeout tm = 3000;
							error = m_authSocket.Connect(&addr_in, &tm);
							if (error && error != CZSocket::WAIT_R_TIMEOUT)
							{
								m_authSocket.Destroy();
								throw -1;
							}
						}while(error);
						if(Auth() == false)
						{
							bConnected = false;
							m_authSocket.Destroy();
							m_status = NET_RUN;
						}
						else
						{
							DPRINTF("auth success\n");
							bConnected = true;
							m_status = NET_READ;
							if(gLog)
								gLog->Write(LOG_NETCOMMU, "[NetCommThread] Auth success.");
						}
					}
				}
				catch(int&)
				{
					m_status = NET_RUN;
					sleep(10);
					sprintf(str, "[NetCommThread] Connect to login server %s:%d error.", m_loginRep.serverIP, m_loginRep.port);
					if(gLog)
						gLog->Write(LOG_ERROR, str);
				}
				break;
			case NET_READ:
				{
					try
					{
						RecvFilter();
					}
					catch (int&)
					{
						DPRINTF("throw\n");
						bConnected = false;
						m_authSocket.Destroy();
						if(m_status == NET_READ)
							m_status = NET_RUN;
					}
				}
				break;
			case NET_REBOOT_REQ:
				//Don't need, we already process the reboot command in recvfilter
				break;
			case NET_STOP:
				DPRINTF("STOP\n");
				return;
			default:
				usleep(100*1000);
				break;
			}
		}
	}
}

void NetCommThread::CloseConnect()
{
	bPause = true;
	m_loginSocket.Destroy();
	m_authSocket.Destroy();
	bConnected = false;
}

void NetCommThread::StartConnect()
{
	bPause = false;
	m_status = NET_RUN;
}

bool NetCommThread::RecvFilter()
{
	struct INET_DESCRIPTOR m_inetDesc;
	size_t get_size = 0;
	static char a_buf[2048];
	static char b_buf[2048];
	char *buf = NULL;
	if(bLeonisCfg)
		buf = a_buf;
	else
		buf = b_buf;

	t_timeout tm = 3000;

	if(nHeartCount >= 10)
		nHeartCount++;
	int err = m_authSocket.Receive2((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), get_size, &tm);
	if( err == CZSocket::WAIT_R_TIMEOUT)
	{
		if(nHeartCount > 15)
		{
			nHeartCount = 0;
			throw -1;
		}
// 		DPRINTF("Receive timeout\n");
		return false;
	}
	if( err < 0)
	{
		nHeartCount = 0;
		throw -1;
	}

	switch(m_inetDesc.command)
	{
	case NET_HEARTBEAT_REQ:

		save_proto((char*)&m_inetDesc, sizeof(struct INET_DESCRIPTOR), m_inetDesc.command,
			NULL, 0);

		DPRINTF("Receive heartbreat request\n");
		HeartBreat();
		break;
	case NET_HEARTBEAT_CONFIRM:

		save_proto((char*)&m_inetDesc, sizeof(struct INET_DESCRIPTOR), m_inetDesc.command,
			NULL, 0);
		nHeartCount = 0;
		DPRINTF("Receive heartbreat confirm\n");
		break;
	case NET_MD5_REP:
		{
			DPRINTF("Receive Md5 responsed\n");
			
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
			{
				buf1 = buf;
				memset(buf1, 0, 2048);
			}
			else
			{
				buf1 = new char[m_inetDesc.payloadLength + 100];
				DPRINTF("New MD5 length:%d\n", m_inetDesc.payloadLength);
				memset(buf1, 0, m_inetDesc.payloadLength + 100);
				bNew = true;
			}
			if(buf1)
			{
				tm = 1000;
				get_size = 0;

				if (m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size) < 0)
				{
					DPRINTF("Receive error\n");
					if(bNew)
						delete[] buf1;
					if (gLog)
					{
						gLog->Write(LOG_ERROR, "MD5 Content Recevie < 0");
					}
					throw -1;
				}
				DPRINTF("MD5 Responsed length:%d\n", get_size);

				save_proto((char*)&m_inetDesc, sizeof(struct INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);

				if(crc32 == *(uint32*)pCrc32)
				{
					DPRINTF("Send Md5 receive confirm\n");
					struct L_MD5_KEY_CONFIRM confirm;
					struct INET_DESCRIPTOR inet_desc;
					inet_desc.command = NET_MD5_CONFIRM;
					inet_desc.subCommand = 0;
					inet_desc.payloadLength = sizeof(L_MD5_KEY_CONFIRM);
					inet_desc.flag = ENC_SESSIONKEY;
					confirm.filmID = gRecv.nFileID;
					DPRINTF("MD5 confirm %u\n", confirm.filmID);
					confirm.crc32 = calc_crc32((uint8 *)&confirm,
						sizeof(L_MD5_KEY_CONFIRM) - 4);
					size_t send_size = 0;

					//Encrypt md5 request with AES128 ECB
					aes_encrypt((uint8*)m_authRep.sessionKey, 
						(uint8*)&confirm, 
						(uint8*)&confirm,
						sizeof(struct L_MD5_KEY_CONFIRM));

					save_proto((char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), inet_desc.command, 
						(char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM));

					GetMutex();
					m_authSocket.Send((const char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), send_size);
					m_authSocket.Send((const char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM), send_size);
					ReleaseMutex();

					R_MD5_KEY_REP *pMd5 = (R_MD5_KEY_REP*)buf1;
					if(gMd5 != NULL)
					{
						delete[] gMd5;
						gMd5 = NULL;
					}

					gMd5 = new char[pMd5->fileLength];
					if(gMd5)
					{
						memcpy(gMd5, &pMd5->fileContent, pMd5->fileLength);
						gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 0x08;
						DPRINTF("Receive MD5File\n");
					}
					else
					{
						if (gLog)
						{
							gLog->Write(LOG_ERROR, "New MD5 buffer error");
						}
						DPRINTF("New gMd5 error\n");
					}
				}
				else
				{
					DPRINTF("CRC ERROR recv %08X calac %08X\n", *pCrc32, crc32);
					if (gLog)
					{
						gLog->Write(LOG_ERROR, "MD5 CRC error");
					}
					struct L_MD5_KEY_CONFIRM confirm;
					struct INET_DESCRIPTOR inet_desc;
					inet_desc.command = NET_MD5_CONFIRM;
					inet_desc.subCommand = 0;
					inet_desc.payloadLength = sizeof(L_MD5_KEY_CONFIRM);
					inet_desc.flag = ENC_SESSIONKEY;
					confirm.filmID = gRecv.nFileID;
					DPRINTF("MD5 confirm %u\n", confirm.filmID);
					confirm.crc32 = calc_crc32((uint8 *)&confirm,
						sizeof(L_MD5_KEY_CONFIRM) - 4);
					size_t send_size = 0;

					//Encrypt md5 request with AES128 ECB
					aes_encrypt((uint8*)m_authRep.sessionKey, 
						(uint8*)&confirm, 
						(uint8*)&confirm,
						sizeof(struct L_MD5_KEY_CONFIRM));

					save_proto((char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), inet_desc.command, 
						(char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM));

					GetMutex();
					m_authSocket.Send((const char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), send_size);
					m_authSocket.Send((const char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM), send_size);
					ReleaseMutex();

					R_MD5_KEY_REP *pMd5 = (R_MD5_KEY_REP*)buf1;
					if(gMd5 != NULL)
					{
						delete[] gMd5;
						gMd5 = NULL;
					}

					gMd5 = new char[pMd5->fileLength];
					if(gMd5)
					{
						memcpy(gMd5, &pMd5->fileContent, pMd5->fileLength);
						gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 0x08;
						DPRINTF("Receive MD5File\n");
					}
					else
					{
						if (gLog)
						{
							gLog->Write(LOG_ERROR, "New MD5 buffer error");
						}
						DPRINTF("New gMd5 error\n");
					}
				}
				if(bNew)
				{
					DPRINTF("free buf\n");
					delete[] buf1;
				}
			}
			else
			{
				DPRINTF("new buf error\n");
				if (gLog)
				{
					gLog->Write(LOG_ERROR, "MD5 new buffer error");
				}
			}		
		}
		break;
	case NET_DECRYPT_REQ:
		{
			DPRINTF("Receive MD5 result request\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
			{
			    buf1 = buf;
			}
			else
			{
			    buf1 = new char[m_inetDesc.payloadLength];
			    bNew = true;
			}
			if(buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}
				
				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);

				}
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				DPRINTF("%08x %08x\n", crc32, *(uint32*)pCrc32);
				if(crc32 == *(uint32*)pCrc32)
				{
					DPRINTF("Return MD5 result\n");
					R_MD5_RESULT_REQ* req = (R_MD5_RESULT_REQ*)buf1;
					DecryptRep(req->filmID);
				}
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_DECRYPT_CONFIRM:
		{
			DPRINTF("Receive MD5 result confirm\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
			{
				buf1 = buf;
			}
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if(buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);

				}
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				if(crc32 == *(uint32*)pCrc32)
				{
				}
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_LOG_REQ:
		{
			DPRINTF("Receive log request\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
			{
				buf1 = buf;
			}
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if (buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);

				}
#if 0
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(R_LOG_REQ) - sizeof(uint32));
				R_LOG_REQ *pReq = (R_LOG_REQ*)buf1;
				if(crc32 == pReq->crc32)
				{
					LogUpload(pReq->logStart, pReq->logEnd, false);
				}
#else
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				R_LOG_REQ *pReq = (R_LOG_REQ*)buf1;
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				if(crc32 == *(uint32*)pCrc32)
				{
					LogUpload(pReq->logStart, pReq->logEnd, false);
				}
#endif
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_LOG_REQ_LEONIS:
		{
			DPRINTF("Receive leonis log request\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if (buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}
				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

#if 0
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(R_LOG_REQ) - sizeof(uint32));
				R_LOG_REQ *pReq = (R_LOG_REQ*)buf1;
				if(crc32 == pReq->crc32)
				{
					LogUpload(pReq->logStart, pReq->logEnd, false);
				}
#else
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				R_LOG_REQ *pReq = (R_LOG_REQ*)buf1;
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				if(crc32 == *(uint32*)pCrc32)
				{
					LogUpload(pReq->logStart, pReq->logEnd, false);
				}
#endif
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_REBOOT_REQ:
		{
			DPRINTF("Receive leonis log request\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if (buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

#if 0
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(R_REBOOT_REQ) - sizeof(uint32));
				R_REBOOT_REQ *pReq = (R_REBOOT_REQ*)buf1;
				uint32* pCrc32 = &pReq->crc32;
#else
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				R_REBOOT_REQ *pReq = (R_REBOOT_REQ*)buf1;
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
#endif
				if(crc32 == *(uint32*)pCrc32)
				{
					if (memcmp(pReq->randomCode, m_authRep.randomCcode, 16) == 0)
					{
						char str[512];
						sprintf(str, "[NetCommThread] Received Reboot CMD from remote.");
						if (gLog)
						{
							gLog->Write(LOG_SYSTEM, str);
						}
						System sys;
						sys.Reboot();
					}
				}
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}

		}
		break;
	case NET_REMOTE_UPDATE_PUSH:
		{
			DPRINTF("Receive RemoteUpdate push\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if (buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				R_REMOTE_UPGRADE_REQ *pReq = (R_REMOTE_UPGRADE_REQ*)buf1;
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				m_strUpdateFile = "";
				if(crc32 == *(uint32*)pCrc32)
				{
					char *pos = (char*)&pReq->pDescription + pReq->descriptionLength;
					uint32 FID = *(uint32*)pos;
					pos += sizeof(uint32);
					uint32 FileNameLen = *(uint32*)pos;
					pos += sizeof(uint32);
					m_strUpdateFile = pos;
					m_strUpdateFile.resize(FileNameLen);

					UpdateRep(pReq->updateSerialNo);
				}
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_REMOTE_UPDATE_REQ:
		{
			DPRINTF("Receive RemoteUpdate req\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if (buf1)
			{
				tm = 1000;
				get_size = 0;
				if(m_authSocket.Receive((char*)buf1, m_inetDesc.payloadLength, get_size, &tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw (-1);
				}
				
				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if (m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				R_REMOTE_UPGRADE_PUSH_CONFIRM *pReq = (R_REMOTE_UPGRADE_PUSH_CONFIRM*)buf1;
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
				m_strUpdateFile = "";
				if(crc32 == *(char*)pCrc32)
				{
					//TODO: Update system
					DPRINTF("Update system\n");
				}
				if(bNew)
					delete[] buf1;
			}
			else
			{
				DPRINTF("new buf error\n");
			}
		}
		break;
	case NET_LOST_REQ:
		{
		    DPRINTF("Receive lost req\n");
		    char *buf1 = NULL;
		    bool bNew = false;
		    if(m_inetDesc.payloadLength < 2048)
			buf1 = buf;
		    else
		    {
			buf1 = new char[m_inetDesc.payloadLength];
			bNew = true;
		    }
		    if(buf1)
		    {
			get_size = 0;
			tm = 10000;
			if(m_authSocket.Receive((char*)buf1,
			    m_inetDesc.payloadLength,
			    get_size,
			    &tm) < 0)
			{
			    if(bNew)
				delete[] buf1;
				throw -1;
			}

			save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
				(char*)buf1, m_inetDesc.payloadLength);

			if(m_inetDesc.flag != ENC_NONE)
			{
			    aes_decrypt((uint8*)m_authRep.sessionKey,
			    (uint8*)buf1,
			    (uint8*)buf1,
			    m_inetDesc.payloadLength);
			}
			uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(struct Lose_Info_Req_Msg) - 4);
			char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
			if(crc32 == *(uint32*)pCrc32)
			{
			    struct Lose_Info_Req_Msg* pLostReq = (struct Lose_Info_Req_Msg*)buf;
			    if(gRecv.nFileID != pLostReq->filmID)
			    {
				DPRINTF("Send lost with id\n");
				pPat->SendLostReport(pLostReq->filmID);
			    }
			    else
			    {
				DPRINTF("Send lost without id\n");
				pPat->SendLostReport();
			    }
			}
			else
			{
				char str[512];
				sprintf(str, "[NetCommThread] Received Lost Report request CRC error.");
				if (gLog)
				{
					gLog->Write(LOG_SYSTEM, str);
				}
			}
		    }
		}
		break;
	case NET_LOST_CONFIRM:
		{
			DPRINTF("Receive lost report confirm\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if(buf1)
			{
				get_size = 0;
				tm = 10000;
				if(m_authSocket.Receive((char*)buf1,
					m_inetDesc.payloadLength,
					get_size,
					&tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw -1;
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if(m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
					(uint8*)buf1,
					(uint8*)buf1,
					m_inetDesc.payloadLength);
				}
#if 0
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(struct R_LOST_INFO_CONFIRM) - 4);
				R_LOST_INFO_CONFIRM* pLostInfo = (R_LOST_INFO_CONFIRM*)buf1;
				uint32* pCrc32 = &pLostInfo->crc32;
#else
				uint32 crc32 = calc_crc32((uint8*)buf1, m_inetDesc.payloadLength - sizeof(uint32));
				char* pCrc32 = buf1 + m_inetDesc.payloadLength - sizeof(uint32);
#endif

				gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) + 6;
				if(crc32 != *(uint32*)pCrc32)
				{
					char str[512];
					sprintf(str, "[NetCommThread] Received Lost Report confirm CRC error.");
					if (gLog)
					{
						gLog->Write(LOG_SYSTEM, str);
					}
				}
				else
				{
					//Update state to upload lost info confirm
					gRecv.nReceiveStatus = (gRecv.nReceiveStatus & 0xffff0000) | 0x06;
				}
				if (bNew)
					delete[] buf1;
			}
		}
		break;
	case NET_PKG_SEND_START:
		{
// 			DPRINTF("Receive PKG Send start\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if(buf1)
			{
				get_size = 0;
				tm = 10000;
				struct R_PKG_SEND_START *pSend_start;
				if(m_authSocket.Receive((char*)buf1,
					m_inetDesc.payloadLength,
					get_size,
					&tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw -1;
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if(m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

				pSend_start = (struct R_PKG_SEND_START *)buf1;
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(R_PKG_SEND_START) + pSend_start->filmNameLen);
				char* pCrc = buf1 + sizeof(R_PKG_SEND_START) + pSend_start->filmNameLen;
				if(*(uint32*)pCrc != crc32)
				{
					char str[512];
					sprintf(str, "[NetCommThread] PKG Send Start CRC ERROR.");
					DPRINTF("%s\n", str);
					if (gLog)
					{
						gLog->Write(LOG_SYSTEM, str);
					}
				}
				else
				{
					pSend_start = (struct R_PKG_SEND_START *)buf1;
					m_strPkgFileName = (char*)buf+sizeof(R_PKG_SEND_START);
					m_strPkgFileName.resize(pSend_start->filmNameLen);
					//DPRINTF("%s\n", m_strPkgFileName.c_str());
					//call confirm
					PkgSendConfirm(pSend_start->filmID);
				}
				if(bNew)
					delete[] buf1;
			}
		}
		break;
	case NET_PKG_DATA:
		{
// 			DPRINTF("Receive PKG Data\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if(buf1)
			{
				get_size = 0;
				tm = 10000;
				struct R_PKG_SEND_DATA *pSend_data;
				if(m_authSocket.Receive((char*)buf1,
					m_inetDesc.payloadLength,
					get_size,
					&tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw -1;
				}
				
				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if(m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}

				pSend_data = (struct R_PKG_SEND_DATA *)buf1;
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(R_PKG_SEND_DATA) + pSend_data->dataLen);
				char* pCrc = (buf1 + sizeof(R_PKG_SEND_DATA) + pSend_data->dataLen);
				if(*(uint32*)pCrc != crc32)
				{
					char str[512];
					sprintf(str, "[NetCommThread] PKG Data CRC ERROR.");
					DPRINTF("%s\n", str);
					if (gLog)
					{
						gLog->Write(LOG_SYSTEM, str);
					}
				}
				else
				{
					char *pData = (char*)(buf1 + sizeof(struct R_PKG_SEND_DATA));
					DPRINTF("segNum %d dataLen %d\n", pSend_data->segNum, pSend_data->dataLen);

					//call save data
					if(pPat)
					{
						std::string fn = m_strPkgFileName;
						fn.resize(m_strPkgFileName.size() - 3);
						pPat->SaveData((char*)fn.c_str(), pData, pSend_data->segNum, pSend_data->dataLen);
					}
				}
				if(bNew)
					delete[] buf1;
			}
		}
		break;
	case NET_PKG_SEND_END:
		{
 			DPRINTF("Receive PKG Send End\n");
			char *buf1 = NULL;
			bool bNew = false;
			if(m_inetDesc.payloadLength < 2048)
				buf1 = buf;
			else
			{
				buf1 = new char[m_inetDesc.payloadLength];
				bNew = true;
			}
			if(buf1)
			{
				get_size = 0;
				tm = 10000;
				if(m_authSocket.Receive((char*)buf1,
					m_inetDesc.payloadLength,
					get_size,
					&tm) < 0)
				{
					if(bNew)
						delete[] buf1;
					throw -1;
				}

				save_proto((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), m_inetDesc.command,
					(char*)buf1, m_inetDesc.payloadLength);

				if(m_inetDesc.flag != ENC_NONE)
				{
					aes_decrypt((uint8*)m_authRep.sessionKey,
						(uint8*)buf1,
						(uint8*)buf1,
						m_inetDesc.payloadLength);
				}
				uint32 crc32 = calc_crc32((uint8*)buf1, sizeof(uint32));
				char* pCrc = (buf1 + sizeof(uint32));
				if(*(uint32*)pCrc != crc32)
				{
					char str[512];
					sprintf(str, "[NetCommThread] PKG Send End CRC ERROR.");
					DPRINTF("%s\n", str);
					if (gLog)
					{
						gLog->Write(LOG_SYSTEM, str);
					}
				}
				else
				{
					bPkgSendStart = false;
					DPRINTF("filmId %08X\n", *(uint32*)buf1);
					//todo: call another destory jobs
				}
				if(bNew)
					delete[] buf1;
			}
		}
		break;
	default:
		{
			DPRINTF("Unknow message %04X\n", m_inetDesc.command);
#if 0
			m_inetDesc.subCommand = 0x02;
			m_inetDesc.payloadLength = 0;
			size_t send_size = 0;
			m_authSocket.Send((const char*)&m_inetDesc, sizeof(struct INET_DESCRIPTOR), send_size);
#endif
		}
	}
}

bool NetCommThread::LogUpload(uint32 nBegin, uint32 nEnd, bool bLeonis)
{
	std::string log;
	DPRINTF("Sendback log\n");

	LOGDATETIME timeAfter, timeBefore;
	if(bLeonis)
	{
		timeAfter.year = nBegin/10000;
		timeAfter.month = (nBegin%10000)/100;
		timeAfter.day = nBegin%100;

		timeBefore.year = nEnd/10000;
		timeBefore.month = (nEnd%10000)/100;
		timeBefore.day = nEnd%100;
	}
	else
	{
		time_t t;
#if 0
		t = nBegin;
		struct tm *pTm;
		pTm = localtime(&t);
		timeAfter.year = 1900 + pTm->tm_year;
		timeAfter.month = pTm->tm_mon + 1;
		timeAfter.day = pTm->tm_mday;

		t = nEnd;
		pTm = localtime(&t);
		timeBefore.year = 1900 + pTm->tm_year;
		timeBefore.month = pTm->tm_mon + 1;
		timeBefore.day = pTm->tm_mday;
#else
		time(&t);
		struct tm *pTm;
		pTm = localtime(&t);
		timeAfter.year = 1900 + pTm->tm_year;
		timeAfter.month = pTm->tm_mon + 1;
		timeAfter.day = pTm->tm_mday;
		timeBefore.year = 1900 + pTm->tm_year;
		timeBefore.month = pTm->tm_mon + 1;
		timeBefore.day = pTm->tm_mday;
#endif
	}

	extern std::string g_LogAll;
	ILog* pLog = CreateLog();
	if (pLog)
	{
		TLogQueryResultArray ra;
		if(pLog)
			pLog->Query(LOG_ALL, &timeAfter, &timeBefore, ra);
		log = g_LogAll;
		char *buf = new char[log.size() + 100];
		//char *buf = new char[nEnd - nBegin + 100];
		if (buf)
		{
			INET_DESCRIPTOR inet_desc;
			if (bLeonis)
				inet_desc.command = NET_LOG_REP_LEONIS;
			else
				inet_desc.command = NET_LOG_REP;
			inet_desc.subCommand = 0;
			inet_desc.flag = ENC_SESSIONKEY;

			L_LOG_REP* logRep = (L_LOG_REP*)buf;
			int pos = 0;
			if(log.size() > 100000)
			    logRep->fileSize = 100000;
			else
			logRep->fileSize = log.size();
			pos = sizeof(logRep->fileSize);
			logRep->logStart = nBegin;
			pos += sizeof(logRep->logStart);
			logRep->logEnd = nEnd;
			pos += sizeof(logRep->logEnd);
			logRep->uploadLength = log.size();
			pos += sizeof(logRep->uploadLength);

			//memcpy(buf + pos, log.c_str(), log.size());
			memcpy(buf + pos, log.c_str() + log.size() - logRep->fileSize, logRep->fileSize);
			//pos += log.size();
			pos += logRep->fileSize;

			uint32 crc = calc_crc32((uint8*)buf, pos);
			memcpy(buf + pos, &crc, sizeof(crc));
			pos += sizeof(uint32);
			char sss[50];
			sprintf(sss, "LOG CRC: %08X pos %08X\n", crc, pos);
			if(gLog)
				gLog->Write(LOG_NETCOMMU, sss);

			inet_desc.payloadLength = pos;
			if (inet_desc.flag != ENC_NONE)
			{
				aes_encrypt(m_authRep.sessionKey, (uint8*)buf, (uint8*)buf, pos);
				#if 0
				inet_desc.payloadLength = ((inet_desc.payloadLength + 15)/16)*16;
				#endif
			}
			
			size_t send_size = 0;
			t_timeout tm = 1000;

			save_proto((char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), inet_desc.command,
				(char*)buf, inet_desc.payloadLength);

			GetMutex();

			if(m_authSocket.Send((char*)&inet_desc,
				sizeof(struct INET_DESCRIPTOR),
				send_size, &tm) < 0)
			{
				ReleaseMutex();
				delete[] buf;
				throw -2;
			}

			send_size = 0;
			tm = 1000;
			if(m_authSocket.Send((char*)buf,
				inet_desc.payloadLength,
				send_size, &tm) < 0)
			{
				ReleaseMutex();
				delete[] buf;
				throw -3;
			}
			
			ReleaseMutex();
			delete[] buf;
		}
		ReleaseLog(pLog);
	}
	return true;
}

bool NetCommThread::PkgSendConfirm(uint32 filmId)
{
// 	DPRINTF("Pkg Send Confirm\n");
	INET_DESCRIPTOR inet_desc;
	inet_desc.command = NET_PKG_SEND_CONFIRM;
	inet_desc.payloadLength = ((sizeof(uint32)+15)/16)*16;
	char buf[128];
	{
		uint32 *pFilmId = (uint32*)buf;
		*pFilmId = filmId;
		uint32 *pCrc32 = (uint32*)(buf + sizeof(uint32));
		*pCrc32 = calc_crc32((uint8*)buf, sizeof(uint32));

		if (inet_desc.flag != ENC_NONE)
		{
			aes_encrypt(m_authRep.sessionKey, (uint8*)buf, (uint8*)buf, inet_desc.payloadLength);
		}

		size_t send_size = 0;
		t_timeout tm = 1000;
		
		save_proto((char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), inet_desc.command,
			(char*)buf, inet_desc.payloadLength);

		GetMutex();
		if(m_authSocket.Send((char*)&inet_desc,
			sizeof(struct INET_DESCRIPTOR),
			send_size, &tm) < 0)
		{
			ReleaseMutex();
			throw -2;
		}

		send_size = 0;
		tm = 1000;
		if(m_authSocket.Send((char*)buf,
			inet_desc.payloadLength,
			send_size, &tm) < 0)
		{
			ReleaseMutex();
			throw -3;
		}
		bPkgSendStart = true;
		ReleaseMutex();
	}
	return false;
}

bool NetCommThread::DecryptRep(uint32 filmId)
{
	char str[512];
	if(!bConnected)
	{
		sprintf(str, "[NetCommThread] DecryptRep: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	DPRINTF("DecryptRep\n");
	
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_DECRYPT_UPLOAD;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_MD5_RESULT_REPORT);

	bMd5Rep = false;

	struct L_MD5_RESULT_REPORT m_md5Report;
	//construct md5 request
	m_md5Report.filmID = filmId;
	
	//TODO: add some md5 decrypt result into struct
	//
	DPRINTF("FILMID %ld %ld\n", filmId, gRecv.nFileID);
	if(filmId != gRecv.nFileID)
	{
	    ICMyini *ini = createMyini();
	    if(ini)
	    {
		sprintf(str, "/tmp/film%ld.md5", filmId);
		if(ini->load(str))
		{
		    std::string tmp;
		    if(ini->read(" ", "SUCCESS", tmp))
		    {
			if(tmp == "true")
			    m_md5Report.md5Result = 0;
			else
			    m_md5Report.md5Result = 1;
		    }
		    if(ini->read(" ", "LENGTH", tmp))
		    {
			m_md5Report.rollBackLen = atoll(tmp.c_str());
		    }
		    else
		    {
			//do nothing
		    }
		}
		releaseMyini(ini);
	    }
	}
	else
	{
	    Md5Class *pMd5 = CreateMd5Class();
	    if(pMd5->bMd5Success)
		m_md5Report.md5Result = 0;
	    else
		m_md5Report.md5Result = 1;
	    m_md5Report.rollBackLen = pMd5->nRollBackLen;
	}
	//
	
	m_md5Report.crc32 = calc_crc32((uint8*)&m_md5Report,
		sizeof(L_MD5_RESULT_REPORT) - 4);
	
	//Encrypt md5 request with AES128 ECB
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&m_md5Report, 
		(uint8*)&m_md5Report,
		sizeof(struct L_MD5_RESULT_REPORT));

	//Send md5 request to remote
	size_t send_size = 0;

	t_timeout tm = 10000;

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&m_md5Report, sizeof(struct L_MD5_RESULT_REPORT));

	GetMutex();
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&m_md5Report,
		sizeof(struct L_MD5_RESULT_REPORT),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	ReleaseMutex();
	return true;
}

bool NetCommThread::DecryptRep()
{
	char str[512];
	if(!bConnected)
	{
		sprintf(str, "[NetCommThread] DecryptRep: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	DPRINTF("DecryptRep\n");

	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_DECRYPT_UPLOAD;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_MD5_RESULT_REPORT);

	bMd5Rep = false;

	struct L_MD5_RESULT_REPORT m_md5Report;
	//construct md5 request
	m_md5Report.filmID = gRecv.nFileID;

	//TODO: add some md5 decrypt result into struct
	//
	DPRINTF("FILMID %ld\n", gRecv.nFileID);
	Md5Class *pMd5 = CreateMd5Class();
	if(pMd5->bMd5Success)
		m_md5Report.md5Result = 0;
	else
		m_md5Report.md5Result = 1;
	m_md5Report.rollBackLen = pMd5->nRollBackLen;
	//

	m_md5Report.crc32 = calc_crc32((uint8*)&m_md5Report,
		sizeof(L_MD5_RESULT_REPORT) - 4);

	//Encrypt md5 request with AES128 ECB
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&m_md5Report, 
		(uint8*)&m_md5Report,
		sizeof(struct L_MD5_RESULT_REPORT));

	//Send md5 request to remote
	size_t send_size = 0;

	t_timeout tm = 10000;
	
	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&m_md5Report, sizeof(struct L_MD5_RESULT_REPORT));

	GetMutex();
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&m_md5Report,
		sizeof(struct L_MD5_RESULT_REPORT),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	ReleaseMutex();
	return true;
}


bool NetCommThread::UpdateRep(uint8* sn)
{
	if(!bConnected)
	{
		char str[512];
		sprintf(str, "[NetCommThread] UpdateRep: remote not connect.");
		if (gLog)
		{
			gLog->Write(LOG_NETCOMMU, str);
		}
		return false;
	}
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_REMOTE_UPDATA_PUSH_REP;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_REMOTE_UPGRADE_PUSH_REP);

	bool bSuccess = false;

	struct L_REMOTE_UPGRADE_PUSH_REP m_upRep;

	char str[512];
	//TODO: add some md5 decrypt result into struct
	if(m_strUpdateFile != "")
	{
		std::string fn = "/storage/" + m_strUpdateFile;
		DPRINTF("check update file %s\n", m_strUpdateFile.c_str());
		FILE *fp = fopen(m_strUpdateFile.c_str(), "rb");
		if (fp >0)
			bSuccess = true;
		else
		{
			sprintf(str, "[NetCommThread] Update Rep: check %s failed.", m_strUpdateFile.c_str());
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
		}
	}
	//

	if (bSuccess)
		m_upRep.updateCheckResult = 0;
	else
		m_upRep.updateCheckResult = 1;
	memcpy(m_upRep.updateSerialNo, sn, 16);
	m_upRep.reserved_1 = 0;

	ICMyini* ini = createMyini();
	bool bRead = false;
	if(ini)
	{
		if (ini->load("/etc/CineCast/Version"))
		{
			std::string tmp;
			if(ini->read("General", "Version", tmp))
			{
				memcpy(m_upRep.oldVersion, tmp.c_str(), tmp.size());
				bRead = true;
			}
		}
	}
	if (!bRead)
	{
		memcpy(m_loginReq.version, "0.0.0.0", 7);
		gLog->Write(LOG_NETCOMMU, "[NetCommThread] read version failed");
	}

	m_upRep.crc32 = calc_crc32((uint8*)&m_upRep,
		sizeof(L_REMOTE_UPGRADE_PUSH_REP) - 4);

	//Encrypt request with AES128 ECB
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&m_upRep, 
		(uint8*)&m_upRep,
		sizeof(struct L_REMOTE_UPGRADE_PUSH_REP));

	//Send request to remote
	size_t send_size = 0;

	t_timeout tm = 10000;

	save_proto((char*)&inet_descriptor, sizeof(struct INET_DESCRIPTOR), inet_descriptor.command,
		(char*)&m_upRep, sizeof(struct L_REMOTE_UPGRADE_PUSH_REP));

	GetMutex();
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&m_upRep,
		sizeof(struct L_REMOTE_UPGRADE_PUSH_REP),
		send_size,
		&tm) < 0)
	{
		ReleaseMutex();
		bConnected = false;
		m_authSocket.Destroy();
		if(m_status == NET_READ)
			m_status = NET_RUN;

		return false;
	}
	ReleaseMutex();
	return true;
}
