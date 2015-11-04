#include "NetCommThread.h"
#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

using namespace brunt;

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
		flag = 0;
	}
};

enum ENCRYPTION
{
	ENC_NONE = 0,
	ENC_HARDKEY,
	ENC_SESSIONKEY
};

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

int getTime()
{
	time_t t;
	time(&t);
	return t;
}

void aes_encrypt(uint8 *key, uint8 *in, uint8 *out, uint32 size)
{
	OpenSSL_add_all_ciphers();
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	int outlen, tmplen;
	EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);
	EVP_EncryptUpdate(&ctx, out, &outlen, in, size);
	EVP_EncryptFinal_ex(&ctx, out + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

void aes_decrypt(uint8* key, uint8* in, uint8* out, uint32 size)
{
	OpenSSL_add_all_ciphers();
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	int outlen, tmplen;
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);
	EVP_DecryptUpdate(&ctx, out, &outlen, in, size);
	EVP_DecryptFinal_ex(&ctx, out + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

NetCommThread::NetCommThread():m_mutex(0)
{
	m_pConfig = getConfig();
	string s = getDateTime();
	memset(m_loginReq.startupTime, 0, 20);
	strcpy((char*)m_loginReq.startupTime, s.c_str());
	m_loginReq.loginCount = 0;
	m_loginReq.machineID = m_pConfig->getMachineId();
}

NetCommThread::~NetCommThread()
{
	Stop();
}

bool NetCommThread::Init()
{
	return true;
}

bool NetCommThread::Start()
{
	Stop();

	m_status = NET_RUN;
	return true;
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
	while(m_mutex);
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
	inet_descriptor.flag = ENC_HARDKEY;
	inet_descriptor.command = NET_LOGIN_REQ;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_LOGIN_REQ);

	//construct login request
	memset(m_loginReq.loginTime, 0, 20);
	strcpy((char*)m_loginReq.loginTime, getDateTime().c_str());
	m_loginReq.loginCount++;
	m_loginReq.crc32 = calc_crc32((uint8*)&m_loginReq,
		sizeof(L_LOGIN_REQ) - 4);

	//Encrypt login request with AES128 ECB
	struct L_LOGIN_REQ req;
	aes_encrypt((uint8*)m_hardKey, 
		(uint8*)&m_loginReq, 
		(uint8*)&req,
		(uint32)sizeof(struct L_LOGIN_REQ));

	//Send login request to remote
	size_t send_size = 0;
	if(m_loginSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size) < 0)
		return false;
	send_size = 0;
	if(m_loginSocket.Send((char*)&req,
		sizeof(struct L_LOGIN_REQ),
		send_size) < 0)
		return false;

	//Read login response
	size_t get_size = 0;
	if(m_loginSocket.Receive((char*)&inet_descriptor,
		(int)sizeof(struct INET_DESCRIPTOR), 
		get_size) < 0)
		return false;
	get_size = 0;
	if(m_loginSocket.Receive((char*)&m_loginRep,
		sizeof(struct R_LOGIN_REP),
		get_size) < 0)
		return false;

	if(inet_descriptor.subCommand != 0)
		return false;

	//decrypt login response
	if(inet_descriptor.flag == ENC_HARDKEY)
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)&m_loginRep,
		(uint8*)&m_loginRep,
		sizeof(struct R_LOGIN_REP));

	uint32 crc32 = calc_crc32((uint8*)&m_loginRep,
		sizeof(struct R_LOGIN_REP) - 4);
	if(crc32 == m_loginRep.crc32)
		return true;
	return false;
}

bool NetCommThread::Auth()
{
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_HARDKEY;
	inet_descriptor.command = NET_AUTH_REQ;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_AUTH_REQ);

	struct L_AUTH_REQ m_authReq;
	//construct auth request
	m_authReq.machineID = m_pConfig->getMachineId();
	memset(m_authReq.version, 0, 16);
	memcpy(m_authReq.version, "1.0.0.0", 7);
	memset(m_authReq.loginTime, 0, 20);
	strcpy((char*)m_authReq.loginTime, getDateTime().c_str());
	m_authReq.logMode = 2; //do not upload log file
	m_authReq.heartCycle = 10; //10 seconds
	m_authReq.connectMode = 1;
	m_authReq.diskNum = 1;
	m_authReq.diskCapacity = 500*1000*1000*1000;
	m_authReq.crc32 = calc_crc32((uint8*)&m_authReq,
		sizeof(L_AUTH_REQ) - 4);

	//Encrypt login request with AES128 ECB
	aes_encrypt((uint8*)m_hardKey, 
		(uint8*)&m_authReq, 
		(uint8*)&m_authReq,
		sizeof(struct L_AUTH_REQ));

	//Send login request to remote
	size_t send_size = 0;

	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size) < 0)
		return false;
	send_size = 0;
	if(m_authSocket.Send((char*)&m_authReq,
		sizeof(struct L_AUTH_REQ),
		send_size) < 0)
		return false;

	//Read auth response
	size_t get_size = 0;
	if(m_authSocket.Receive((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR), 
		get_size) < 0)
		return false;
	get_size = 0;
	if(m_authSocket.Receive((char*)&m_authRep,
		sizeof(struct R_AUTH_REP),
		get_size) < 0)
		return false;

	//decrypt auth response
	if(inet_descriptor.flag == ENC_HARDKEY)
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)&m_authRep,
		(uint8*)&m_authRep,
		sizeof(struct R_AUTH_REP));

	uint32 crc32 = calc_crc32((uint8*)&m_authRep,
		sizeof(struct R_AUTH_REP) - 4);
	if(crc32 == m_authRep.crc32)
		return true;
	return false;
}

