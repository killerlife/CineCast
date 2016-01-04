#include "NetCommThread.h"
#include "message.h"
#include "ini.h"
#include "BaseOperation.h"

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

using namespace brunt;
extern ILog* gLog;
extern TUNER_INFO gInfo;
extern RECEIVE_INFO gRecv;
extern TUNER_CONF gTuner;

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
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	EVP_CIPHER_CTX ctx;
	const EVP_CIPHER *cipher;
	EVP_CIPHER_CTX_init(&ctx);
	cipher = EVP_aes_128_ecb();
	EVP_EncryptInit_ex(&ctx, cipher, NULL, key, NULL);

	int outlen = 0, tmplen;
	EVP_EncryptUpdate(&ctx, out+outlen, &outlen, in, size);
	EVP_EncryptFinal_ex(&ctx, out + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

void aes_decrypt(uint8* key, uint8* in, uint8* out, uint32 size)
{
	OpenSSL_add_all_ciphers();
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	int outlen, tmplen;
	outlen = 0;
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);
	EVP_DecryptUpdate(&ctx, out + outlen, &outlen, in, size);
	EVP_DecryptFinal_ex(&ctx, out + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
}

NetCommThread::NetCommThread():m_mutex(0), bConnected(false)
{
	string s = getDateTime();
	memset(m_loginReq.startupTime, 0, 20);
	strcpy((char*)m_loginReq.startupTime, s.c_str());
	m_loginReq.loginCount = 0;
}

NetCommThread::~NetCommThread()
{
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

bool NetCommThread::Init()
{
	return true;
}

bool NetCommThread::Start()
{
	char str[512];

	if(!(status() == thread_stopped || status() == thread_ready))
	{
		sprintf(str, "[TmsThread] The thread had not been ready, status = %d.", status());
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
		sprintf(str, "[TmsThread] Start thread failed, status = %d.", status());
		DPRINTF("%s\n", str);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
		return false; 
	}

	m_status = NET_RUN;
	return true;
}

void NetCommThread::cleanThread()
{
	m_status = STOP;
	CActiveThread::stop();

	m_loginSocket.Destroy();
	m_authSocket.Destroy();
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
				if(gLog)
					gLog->Write(LOG_NETCOMMU, tmp.c_str());
				bRead = true;
			}
		}
	}
	if (!bRead)
	{
		memcpy(m_loginReq.version, "0.0.0.0", 7);
		gLog->Write(LOG_NETCOMMU, "[NetCommThread] read version failed");
	}

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
	t_timeout tm = 10000;
	if(m_loginSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
		return false;

	send_size = 0;
	tm = 10000;
	if(m_loginSocket.Send((char*)&req,
		sizeof(struct L_LOGIN_REQ),
		send_size,
		&tm) < 0)
		return false;

	//Read login response
	size_t get_size = 0;
	tm = 10000;
	if(m_loginSocket.Receive((char*)&inet_descriptor,
		(int)sizeof(struct INET_DESCRIPTOR), 
		get_size,
		&tm) < 0)
		return false;
	get_size = 0;
	tm = 10000;
	if(m_loginSocket.Receive((char*)&m_loginRep,
		sizeof(struct R_LOGIN_REP),
		get_size,
		&tm) < 0)
		return false;

	char str[512];
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
		return true;
	sprintf(str, "[NetCommThread] Login receive crc error");
	DPRINTF("%s\n", str);
	gLog->Write(LOG_ERROR, str);
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
	m_authReq.heartCycle = 10; //10 seconds
	m_authReq.connectMode = 1; //always connect.
	m_authReq.diskNum = 1;

	ContentOperation co;
	m_authReq.diskCapacity = co.GetTotalSpace(0);
	m_authReq.reserved_1 = m_authReq.reserved_2 = 0;
	m_authReq.crc32 = calc_crc32((uint8*)&m_authReq,
		sizeof(L_AUTH_REQ) - 4);

	int leng = sizeof(m_authReq);
	uint8 *buf = new uint8[leng+100];
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

	//Encrypt login request with AES128 ECB
	if(inet_descriptor.flag == ENC_HARDKEY)
	{
	aes_encrypt((uint8*)m_hardKey, 
			(uint8*)buf+sizeof(uint32), 
			(uint8*)buf+sizeof(uint32),
			sizeof(struct L_AUTH_REQ)-sizeof(uint32));
		inet_descriptor.payloadLength = ((sizeof(struct L_AUTH_REQ)-sizeof(uint32)+15)/16)*16 + sizeof(uint32);
	}

	//Send login request to remote
	size_t send_size = 0;

	t_timeout tm = 10000;
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
	{
		delete[] buf;
		return false;
	}
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)buf,
		inet_descriptor.payloadLength,
		send_size, &tm) < 0)
	{
		delete[] buf;
		return false;
	}

	//Read auth response
	size_t get_size = 0;
	tm = 10000;
	if(m_authSocket.Receive((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR), 
		get_size,
		&tm) < 0)
	{
		delete[] buf;
		return false;
	}
	get_size = 0;
	tm = 10000;
	if(m_authSocket.Receive((char*)buf,
		inet_descriptor.payloadLength,
		get_size,
		&tm) < 0)
	{
		delete[] buf;
		return false;
	}

	//decrypt auth response
	if(inet_descriptor.flag == ENC_HARDKEY)
		aes_decrypt((uint8*)m_hardKey,
		(uint8*)&buf,
		(uint8*)&buf,
		inet_descriptor.payloadLength);
	memcpy(&m_authRep, buf, sizeof(m_authRep));

	uint32 crc32 = calc_crc32((uint8*)&m_authRep,
		sizeof(struct R_AUTH_REP) - 4);
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
		return true;
	}
	if (gLog)
		gLog->Write(LOG_ERROR, "[NetCommThread] Auth crc error");
	delete[] buf;
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

	t_timeout tm = 10000;
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
		return false;
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&m_md5Req,
		sizeof(struct L_MD5_KEY_REQ),
		send_size,
		&tm) < 0)
		return false;

	int t1 = getTime();
	while(!bMd5Rep)
	{
		if(getTime() > (t1 + 10))
			return false;
	}
	return true;
}

