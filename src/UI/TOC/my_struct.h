#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_

//ʱѼ㷨ͷڴ˴

#include <string>

typedef char  int8;
typedef short int16;
typedef int   int32;
typedef long long int64;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long long uint64;

#ifndef NULL
#define NULL 0
#endif

//#pragma pack(2)   //old #pragma pack(1)
//#pragma pack(push,1)  //״̬
//#pragma pack(pop)     //ָ״̬
//#pragma pack(4)       //趨Ϊ4ֽڶ
#pragma pack(1)

enum FILTER_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};

typedef enum
{
	KEY_NO=0,     //
	KEY_HARD=1,   //Ӳ
	KEY_MEET=2,   //Կ
} KEY_TYPE;       //ļ



typedef enum
{
	UI_SEND_UPDATE_FILE=0X1000,        //ļ
	UI_GET_LOG,                          //־Ĳ 
	UI_GET_MD5RES,
	UI_REBOOT,
	UI_LOG,
	UI_TMS_LIST,
	UI_TMS_INFO,
	UI_CLOSE_SOCKET,
} CMD_UItoSocket;    //UIsocket̵߳ͨUIsocket



typedef enum
{
	SK_UPDATE_TREEWIDGET_LIST=0X2000,      //UIб
	SK_UPDATE_PROGRESSBAR,             //½

	SK_UPDATE_TEXTBROWSER,

} CMD_SockettoUI;    //UIsocket̵߳ͨ

typedef enum
{
	SV_SOCKET_DISCONNECT=0X3000,    //socketϿ

} CMD_TcpServertoUI;    //TcpServertoUI


typedef enum
{
	//SeverҪǰ׺S   Client˼C ̫
	M_SEND_TEST,
	PKG_HEAD=0x55,           //ݰͷ
	CMD_ZERO=0x0000,        //cmd_Sub 
    CMD_REV=0x78563412,      //Զ屣


	CMD_LOGIN_REQUEST= 0x0011,      //¼
	CMD_LOGIN_BACK,                  //¼  
	CMD_AUTHEN_REQUEST,              //֤ 
	CMD_AUTHEN_BACK,                 //֤
	CMD_LOST_REPORT = 0x22,
	CMD_MD5_REQUEST = 0x24,
	CMD_MD5_RES_UP_REQUEST=0x28,
    CMD_UPDATE_FILE_RECEV=0x0071,   //ջԶͱ
    CMD_UPDATE_BACK,                 //ջԶͷ
    CMD_UPDATE_START,                //ջԶ,ʵջʼ

    CMD_LOG_REQUEST=0x0100,           //־
    CMD_LOG_BACK,                      //־ش

	CMD_HEARTBREAT=0x0032,
    CMD_LOGFILE_REQUEST=0x0034,       //־ļ
    CMD_LOGFILE_BACK=0x0035,          //־ļ

} COMMOND_TYPE;



//server˱ѾעĽջIDӲָ
typedef struct _RegisterIDHARDKEY_
{
    uint32  ID;
    char HardKey[16];
}RegisterIDHARDKEY;



//ͨѶЭ鱨Ļṹ
//Ƕбǰ,Ϊͷ BASEprotocol
typedef struct _KH_           // HEADER дHD  Package Header дKH  pKHpPHÿ
{
	uint8   pkgHead;            //preamble;//8λ޷ͣʼֶͬΣΪ(0x55)
	uint8   flag_pwd;           //ܱ 0	δ 1ĲӲָƼ 2	ĲûԿ
	uint32  rev;                //reserved 
	uint16  cmd;                //ñĵ
	uint16  cmd_Sub;            //ñĵĸ Sub_cmd
    uint32  cmd_length;         //Payload_length;//غݳȣ(ݱ+CRC32Уֵ)
    //uint8  Payload_data_byte;  
}KH;



//½  128λֶ 16ֽ
typedef struct _LOGINREQUEST_
{
	KH kh;
	uint32 Machine_ID;          //ʾջIDţֶβ
	uint32 LoginCounter;        //ջĵ¼
	char   SoftVersion[16];     //bslbf 128λֶ ʾջǰİ汾ţɳж
	char   Time_Startup[20];    //bslbf 160λֶ ʾջʱ ʽΪYYYY-MM-DD HH:MM:SS
	char   Time_Login[20];      //bslbf 160λֶ ʾջ½ʱ ʽΪYYYY-MM-DD HH:MM:SS
    uint32 CRC32;                //rpchof ṹֶμǰCRCУֵ
}LOGINREQUEST;