bool NetCommThread::GetMD5File(uint32 filmId)
{
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

	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size) < 0)
		return false;
	send_size = 0;
	if(m_authSocket.Send((char*)&m_md5Req,
		sizeof(struct L_MD5_KEY_REQ),
		send_size) < 0)
		return false;

	int t1 = getTime();
	while(!bMd5Rep)
	{
		if(getTime() > (t1 + 10))
			return false;
	}
	return true;
}

bool NetCommThread::ReportLost(struct L_LOST_INFO& lost)
{
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_MD5_REQ;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = sizeof(struct L_LOST_INFO) - 4 + lost.lostLength;

	uint8 *buf = new uint8[inet_descriptor.payloadLength];
	memcpy(buf, &lost, sizeof(L_LOST_INFO) - 8);
	memcpy(buf+sizeof(L_LOST_INFO) - 8, lost.pLost, lost.lostLength);

	lost.crc32 = calc_crc32(buf, (int32)inet_descriptor.payloadLength - 4);
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&buf, 
		(uint8*)&buf,
		inet_descriptor.payloadLength);

	size_t send_size = 0;

	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size) < 0)
		return false;
	send_size = 0;
	if(m_authSocket.Send((char*)&buf,
		inet_descriptor.payloadLength,
		send_size) < 0)
		return false;

	size_t get_size = 0;
	if(m_authSocket.Receive((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR), 
		get_size) < 0)
		return false;
	
	get_size = 0;
	struct R_LOST_INFO_CONFIRM rlc;
	if(m_authSocket.Receive((char*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM),
		get_size) < 0)
		return false;

	//decrypt auth response
	if(inet_descriptor.flag == ENC_HARDKEY)
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)&rlc,
		(uint8*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM));
	else
		aes_decrypt((uint8*)m_authRep.sessionKey,
		(uint8*)&rlc,
		(uint8*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM));

	uint32 crc32 = calc_crc32((uint8*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM) - 4);
	if(crc32 == rlc.crc32)
		return true;
	return false;
}

void NetCommThread::doit()
{
#if 0
	while(1)
	{
		switch(m_status)
		{
		case NET_RUN:
			{
				string url = m_Config.getRemoteUrl();
				int port = m_Config.getRemotePort();
				try
				{
					if(m_loginSocket.Create(AF_INET, SOCK_STREAM, 0))
					{
						struct sockaddr_in addr_in;
						memset(&addr_in, 0, sizeof(sockaddr_in));
						addr_in.sin_family = AF_INET;
						addr_in.sin_port = htons(port);
						addr_in.sin_addr.s_addr = inet_addr(url.c_str());
						int error = 0;
						do
						{
							t_timeout tm = 3000;
							error = m_loginSocket.Connect(&addr_in, &tm);
							if (error && error != CZSocket::WAIT_R_TIMEOUT)
							{
								m_loginSocket.Destroy();
								throw -1;
							}
						}while(error);
						m_status = NET_LOGIN;
					}
				}
				catch(int&)
				{
					DPRINTF("Socket error\n");
				}
			}
			break;
		case NET_LOGIN:
			if(Login() == false)
			{
				m_loginSocket.Destroy();
				m_status = NET_RUN;
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
					struct sockaddr_in addr_in;
					memset(&addr_in, 0, sizeof(sockaddr_in));
					addr_in.sin_family = AF_INET;
					addr_in.sin_port = htons(m_loginRep.port);
					addr_in.sin_addr.s_addr = inet_addr(m_loginRep.serverIP);
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
						m_authSocket.Destroy();
						m_status = NET_RUN;
					}
					else
					{
						m_status = NET_READ;
					}
				}
			}
			catch(int&)
			{
				m_status = NET_RUN;
				DPRINTF("Socket error\n");
			}
			break;
		case NET_READ:
			{
				struct INET_DESCRIPTOR m_inetDesc;
				int get_size = 0;
				if(m_authSocket.Receive(&m_inetDesc, sizeof(INET_DESCRIPTOR), get_size, 1000) < 0)
				{
					m_authSocket.Destroy();
					m_status = NET_RUN;
				}
				switch(m_inetDesc.command)
				{
				case NET_MD5_REP:
					if (m_authSocket.Receive(&m_md5Rep, 12, get_size) < 0)
					{
						m_authSocket.Destroy();
						m_status = NET_RUN;
					}
					else
					{
						m_md5Rep.fileContent = new uint8[m_md5Rep.fileLength];
						m_authSocket.Receive(m_md5Rep.fileContent, m_md5Rep.fileLength + 4, get_size);
						bMd5Rep = true;
						struct L_MD5_KEY_CONFIRM confirm;
						struct INET_DESCRIPTOR inet_desc;
						inet_desc.command = NET_MD5_CONFIRM;
						inet_desc.payloadLength = sizeof(L_MD5_KEY_CONFIRM);
						confirm.filmID = m_md5Rep.filmID;
						confirm.crc32 = calc_crc32(&confirm,
							sizeof(L_MD5_KEY_CONFIRM) - 4);
						int send_size = 0;
						m_authSocket.Send((const char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), send_size);
						m_authSocket.Send((const char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM), send_size);
					}
					break;
				}
			}
			break;
		case NET_REBOOT_REQ:
			//TODO:
			//	reboot system
			break;
		default:
			break;
		}
	}
#endif
}