bool NetCommThread::ReportLost(char* buf, int nSize)
{
	if(!bConnected)
		return false;
	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_LOST_UPLOAD;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = nSize;

	uint32 crc32 = calc_crc32((uint8*)buf, nSize - sizeof(uint32));
	memcpy(buf + nSize - sizeof(uint32), &crc32, sizeof(uint32));

	if (inet_descriptor.flag != ENC_NONE)
	{
	aes_encrypt((uint8*)m_authRep.sessionKey, 
		(uint8*)&buf, 
		(uint8*)&buf,
			nSize);
		inet_descriptor.payloadLength = ((nSize + 15)/16)*16;
	}

	size_t send_size = 0;
	t_timeout tm = 10000;
	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size,
		&tm) < 0)
		return false;
	send_size = 0;
	tm = 10000;
	if(m_authSocket.Send((char*)&buf,
		inet_descriptor.payloadLength,
		send_size,
		&tm) < 0)
		return false;

	size_t get_size = 0;
	tm = 10000;
	if(m_authSocket.Receive((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR), 
		get_size,
		&tm) < 0)
		return false;
	
	get_size = 0;
	tm = 10000;
	struct R_LOST_INFO_CONFIRM rlc;
	if(m_authSocket.Receive((char*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM),
		get_size,
		&tm) < 0)
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

	crc32 = calc_crc32((uint8*)&rlc,
		sizeof(struct R_LOST_INFO_CONFIRM) - 4);
	if(crc32 == rlc.crc32)
		return true;
	return false;
}

