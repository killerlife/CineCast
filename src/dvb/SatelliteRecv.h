#pragma once

#include <string>
#include "mytype.h"

#include <stdlib.h>
#include <stdio.h>

#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h>
#include <linux/dvb/audio.h>
#include <linux/dvb/version.h>
#include <netcomm/UiProtocal.h>

#ifdef USE_POLL
#include <sys/poll.h> 
#endif

#if DVB_API_VERSION < 5
#error szap-s2 requires Linux DVB driver API version 5.0 or newer!
#endif 

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Remember redefine this function
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define STORAGE_PATH "/storage/"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// These codes are part of TUNER
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct t_channel_parameter_map {
	int user_value;
	int driver_value;
	std::string user_string;
}; 

static struct t_channel_parameter_map inversion_values[] = {
	{   0, INVERSION_OFF, "off" },
	{   1, INVERSION_ON,  "on" },
	{ 999, INVERSION_AUTO },
	{ -1 }
};

static struct t_channel_parameter_map coderate_values[] = {
	{   0, FEC_NONE, "none" },
	{  12, FEC_1_2,  "1/2" },
	{  23, FEC_2_3,  "2/3" },
	{  34, FEC_3_4,  "3/4" },
	{  35, FEC_3_5,  "3/5" },
	{  45, FEC_4_5,  "4/5" },
	{  56, FEC_5_6,  "5/6" },
	{  67, FEC_6_7,  "6/7" },
	{  78, FEC_7_8,  "7/8" },
	{  89, FEC_8_9,  "8/9" },
	{ 910, FEC_9_10, "9/10" },
	{ 999, FEC_AUTO, "auto" },
	{ -1 }
};

static struct t_channel_parameter_map modulation_values[] = {
	{  16, QAM_16,   "QAM16" },
	{  32, QAM_32,   "QAM32" },
	{  64, QAM_64,   "QAM64" },
	{ 128, QAM_128,  "QAM128" },
	{ 256, QAM_256,  "QAM256" },
	{   2, QPSK,    "QPSK" },
	{   5, PSK_8,    "8PSK" },
	{   6, APSK_16,  "16APSK" },
	{   7, APSK_32,  "32APSK" },
	{   8, DQPSK,    "OFDM" },
	//  {   9, COFDM,   "COFDM" },
	{  10, VSB_8,    "VSB8" },
	{  11, VSB_16,   "VSB16" },
	{ 998, QAM_AUTO, "QAMAUTO" },
	{ -1 }
};

static struct t_channel_parameter_map system_values[] = {
	{   0, SYS_DVBS,  "DVB-S" },
	{   1, SYS_DVBS2, "DVB-S2" },
	{ -1 }
};


static struct t_channel_parameter_map rolloff_values[] = {
	{   0, ROLLOFF_AUTO, "auto"},
	{  20, ROLLOFF_20, "0.20" },
	{  25, ROLLOFF_25, "0.25" },
	{  35, ROLLOFF_35, "0.35" },
	{ -1 }
}; 

static struct t_channel_parameter_map polvert_values[] = {
	{ 0, 0, "H"},
	{ 1, 1, "V"},
	{ -1 }
};

#if 0
typedef struct tuner_conf {
	unsigned int nFreq;
	unsigned int nHiBand;
	unsigned int nSR;
	unsigned int nMis;
	unsigned int nSatNo;
	std::string strDevName;		//Such as "/dev/dvb/adapter0/frontend0"
	std::string strDelSys;		//Such as "DVB-S", "DVB-S2"
	std::string strFec;			//Such as "1/2", "2/3", "3/4", "4/5", "5/6"
	std::string strModulation;	//Such as "8PSK", "16APSK"
	std::string strRollOff;		//Such as "0.20", "0.25", "0.35"
	std::string strPolVert;		//Such as "V", "H"

	//for default
	tuner_conf()
	{
		strDevName = "/dev/dvb/adapter0/frontend0";
		strDelSys = "DVB-S2";
		strFec = "3/4";
		strModulation = "8PSK";
		strRollOff = "0.25";
		strPolVert = "V";
		nFreq = 12500000;
		nHiBand = 11300000;
		nSR = 43200000;
		nMis = -1;
		nSatNo = 1;
	};
} TUNER_CONF;

typedef struct tuner_info
{
	uint8 nStatus;
	uint16 nAGC;
	uint16 nSNR;
	uint32 nBER;
	uint32 nUNC;
	uint8 nLock;
	tuner_info()
	{
		nStatus = nAGC = nSNR = nBER = nUNC = nLock = 0;
	};
} TUNER_INFO;
#endif

//class ILog;

class ITuner
{
public:
	virtual ~ITuner(){};
	virtual bool SetTunerConf(TUNER_CONF conf) = 0;
	virtual bool Zapto() = 0;
	virtual bool Zapto(TUNER_CONF conf) = 0;
	virtual TUNER_INFO GetTunerInfo() = 0;
};

ITuner * CreateTuner();
void ReleaseTuner(ITuner* tuner);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// End of TUNER codes
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// These codes are part of FILTER
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class Filter
{
public:
	Filter();
	~Filter();
	bool SetStrDevName(std::string strDevName);
	bool SetFilterID(uint16 pid, uint16 tid);
	bool SetPesFilterID(uint16 pid, uint16 tid);
	bool ReadFilter(uint8 *buf, uint16& count);
	bool Stop();

private:
	int fd;
	uint16 nPid;
	uint16 nTid;

#ifdef USE_POLL
	struct pollfd m_poll;
#endif

//	ILog* pLog;
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// End of FILTER codes
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if 0
enum FILTER_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Filter Processor
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class IFilterProcessor
{
public:
	virtual ~IFilterProcessor(){};
	virtual bool Init(void *param1, void *param2) = 0;
	virtual bool Start() = 0;
	virtual bool Stop() = 0;
	virtual int GetStatus() = 0; 
	virtual uint64 ReciveLength() = 0;
	virtual uint64 FileLength() = 0;
};

#define D_NOTIFY	(0x10000001)
#define D_START		(0x10000002)
#define D_PAT		(0x10000003)
#define D_PMT		(0x10000004)
#define D_DAT		(0x10000005)
#define D_FINISH	(0x10000006)
#define D_CANCEL	(0x10000007)

uint32* GetDebugCommand();