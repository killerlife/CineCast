#ifndef _INTERFACE_H
#define _INTERFACE_H

//#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CINEMA_OPERATION_SUCCESS 0 //函数成功
#define CINEMA_KEY_OPEN_ERROR 1 //key设备打开错误
#define CINEMA_SAM_OPEN_ERROR 2 //sam设备打开错误
#define CINEMA_KEY_INIT_ERROR 3 //key初始化错误
#define CINEMA_SAM_INIT_ERROR 4 //sam初始化错误
#define CINEMA_KEY_COMMUNICATION_ERROR 5 //key通信错误
#define CINEMA_SAM_COMMUNICATION_ERROR 6 //sam通信错误
#define CINEMA_KEY_AUTHENTICATE_ERROR 7 //key认证错误
#define CINEMA_SAM_AUTHENTICATE_ERROR 8 //sam认证错误
#define CINEMA_LINE_KEY_ERROR 9 //不同院线的Key
#define CINEMA_USER_KEY_ERROR 10 //不同设备的Key
#define CINEMA_KEY_DATA_ERROR 11 //Key数据出错
#define CINEMA_SAM_DATA_ERROR 12 //sam数据出错
#define CINEMA_KEY_OUT_OF_TIME 13 //Key时间失效
#define CINEMA_SAM_OUT_OF_TIME 14 //sam时间失效
#define CINEMA_PERIOD_BEFORE_ERROR 15 //档期未到
#define CINEMA_PERIOD_AFTER_ERROR 16 //档期已过
#define CINEMA_KEY_NO_AUTH 17 //key里无授权
#define CINEMA_COUNT_ERROR 18 //场次出错
#define CINEMA_COUNT_ZERO 19 //场次为0
#define CINEMA_DECRYPT_TIMEOUT 20 //解密超时
#define CINEMA_KLV_TRIPLET_ERROR 21 //klv结构出错
#define CINEMA_KLV_DECRYPTION_KEY_ERROR 22 //klv解密密钥出错


#define GDD_INFOLEN 60
#define GDD_KEYLEN 256
typedef struct _KEYINFO
{
unsigned char m_uchInfoLen[GDD_INFOLEN]; //信息长度
unsigned char m_uchKey1[GDD_KEYLEN]; //密钥一
unsigned long m_ulKey1Len; //密钥一长度，0表示无此密钥
unsigned char m_uchKey2[GDD_KEYLEN]; //密钥二
unsigned long m_ulKey2Len; //密钥二长度，0表示无此密且
}GDD_KEYINFO, *PGDD_KEYINFO;
typedef struct _tagReadFilmKeyInfo
{
int FilmItemCount; //Film item count in the CPU card
GDD_KEYINFO KeyInfo[40];
} ReadFilmKeyInfo;


#ifdef WIN32
typedef unsigned __int64 uint64_t;
typedef unsigned __int8 uint8_t;
#endif

/*读key信息的函数组*/
//打开设备
int OpenDevice();
//关闭设备
int CloseDevice();
//读出卡的信息，ReadInfo为返回值
//其中KeyInfo的信息如下：
//m_uchInfoLen格式如: 16Bytes是电影的hex uuid, 4Bytes是可用的场次，如90场为"0090",40Bytes为电影名
//m_uchKey2   格式如：25Bytes是电影档期开始时间，格式如"2008-06-04T09:00:00-00:00"表示为2008年6月4日9时0分0秒，25Bytes为档期结束时间，格式同前，1Bytes，档期有效标识位1有效，0无效，1Bytes，场次有效信息为1有效，0无效
int ReadCinemaInfo(ReadFilmKeyInfo* ReadInfo);


/*解密函数组*/
//解密初始化函数，cpluuid为入参，cpluuid为36Bytes,形如"9a88dd9f-3301-428e-b951-198eaca5b38b"，从dcp里查找出
int CinemaInit(unsigned char *cpluuid);
//解密函数，每个klv都需要解密
//klv为入参，decrypted_offset和decrypted_size为返回值
//klv为一个完整的加密klv结构，decrypted_offset给出解密后从klv+decrypted_offset数据为当前视频开始有效值，decrypted_size为视频数据大小。
int CinemaDecryptTripletKey(unsigned char *klv, uint64_t* decrypted_offset, uint64_t *decrypted_size);

//解密结束函数
int CinemaEnd();

/*这个key就是加密的key
原始的key通过
        memset(initKey, 84, 4);
        memset(initKey + 4, 48, 4);
        memset(initKey + 8, 65, 4);
        memset(initKey + 12, 56, 4);

        memset(ivec, '1', 4);
        memset(ivec + 4, '0', 4);
        memset(ivec + 8, '1', 4);
        memset(ivec + 12, '0', 4);
initKey作为加密密钥，ivec为向量，经过128bit cbc加密，得到pushDecryptKey函数的key*/

int PushDecryptKey(unsigned char *key);


#ifdef __cplusplus
}
#endif

#endif