typedef struct _HEARTBREAT_1_
{
	KH kh;
	uint32 machineID;//  接收机的机器?
	uint32 temperature;//  运行温度
	uint32 CPUUsage;//  CPU使用?
	uint64 memTotal;//  内存总数
	uint64 memIdle;//  内存空闲?
	uint64 networkRate;//  互联网数据速率
	uint32 agc;//  卫星信道信号强度 1-100
	uint32 snr;//  卫星信道信号质量 1-100
	uint32 interfaceRate;//  接口接收速率 Mb/s
	uint32 dataRate;//  数据接收速率 Mb/s
	uint8 reserved[16];
	uint32 filmID;//  影片ID ,if no film,the value is 0
	uint32 filmNameLength;//  影片名称长度
}HEARTBREAT_1;

typedef struct _HEARTBREAT_2_ {
	uint64 filmLength;//  影片总大?
	uint64 filmSegment;//  影片segment包数?
	uint32 recvRound;//  接收轮次
	uint8 taskStartTime[20];
	uint8 recvStartTime[20];
	uint32 filmRecvState;//  影片接收?
	uint64 reserved2;//本结构体定义中有两处关于reserved
	uint64 recvLength;//  影片已接收大?
	uint64 lostSegment;//  影片已丢失segment包数?
	uint32 crc32;
}HEARTBREAT_2;

//½
typedef struct _LOGINBACK_
{
	KH kh;
	char    ReSeverIP[16];        //ضҵIP 128λֶΣʾضҵIPַʽΪxxx.xxx.xxx.xxx
	uint32  ReSeverPort;          //ضҵIP ˿ں
    uint32  Year;                  //ϵͳǰ,,գʱ
    uint32  Month;                 //32λ޷ͣʾϵͳǰ·ݣȡֵΧ112  
    uint32  Day;                   //32λ޷ͣʾϵͳǰڣȡֵΧ131
    uint32  Hour;                  //32λ޷ͣʾϵͳǰСʱȡֵΧ024
    uint32  Minute;                //32λ޷ͣʾϵͳǰӣȡֵΧ059
    uint32  Seconds;               //32λ޷ͣʾϵͳǰӣȡֵΧ059
    uint32 CRC32;                  //rpchof ṹֶμǰCRCУֵ
}LOGINBACK;



//֤ authentication request
typedef struct _AuthenREQUEST_
{
	KH kh;
	uint32  Machine_ID;           //ʾջIDšֶβܡ
    char    SoftVersion[16];      //bslbf  128λֶ ʾջǰİ汾ţɳж
    char    Time_Login[20];       //bslbf  160λֶ ʾջ½ʱ ʽΪYYYY-MM-DD HH:MM:SS
    uint32  Model_Log;            //־ģʽ ʾջǷϱ־ֵֶΪ1ʾջϱ־ֵΪ2ʾϱ־
	uint32	Reserved1;			  //Reserved
    uint32  BeatCycle;            //heartbeat cycle ʾջϱݱĵڣλΪ롣ȷϵջϱڲͬջϱԴΪ׼
    uint32  Model_Connect;        //ӷʽ ʾջջԶάϵͳӵķʽֵֶΪ1ʾ뷢ϵͳƽֲ̨ϵĳӣֵΪ0ʾ뷢ϵͳƽ̨ӣջֻݴҪʱջԶάϵͳӡ
    uint32  HardDiskNum;          //32λ޷ͣʾջǰصڽյӰֿӲ
    uint64  AllDiskCapacity;      //64λ޷ͣʾջӲ̵λΪֽڡ
	uint32	Reserved2;			  //Reserved
	uint32  CRC32;                 //rpchof ṹֶμǰCRCУֵ
}AuthenREQUEST;


//֤ authentication feedback
typedef struct _AuthenBACK_
{
	KH kh;
	char    MeetKey[16];          //Կ128λֶΣջԶάϵͳݲϵͳջĵļԿ
    char    RandomCode[16];       //128λֶΣҪЯԹջ֤
    uint32  Model_Log;            //־ģʽ32λ޷ͣջԶάϵͳջȷϱ־ģʽ
    uint32  reserved;             //  
    uint32  BeatCycle;            // 32λ޷ͣջԶάϵͳջȷϱڡ
    uint32  Model_Connect;        //ӷʽ 32λ޷ͣջԶάϵͳջȷϱӷʽ
	uint32  CRC32;                //rpchof ṹֶμǰCRCУֵ
}AuthenBACK;



