#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "mytype.h"


typedef enum Recv_State_e
{
    RECV_STATE_IDLE = 0,
    RECV_STATE_RECV,
    RECV_STATE_RECV_END,
    RECV_STATE_LOST_ANALYSIS,
    RECV_STATE_RESERVED,
    RECV_STATE_LOST_INFO_REPORT,
    RECV_STATE_LOST_INFO_REPORT_CONFIRM,
    RECV_STATE_REQ_MD5_KEY_FILE,
    RECV_STATE_MD5_KEY_RECV_CONFIRM,
    RECV_STATE_DATA_CHECK,
    RECV_STATE_DATA_CHECK_SUCCESS,
    RECV_STATE_TASK_SUCCESS
}Recv_State_t;

typedef enum Net_Command
{
	NET_LOGIN_REQ = 0x11,
	NET_LOGIN_REP,
	NET_AUTH_REQ,
	NET_AUTH_REP,
	NET_LOST_REQ = 0x21,
	NET_LOST_UPLOAD,
	NET_LOST_CONFIRM,
	NET_MD5_REQ,
	NET_MD5_REP,
	NET_MD5_CONFIRM,
	NET_DECRYPT_REQ,
	NET_DECRYPT_UPLOAD,
	NET_DECRYPT_CONFIRM,
	NET_HEARTBEAT_REQ = 0x31,
	NET_HEARTBEAT_UPLOAD,
	NET_HEARTBEAT_CONFIRM,
	NET_LOG_REQ,
	NET_LOG_REP,
	NET_REBOOT_REQ = 0X81,
	NET_REMOTE_UPDATE_PUSH = 0X71,
	NET_REMOTE_UPDATA_PUSH_REP,
	NET_REMOTE_UPDATE_REQ,
}Net_Command_t;

struct L_LOGIN_REQ
{
	uint32 machineID;
	uint32 loginCount;
	uint8 version[16];
	uint8 startupTime[20];
	uint8 loginTime[20];
	uint32 crc32;
	L_LOGIN_REQ()
	{
		loginCount = 0;
		memset(version, 0, 16);
		memcpy(version, "1.0.0.0", 7);
	}
};

struct R_LOGIN_REP
{
	uint8 serverIP[16];
	uint32 port;
	uint32 sys_year;//  系统当前年份 32位无符号整型
	uint32 sys_month;//  系统当前月份 32位无符号整型 1-12
	uint32 sys_date;//  系统当前日期 32位无符号整型 1-31
	uint32 sys_hour;//  系统当前小时 32位无符号整型 0-24
	uint32 sys_minute;//  系统当前分钟 32位无符号整型 0-59
	uint32 sys_second;//  系统当前�?32位无符号整型 0-59
	uint32 crc32;
};

struct L_AUTH_REQ
{
    uint32 machineID;//  接收机id
    uint8 version[16];
    uint8 loginTime[20];
    uint32 logMode;//  日志模式
    uint32 reserved_1;//  reserved
    uint32 heartCycle;//  心跳周期
    uint32 connectMode;//  连接方式
	//------------------------------------------------------------------
	//In this structure, we only declare one disk to save data
    uint32 diskNum;
	//The below will be an structure list for disk capacity and reserved
	//We use one diskCapacity and reserved because declare one disk
    uint64 diskCapacity;
    uint32 reserved_2;
	//------------------------------------------------------------------
    uint32 crc32;
};

struct R_AUTH_REP
{
    uint8 sessionKey[16];
    uint8 randomCcode[16];
    uint32 logMode;//  日志模式
    uint32 reserved;//  reserved
    uint32 heartCycle;//  心跳周期
    uint32 connectMode;//  连接方式
    uint32 crc32;

};

struct L_MD5_KEY_REQ//  MD5与密钥请求报�?
{
    uint32 filmID;//  影片ID
    uint64 reserved;
    uint32 crc32;
};

struct R_MD5_KEY_REP//  MD5与密钥反馈报�?
{
    uint32 filmID;//  影片ID
    uint32 fileLength;//  文件长度
    uint32 fileCrc;//  文件crc 32位无符号整数
    uint8 fileContent[];
	//We really need to confirm CRC?
	//Because TCP/IP already do the CRC check
    uint32 crc32;
	R_MD5_KEY_REP()
	{
		fileContent = NULL;
	}
	~R_MD5_KEY_REP()
	{
		if(fileContent != NULL)
			delete[] fileContent;
	}
};

