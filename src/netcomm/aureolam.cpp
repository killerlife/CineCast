//////////////////////////////////////////////////////////////////////////
// 修改记录
// --------
//
// 修改OPENSSL BASE64编码后加入了回车分行，造成光环解码出错 [8/1/2018 jaontolt]
// 光环参数中少了一个data=，补上 [8/1/2018 jaontolt]
// ！！注意！！光环的HTTP URL必须为小写，不能有大写 [8/1/2018 jaontolt]
// 教训！生产安装售后修改配置文件时，配置中的光环URL必须小写 [8/1/2018 jaontolt]
// 增加光环传媒管理中心通讯协议支持，使用独立线程实现 [7/31/2018 jaontolt]
//////////////////////////////////////////////////////////////////////////


#ifdef AUREOLAM
#include "aureolam.h"
#include "message.h"
#include "ini.h"
#include "BaseOperation.h"
#include "../dvb/PATDataProcess.h"

#include <unistd.h>
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
extern bool bRemoteConnect;

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

extern char gVersion[16];
extern uint32 gMachineId;

extern string getDateTime();
extern void print_hex(char* buf, int len);
extern int getTime();
extern unsigned long get_tick();

AureoLamThread::AureoLamThread()
{
	nRecvLength = 0;
	bExit = false;
	m_logCount = 0;
}

AureoLamThread::~AureoLamThread()
{
	Stop();
	cleanThread();
}

bool AureoLamThread::Init()
{
	ICMyini* ini = createMyini();
	std::string tmp, url;
	if(ini)
	{
		if(ini->load("/etc/CineCast/CineCast.cfg"))
		{
			if(ini->read(" ", "AUREOLAM-URL", url))
			{
				m_strAureoLamNS = url;
			}
			else
			{
				url = "stmp.aureolamedia.com/heartbeat";
				if(gLog)
					gLog->Write(LOG_NETCOMMU, "[AureoLamThread] read AureoLam URL error.");
			}
		}
		else
			url = "stmp.aureolamedia.com/heartbeat";
		if (ini->load("/etc/CineCast/ID"))
		{
			if (ini->read("ID_HardKey", "ID", tmp))
			{
				gMachineId = atoi(tmp.c_str());
			}
			else
			{
				if(gLog)
					gLog->Write(LOG_NETCOMMU, "[AureoLamThread] read MachineId failed");
				gMachineId = 0;
			}
		}
	}
	else
		url = "stmp.aureolamedia.com/heartbeat";

	m_strAureoLamNS = url;


	return true;
}

bool AureoLamThread::Start()
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

bool AureoLamThread::Stop()
{
	bExit = true;
}

#include "../externcall/ExternCall.h"

