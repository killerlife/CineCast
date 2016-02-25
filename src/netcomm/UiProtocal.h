#ifndef _UI_PROTOCOL_H_
#define _UI_PROTOCOL_H_

#include <dvb/mytype.h>
#include <string>
#include <vector>
#include <string.h>

#pragma pack(1)
typedef enum
{
	//0x100 Status function
	S_GET_SATELLITE = 0x100, //include AGC/SNR/LOCK
	S_GET_RECEIVE,
	S_GET_REMOTE,
	S_GET_HARDDRIVER,
	S_GET_TMS,

	//0x150 Config function
	S_GET_CONFIG = 0x150, //include DelSys/FEC/MOD/ROL/POL
	S_SET_CONFIG,		//FRQ/LNB/SR/MI/SATNO/DEVNO

	N_GET_CONFIG,	//include dhcp/ip/netmask/gateway
	N_SET_CONFIG,	//dns/devno

	N_GET_REMOTE,	//remote ip
	N_SET_REMOTE,

	//0x200 Manager function
	M_GET_USB_MOUNT = 0x200,
	M_SET_USB_MOUNT,
	M_GET_HDD_CONTENT_LIST,
	M_SET_HDD_CONTENT_DEL,
	M_GET_USB_CONTENT_LIST,
	M_SET_USB_CONTENT_DEL,
	M_GET_HDD_INFO,
	M_GET_USB_INFO,

	M_SET_HDD_CONTENT_EXPORT = 0x300,
	M_GET_HDD_CONTENT_EXPORT,
	M_SET_HDD_FORMAT,
	M_GET_HDD_FORMAT,

	S_REBOOT = 0x350,
	S_SHUTDOWN,
	S_SET_FORMATDISK,
	S_GET_FORMAT_RES,
	S_GET_FORMAT_STATUS,

    //new
    M_USB_MOUNT,        
    M_USB_UNMOUNT,

    //new
	M_COPYDIR_HDD_TO_USB,
	M_GETCOPYPROCESS,  //ȡļ

    M_DELETE_DIR,      //ɾӰƬļĿ¼

	M_UPDATE_PROGRAM_LIST_HDD,     //UpdateProgramListˢӲб���Ա? 
	M_UPDATE_PROGRAM_LIST_USB,     //UpdateProgramListˢӲб���Ա? 
     
	M_IS_PROGRAM_LIST_READY_HDD,   //IsProgramListReadyѯǷ׼
	M_IS_PROGRAM_LIST_READY_USB,   //IsProgramListReadyѯǷ׼

	M_GET_RAID_CONTENT_LIST,
	M_SET_RAID_CONTENT_DEL,
	M_GET_RAID_INFO,
	M_UPDATE_PROGRAM_LIST_RAID,     //UpdateProgramListˢӲб���Ա? 
	M_IS_PROGRAM_LIST_READY_RAID,   //IsProgramListReadyѯǷ׼

	R_GET_RAID_INFO = 0x400,

} UiProtocalKey;


//һ־ʱṹ塣uiݸserver
typedef struct _LOGDATE_
{
	unsigned short after_year;
	unsigned char  after_month;
	unsigned char  after_day;
	unsigned short before_year;
	unsigned char  before_month;
	unsigned char  before_day;
}LOGDATE;



//charַ·ٸ
typedef struct _copy_path_
{
	char path_src[512];
	char path_dst[512];
public:
	_copy_path_()
	{
		memset(path_src, 0, 512);
		memset(path_dst, 0, 512);
	}
}copy_path;


typedef struct _tocopy_path_
{
	std::string path_src;
	std::string path_dst;
}tocopy_path;





typedef struct _KL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
public:
	_KL_()
	{
		m_pkgHead = 0x7585;
	}
} KL;

typedef struct _SL_
{
	uint16 m_sID;
	uint16 m_length;
} SL;

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

//KLV
typedef enum
{
	RECEIVE_STATUS = 0,
	RECEIVE_FILM_NAME,
	RECEIVE_FILM_UUID,
	RECEIVE_FILM_ISSUE_DATE,
	RECEIVE_FILM_ISSUER,
	RECEIVE_FILM_CREATOR,
	RECEIVE_EXTEND,

	TUNER_CONFIG = 0x20,
	TUNER_DEV_NAME,
	TUNER_DEL_SYS,
	TUNER_FEC,
	TUNER_MOD,
	TUNER_ROLL_OFF,
	TUNER_POL,

	NET_CONFIG = 0x40,
	NET_DEV_NAME,
	NET_DHCP,
	NET_IP,
	NET_NETMASK,
	NET_GATEWAY,
	
	REMOTE_CONFIG = 0x60,
	REMOTE_DNS,
	REMOTE_SERVER,
	REMOTE_PORT,

	CONTENT_ID = 0x80,
	CONTENT_NAME,
	CONTENT_PROGRESS,
	CONTENT_STATUS,
	CONTENT_FORMAT,
	CONTENT_FILMLENGTH,
	CONTENT_PROGRAMLENGTH,
	CONTENT_STEREOSCOPIC,
	CONTENT_ISSUER,
	CONTENT_ISSUEDATE,
	CONTENT_TIMERANGE,
	CONTENT_RECVSEGMENT,
	CONTENT_TOTALSEGMENT,
	CONTENT_RECV_DATETIME,
	CONTENT_LOCATE,

	CONTENT_PATH_SRC,   //Դ·ַ
	CONTENT_PATH_DST,   //Ŀ·ַ

	RAID_INFO_LEVEL,
	RAID_INFO_STATE,
	RAID_INFO_DEVSTATE,

} ReceiveKey;

typedef struct receive_info
{
	uint64 nFileLength;
	uint64 nReceiveLength;
	uint64 nTotalSegment;
	uint64 nReceiveSegment;
	uint64 nCrcErrorSegment;
	uint64 nLostSegment; //Reserved
	uint32 nFileID;
	uint32 nReceiveStatus;
	std::string strFilmName;
	std::string strUuid;
	std::string strIssueDate;
	std::string strIssuer;
	std::string strCreator;
	std::string strExtend;
} RECEIVE_INFO;

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

typedef struct network_conf {
	uint8 nDhcp;
	std::string strDevName;
	std::string strIp;
	std::string strNetmask;
	std::string strGateway;
} NETWORK_CONF;

typedef struct remote_conf {
	std::string strDns1;
	std::string strDns2;
	std::string strRemote;
	int nPort;
} REMOTE_CONF;

typedef struct content_info {
	std::string strId; //0
	std::string strName; //1
	std::string strProgress; //2
	std::string strStatus; //3
	std::string strFormat; //4
	std::string strFilmLength; //5
	std::string strProgramLength; //6
	std::string strStereo; //7
	std::string strIssuer; //8
	std::string strIssueDate; //9
	std::string strTimeRange;
	std::string strNational;
	std::string strIssueArea;
	std::string strSubAudio;
	std::string strPath; //14
}CONTENT_INFO;

typedef struct disk_info
{
	uint64 nTotal;
	uint64 nAvali;
}DISK_INFO;

typedef struct raid_info
{
	uint16 nRaidDevices;
	uint16 nActiveDevices;
	uint16 nWorkingDevices;
	uint16 nFailedDevices;
	uint64 nArraySize;
	uint64 nUsedSize;
	std::string strLevel;
	std::string strState;
	std::vector<std::string> strDevState;
}RAID_INFO;

#endif _UI_PROTOCOL_H_