//ͱ (ļϢ,ļݺ)  Upgradepackage
typedef struct _UpdateFileINFO_
{
	KH kh;
    uint64  reserved;               //  
	char    UpdateSerialNo[16];    //128λֶΣкš
    char    reserved2[16];          //ֶ2 
    uint32  DescriptionLength;     //32λ޷ͣϢȡ
    char    Description[256];      //ɱ䳤ֶΣϢ(ݶ256λַ)
    uint32  FID;                    //32λ޷ͣļIDš
    uint32  UpdateFileNameLength;  //32λ޷ͣļȡ
    char    UpdateFileName[256];   //ɱ䳤ֶΣļ   (ݶ256λַ)
    uint32  UpdateFileLength;      //32λ޷ͣԱʾ4GB,ļȡָͨͣļȣͨŵͣȡֵΪ0
    //char UpdateFile[1024];       //ɱ䳤ֶΣļͨͣΪ͵ļģͨŵͣڡ
	//uint32  CRC32;               //rpchof ṹֶμǰCRCУֵ
}UpdateFileINFO;
//ɽṹϺ,ڴļ,ȻļCRCУ



/*
//Զļֶη͵Ľṹ
typedef struct _SendFileInfo_
{
    char    FileName[256];          //Ҫ͵ļ
    char    FilePath_dst[256];      //ļĿ·
    char    FilePath_rsc[256];      //ļĿ·
    uint64  FileSize;               //ļܴС,λֽByte
    uint32  FileCRC32;              //ļݱCRC32У
    char    FileMD5[32];            //ļݱMD5
    uint32  FilePkgAll;             //ļ    һļְ,64Kÿ
    uint32  FilePkgCurrent;         //ǰǵڼ
    uint32  FileEndFlag;            //ǰǷһ,Ϊ1,Ϊ0,
    uint32  FileEndPos;             //ǰһ,¼λ,λֽ,
    char    FileBuf[64*1024];       //ļ.
}SendFileInfo;
*/



//ջԶͷ  ջյķ
typedef struct _UpdateFileBACK_
{
	KH kh;
    uint32  UpdateCheckResult;     //32λ޷ͣȷϡֵΪ0ʾЧ1ʾЧ
    uint32  reserved;               //ֶ
    char  OldVersion[16];          //128λֶΣǰİ汾Ϣ
    char  UpdateSerialNo[16];      //128λֶΣյкš
    char  reserved2[16];           //ֶ
	uint32  CRC32;                  //rpchof ṹֶμǰCRCУֵ
}UpdateFileBACK;


//ջԶάϵͳڽյԶЯ췴ĺջ
//ջʼ
typedef struct _UpdateSTART_
{
	KH kh;
    char  UpdateSerialNo[16];          //128λֶΣİ汾Ϣ
    uint32  reserved;
	uint32  CRC32;                   //rpchof ṹֶμǰCRCУֵ
}UpdateSTART;





//־,ջ־
typedef struct _LOGREQUEST_
{
	KH kh;
    uint32  DateStart;   //־ļʼ 32λ޷ͣΪYYYYMMDDʮƱʾ
    uint32  DateEnd;    //־ļ 32λ޷ͣΪYYYYMMDDʮƱʾ
	uint32  LogType;    //־ 32λ޷ͣΪ0x74752485ʾ־ΪLEONISרиʽֵΪʽĿǰûʽ
	uint32  CRC32;   //CRC32 32λֶΣֶμǰCRCУֵ  
}LOGREQUEST;


//־,ͷݱ,ջĿ
typedef struct _LOGBACKINFO_
{
	KH kh;  
    uint32  LogLengthALL;   //־ļܳ   uint32ջ־ļܳȡ
    uint32  DateStart;      //־ļʼ  uint32ΪYYYYMMDDʮƱʾ
    uint32  DateEnd;        //־ļ  uint32ΪYYYYMMDDʮƱʾ
	uint32  LogLengthCurr; //ϴݳ uint32 ϴ־ݳȡ
//	char    log[1024*10]; 	 //־ݱɱֶ, variable	  bslbf
//	uint32  CRC32;          //          
}LOGBACKINFO;
//һ־ļȽС,ʱ10K,߸,û0 㴦






typedef struct _KL_
{
	int m_pkgHead;
	int m_keyID;
	int m_length;
	int m_flag;               //-1,0ݰͳɹ
}KL;

/*
typedef struct _KL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
	int    m_flag;           //-1,0ݰͳɹ
public:
	_KL_()
	{
		m_pkgHead = 0x7585;   //Զù캯
	}
} KL;
*/


typedef struct _SL_
{
	uint16 m_sID;
	uint16 m_length;
} SL;

//KLV
typedef enum
{
	RECEIVE_STATUS = 0,
	RECEIVE_FILM_NAME,
	RECEIVE_FILM_UUID,
	RECEIVE_FILM_ISSUE_DATE,
	RECEIVE_FILM_ISSUER,
	RECEIVE_FILM_CREATOR,

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
	uint16 nReceiveStatus;
	std::string strFilmName;
	std::string strUuid;
	std::string strIssueDate;
	std::string strIssuer;
	std::string strCreator;
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
} REMOTE_CONF;




#endif