bool AureoLamThread::HeartBreat()
{
	DPRINTF("[AureoLam] Heartbreat\n");

	L_HEART_INFO_REPORT m_sHeart;
	m_sHeart.machineID = gMachineId;
	m_sHeart.temperature = 30;
	m_sHeart.CPUUsage = 10;
	m_sHeart.memTotal = (uint64)(4096L*1024L*1024L); //For DMCC Requirement, Change GigaBytes to Bytes
	m_sHeart.memIdle = (uint64)(1024L*1024L*1024L);	//For DMCC Requirement, Change GigaBytes to Bytes
	m_sHeart.networkRate = 2;
	m_sHeart.agc = gInfo.nAGC;
	m_sHeart.snr = gInfo.nSNR;
	m_sHeart.interfaceRate = gTuner.nSR/1000;
	
        m_sHeart.dataRate = (gRecv.nReceiveLength - nRecvLength)*8/1000/1000/120;
	nRecvLength = gRecv.nReceiveLength;
	
	if(m_sHeart.dataRate < 80)
	m_sHeart.dataRate = 80;
    
	m_sHeart.reserved;

	//////////////////////////////////////////////////////////////////////////
	// For AureoLam Special Requirement [7/30/2018 jaontolt]
	strncpy((char*)m_sHeart.reserved, gVersion, 16);
	//////////////////////////////////////////////////////////////////////////

	m_sHeart.filmID = gRecv.nFileID;
	m_sHeart.filmNameLength = gRecv.strFilmName.size();
	m_sHeart.filmLength = gRecv.nFileLength;
	m_sHeart.filmSegment = gRecv.nTotalSegment;
	m_sHeart.recvRound = gRecv.nReceiveStatus >> 16;

	//////////////////////////////////////////////////////////////////////////
	//  [5/28/2018 jaontolt]
	//Make sure the value is 0, while waiting the task start
	if(m_strTaskTime.empty())
		memset(m_sHeart.taskStartTime, 0, 20);
	else
		memcpy(m_sHeart.taskStartTime, m_strTaskTime.c_str(), m_strTaskTime.size());
	if(m_strRoundTime.empty())
		memset(m_sHeart.recvStartTime, 0, 20);
	else
		memcpy(m_sHeart.recvStartTime, m_strRoundTime.c_str(), m_strRoundTime.size());
	//////////////////////////////////////////////////////////////////////////
	DPRINTF("Task Time: %s\n", m_sHeart.taskStartTime);
	DPRINTF("Round Time: %s\n", m_sHeart.recvStartTime);

	m_sHeart.filmRecvState = gRecv.nReceiveStatus & 0xffff;
	m_sHeart.reserved2 = 0;
	m_sHeart.recvLength = gRecv.nReceiveLength;
	
	m_sHeart.lostSegment = gRecv.nLostSegment;

	int leng = sizeof(L_HEART_INFO_REPORT) - sizeof(uint8*) + m_sHeart.filmNameLength;
	uint8* buf = new uint8[leng+100];
	memset(buf, 0, leng+100);


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

	//////////////////////////////////////////////////////////////////////////
	//Fix DMCC error, 2017-5-10
	if(m_sHeart.filmNameLength > 0)
		m_sHeart.filmNameLength += 1;
	//////////////////////////////////////////////////////////////////////////
	memcpy(pos, &m_sHeart.filmNameLength, sizeof(uint32));
	pos += sizeof(uint32);

	if(gRecv.strFilmName.size() > 0)
	{
		memcpy(pos, gRecv.strFilmName.c_str(), gRecv.strFilmName.size());
		pos += m_sHeart.filmNameLength;

		//////////////////////////////////////////////////////////////////////////
		//Fix DMCC error, 2017-5-9
		memset((pos - 1), 0, 1);
		//////////////////////////////////////////////////////////////////////////
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

	size_t send_size = pos - buf;

	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();
	ERR_load_crypto_strings();

	unsigned char out[6000] = {0};
	int len, elen;

	EVP_ENCODE_CTX dctx;
	EVP_EncodeInit(&dctx);
	EVP_EncodeUpdate(&dctx, out, &len, (unsigned char*) buf, send_size);
	elen = len;
	EVP_EncodeFinal(&dctx, out+len, &len);
	elen += len;

	//////////////////////////////////////////////////////////////////////////
	// 修正BASE64编码后补0x0a的问题 [8/1/2018 jaontolt]
	int j = 0;
	for(int i = 0; i < elen; i++, j++)
	{
		if(out[j] == '\r')
			j++;
		if(out[j] == '\n')
			j++;
		if(out[j] == ' ')
			j++;
		out[i] = out[j];
	}
	//////////////////////////////////////////////////////////////////////////

	std::string cmd = (char*)out;
	DPRINTF("out len:%d\tcmd len:%d\n", strlen((char*)out), cmd.size());
	sprintf((char*)out, "/usr/bin/curl -X POST -d \"data=%s\" \"http://%s\"\0\0", cmd.c_str(), m_strAureoLamNS.c_str());
#if 0
	std::string cmd = "/usr/bin/curl -X POST -d \"";
	DPRINTF("%s\n", cmd.c_str());
	cmd += (char*)out;
	DPRINTF("%s\n", cmd.c_str());
	cmd += "\" \"http://";
	DPRINTF("%s\n", cmd.c_str());
	cmd += m_strAureoLamNS;
	DPRINTF("%s\n", cmd.c_str());
	cmd += "/heartbeat\"";
#endif // 0

	DPRINTF("len:%d %s\n", strlen((char*)out), (char*)out);
	IExternCall *pEc = CreateExternCall();
	pEc->RunCommand((char*)out);
	while(!pEc->IsFinish())
	{
		usleep(200000);
	}
	DPRINTF("Result: %s\n", pEc->GetOutput().c_str());
	if((m_logCount % 15) == 0)
	{
		if(gLog)
		{
			gLog->Write(LOG_NETCOMMU, (char*)out);
			gLog->Write(LOG_NETCOMMU, pEc->GetOutput().c_str());
		}
		m_logCount = 0;
	}
	m_logCount++;

	delete[] buf;
	return true;
}

void AureoLamThread::StartRoundRecv()
{
	m_strRoundTime = getDateTime();
}

void AureoLamThread::cleanThread()
{
	CActiveThread::stop();
}

void AureoLamThread::doit()
{
	int i = 0;
	while(!bExit)
	{
		i++;
		if(i > 120*1000)
		{
			HeartBreat();
			i = 0;
		}
		else
			usleep(1000);
	}
}

void AureoLamThread::StartRecvTask()
{
	m_strTaskTime = getDateTime();
}

void AureoLamThread::ResetRecvTask()
{
	m_strTaskTime.clear();
}

void AureoLamThread::ResetRoundRecv()
{
	m_strRoundTime.clear();
}

AureoLamThread gAureoLam;
AureoLamThread* CreateAureoLam()
{
	return &gAureoLam;
}

void ReleaseAureoLam(AureoLamThread* pAL)
{

}


#endif