bool NetCommThread::HeartBreat()
{
	if(!bConnected)
		return false;

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
	m_sHeart.dataRate = 80;
	m_sHeart.reserved;
	m_sHeart.filmID = gRecv.nFileID;
	m_sHeart.filmNameLength = gRecv.strFilmName.size();
	m_sHeart.filmLength = gRecv.nFileLength;
	m_sHeart.filmSegment = gRecv.nTotalSegment;
	m_sHeart.recvRound = gRecv.nReceiveStatus >> 8;
	memcpy(m_sHeart.taskStartTime, m_strTaskTime.c_str(), m_strTaskTime.size());
	memcpy(m_sHeart.recvStartTime, m_strRoundTime.c_str(), m_strRoundTime.size());
	m_sHeart.filmRecvState = gRecv.nReceiveStatus & 0xff;
	m_sHeart.reserved2 = 0;
	m_sHeart.recvLength = gRecv.nReceiveLength;
	m_sHeart.lostSegment = gRecv.nLostSegment;

	int leng = sizeof(m_sHeart) - sizeof(uint8*) + m_sHeart.filmNameLength;
	uint8* buf = new uint8[leng+100];

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
	memcpy(pos, &m_sHeart.machineID, sizeof(m_sHeart.machineID));
	pos += sizeof(m_sHeart.machineID);

	memcpy(pos, &m_sHeart.temperature, sizeof(m_sHeart.temperature));
	pos += sizeof(m_sHeart.temperature);

	memcpy(pos, &m_sHeart.CPUUsage, sizeof(m_sHeart.CPUUsage));
	pos += sizeof(m_sHeart.CPUUsage);

	memcpy(pos, &m_sHeart.memTotal, sizeof(m_sHeart.memTotal));
	pos += sizeof(m_sHeart.memTotal);

	memcpy(pos, &m_sHeart.memIdle, sizeof(m_sHeart.memIdle));
	pos += sizeof(m_sHeart.memIdle);

	memcpy(pos, &m_sHeart.networkRate, sizeof(m_sHeart.networkRate));
	pos += sizeof(m_sHeart.networkRate);

	memcpy(pos, &m_sHeart.agc, sizeof(m_sHeart.agc));
	pos += sizeof(m_sHeart.agc);

	memcpy(pos, &m_sHeart.snr, sizeof(m_sHeart.snr));
	pos += sizeof(m_sHeart.snr);

	memcpy(pos, &m_sHeart.interfaceRate, sizeof(m_sHeart.interfaceRate));
	pos += sizeof(m_sHeart.interfaceRate);

	memcpy(pos, &m_sHeart.dataRate, sizeof(m_sHeart.dataRate));
	pos += sizeof(m_sHeart.dataRate);

	memcpy(pos, &m_sHeart.reserved, 16);
	pos += 16;

	memcpy(pos, &m_sHeart.filmID, sizeof(m_sHeart.filmID));
	pos += sizeof(m_sHeart.filmID);

	memcpy(pos, &m_sHeart.filmNameLength, sizeof(m_sHeart.filmNameLength));
	pos += sizeof(m_sHeart.filmNameLength);

	if(m_sHeart.filmNameLength > 0)
	{
		memcpy(pos, gRecv.strFilmName.c_str(), m_sHeart.filmNameLength);
		pos += sizeof(m_sHeart.filmNameLength);
	}

	memcpy(pos, &m_sHeart.filmLength, sizeof(m_sHeart.filmLength));
	pos += sizeof(m_sHeart.filmLength);

	memcpy(pos, &m_sHeart.filmSegment, sizeof(m_sHeart.filmSegment));
	pos += sizeof(m_sHeart.filmSegment);

	memcpy(pos, &m_sHeart.recvRound, sizeof(m_sHeart.recvRound));
	pos += sizeof(m_sHeart.recvRound);

	memcpy(pos, &m_sHeart.taskStartTime, 20);
	pos += 20;

	memcpy(pos, &m_sHeart.recvStartTime, 20);
	pos += 20;

	memcpy(pos, &m_sHeart.filmRecvState, sizeof(m_sHeart.filmRecvState));
	pos += sizeof(m_sHeart.filmRecvState);

	memcpy(pos, &m_sHeart.reserved2, sizeof(m_sHeart.reserved2));
	pos += sizeof(m_sHeart.reserved2);

	memcpy(pos, &m_sHeart.recvLength, sizeof(m_sHeart.recvLength));
	pos += sizeof(m_sHeart.recvLength);

	memcpy(pos, &m_sHeart.lostSegment, sizeof(m_sHeart.lostSegment));
	pos += sizeof(m_sHeart.lostSegment);

	uint32 crc = calc_crc32(buf, (pos - buf));
	memcpy(pos, &crc, sizeof(crc));
	pos +=sizeof(crc);

	//construct preamble
	struct INET_DESCRIPTOR inet_descriptor;
// 	inet_descriptor.flag = ENC_NONE;
	inet_descriptor.flag = ENC_SESSIONKEY;
	inet_descriptor.command = NET_HEARTBEAT_UPLOAD;
	inet_descriptor.subCommand = 0x00;
	inet_descriptor.payloadLength = pos - buf;

	//Encrypt request with AES128 ECB
	if(inet_descriptor.flag != ENC_NONE)
	{
		aes_encrypt((uint8*)m_authRep.sessionKey, 
			(uint8*)buf, 
			(uint8*)buf,
			inet_descriptor.payloadLength);
		inet_descriptor.payloadLength = ((inet_descriptor.payloadLength+15)/16)*16;
	}

	//Send md5 request to remote
	size_t send_size = 0;

	if(m_authSocket.Send((char*)&inet_descriptor,
		sizeof(struct INET_DESCRIPTOR),
		send_size) < 0)
	{
		delete[] buf;
		return false;
	}

	send_size = 0;
	if(m_authSocket.Send((char*)buf,
		inet_descriptor.payloadLength,
		send_size) < 0)
	{
		delete[] buf;
		return false;
	}

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
#if 1
	char str[512];

	while(1)
	{
		switch(m_status)
		{
		case NET_RUN:
			{
				ICMyini* ini = createMyini();
				std::string tmp;

				std::string url;
				int port;
				if(ini)
				{
					if (ini->load("/etc/CineCast/CineCast.cfg"))
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
					sleep(5);
					sprintf(str, "[NetCommThread] Connect to login server %s:%d error.", url.c_str(), port);
					if(gLog)
						gLog->Write(LOG_ERROR, str);
					DPRINTF("%s\n", str);
				}
			}
			break;
		case NET_LOGIN:
			if(Login() == false)
			{
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
						bConnected = true;
						m_status = NET_READ;
					}
				}
			}
			catch(int&)
			{
				m_status = NET_RUN;
				sleep(1);
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
					bConnected = false;
					m_authSocket.Destroy();
					m_status = NET_RUN;
				}
#if 0
				struct INET_DESCRIPTOR m_inetDesc;
				size_t get_size = 0;

				t_timeout tm = 1000;

				if(m_authSocket.Receive((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), get_size, &tm) < 0)
				{
					bConnected = false;
					m_authSocket.Destroy();
					m_status = NET_RUN;
				}
				switch(m_inetDesc.command)
				{
				case NET_MD5_REP:
					if (m_authSocket.Receive((char*)&m_md5Rep, 12, get_size) < 0)
					{
						m_authSocket.Destroy();
						m_status = NET_RUN;
					}
					else
					{
						m_md5Rep.fileContent = new uint8[m_md5Rep.fileLength];
						m_authSocket.Receive((char*)m_md5Rep.fileContent, m_md5Rep.fileLength + 4, get_size);
						bMd5Rep = true;
						struct L_MD5_KEY_CONFIRM confirm;
						struct INET_DESCRIPTOR inet_desc;
						inet_desc.command = NET_MD5_CONFIRM;
						inet_desc.payloadLength = sizeof(L_MD5_KEY_CONFIRM);
						confirm.filmID = m_md5Rep.filmID;
						confirm.crc32 = calc_crc32((uint8 *)&confirm,
							sizeof(L_MD5_KEY_CONFIRM) - 4);
						size_t send_size = 0;
						m_authSocket.Send((const char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), send_size);
						m_authSocket.Send((const char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM), send_size);
					}
					break;
				}
#endif
			}
			break;
		case NET_REBOOT_REQ:
			//TODO:
			//	reboot system
			break;
		case NET_STOP:
			return;
		default:
			break;
		}
	}