struct L_MD5_KEY_CONFIRM//MD5与密钥确认报�?
{
    uint32 filmID;//  影片ID
    uint64 reserved;
    uint32 crc32;
};

struct  R_MD5_RESULT_REQ//解密与MD5校验结果请求报文
{
    uint32 filmID;//  影片ID
    uint64 reserved;
    uint32 crc32;
};

struct L_MD5_RESULT_REPORT//解密与MD5校验结果上报报文
{
    uint32 filmID;//  影片ID
    uint32 md5Result;//md5校验结果
    uint64 reserved;
    uint64 rollBackLen;
    uint32 crc32;
};

struct R_MD5_RESULT_CONFIRM//解密与MD5校验结果确认报文
{
    uint32 filmID;//  影片ID
    uint64 reserved;
    uint32 crc32;
};

struct L_HEART_INFO_REPORT//心跳信息上报报文
{
    uint32 machineID;//  接收机的机器�?
    uint32 temperature;//  运行温度
    uint32 CPUUsage;//  CPU使用�?
    uint64 memTotal;//  内存总数
    uint64 memIdle;//  内存空闲�?
    uint64 networkRate;//  互联网数据速率
    uint32 agc;//  卫星信道信号强度 1-100
    uint32 snr;//  卫星信道信号质量 1-100
    uint32 interfaceRate;//  接口接收速率 Mb/s
    uint32 dataRate;//  数据接收速率 Mb/s
    uint8 reserved[16];
    uint32 filmID;//  影片ID ,if no film,the value is 0
    uint32 filmNameLength;//  影片名称长度
    uint8* pFilmName;
    uint64 filmLength;//  影片总大�?
    uint64 filmSegment;//  影片segment包数�?
    uint32 recvRound;//  接收轮次
    uint8 taskStartTime[20];
    uint8 recvStartTime[20];
    uint32 filmRecvState;//  影片接收状�?
    uint64 reserved2;//本结构体定义中有两处关于reserved
    uint64 recvLength;//  影片已接收大�?
    uint64 lostSegment;//  影片已丢失segment包数�?
    uint32 crc32;
};

struct R_LOG_REQ
{
    uint32 logStart;
    uint32 logEnd;
    uint32 reerved;
    uint32 crc32;
};

struct L_LOG_REP
{
    uint32 fileSize;
    uint32 logStart;
    uint32 logEnd;
    uint32 uploadLength;
    uint8* pContent;
    uint32 crc32;
};

struct R_REBOOT_REQ
{
    uint8 randomCode[16];
    uint32 crc32;
};

struct R_REMOTE_UPGRADE_REQ
{
    uint64 reserved_1;
    uint8 updateSerialNo[16];
    uint8 reserved_2[16];
    uint32 descriptionLength;
    uint8* pDescription;
    uint32 FID;
    uint32 fileNameLength;
    uint8* pFileName;
    uint32 fileLength;
    uint8* pFile;
    uint32 crc32;
};

struct L_REMOTE_UPGRADE_PUSH_REP
{
    uint32 updateCheckResult;
    uint32 reserved_1;
    uint8 oldVersion[16];
    uint8 updateSerialNo[16];
    uint8 reserved_2[16];
    uint32 crc32;
};

struct R_REMOTE_UPGRADE_PUSH_CONFIRM
{
    uint8 updateSerialNo[16];
    uint8 reserved[16];
    uint32 crc32;
};

struct L_LOST_INFO
{
    uint32 filmID;//  影片ID ,if no film,the value is 0
    uint32 lostNum;
    uint64 receivedByteCount;
    uint32 recvState;//  影片接收状�?
    uint32 reserved;
    uint32 lostLength;
    uint8* pLost;
    uint32 crc32;
};

struct R_LOST_INFO_CONFIRM
{
    uint32 filmID;//  影片ID ,if no film,the value is 0
    uint64 reserved;
    uint32 crc32;
};

struct Lose_Info_Req_Msg
{
    uint32 filmID;//  影片ID ,if no film,the value is 0
    uint64 reserved;
    uint32 crc32;
};

std::string getDateTime();
void aes_encrypt(uint8 *key, uint8 *in, uint8 *out, uint32 size);
void aes_decrypt(uint8 *key, uint8 *in, uint8 *out, uint32 size);

#endif // MESSAGE_H