#endif
}
bool NetCommThread::RecvFilter()
{
	struct INET_DESCRIPTOR m_inetDesc;
	size_t get_size = 0;

	t_timeout tm = 1000;

	if(m_authSocket.Receive((char*)&m_inetDesc, sizeof(INET_DESCRIPTOR), get_size, &tm) < 0)
	{
		throw (-1);
	}
	switch(m_inetDesc.command)
	{
	case NET_HEARTBEAT_REQ:
		DPRINTF("Receive heartbreat request\n");
		HeartBreat();
		break;
	case NET_HEARTBEAT_CONFIRM:
		DPRINTF("Receive heartbreat confirm\n");
		break;
	case NET_MD5_REP:
		if (m_authSocket.Receive((char*)&m_md5Rep, 12, get_size) < 0)
		{
			m_authSocket.Destroy();
			m_status = NET_RUN;
		}
		else
		{
			m_md5Rep.fileContent = new uint8[m_md5Rep.fileLength];
			t_timeout tm = 1000;
			m_authSocket.Receive((char*)m_md5Rep.fileContent, m_md5Rep.fileLength + 4, get_size);
			bMd5Rep = true;
			struct L_MD5_KEY_CONFIRM confirm;
			struct INET_DESCRIPTOR inet_desc;
			inet_desc.command = NET_MD5_CONFIRM;
			inet_desc.payloadLength = sizeof(L_MD5_KEY_CONFIRM);
			confirm.filmID = m_md5Rep.filmID;
			confirm.crc32 = calc_crc32((uint8 *)&confirm,
				sizeof(L_MD5_KEY_CONFIRM) - 4);
			size_t send_size = 0;
			m_authSocket.Send((const char*)&inet_desc, sizeof(struct INET_DESCRIPTOR), send_size);
			m_authSocket.Send((const char*)&confirm, sizeof(struct L_MD5_KEY_CONFIRM), send_size);
		}
		break;
	}
}
