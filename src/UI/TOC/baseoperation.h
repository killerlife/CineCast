#ifndef _BASE_OPERATION_H_
#define _BASE_OPERATION_H_
#include "my_struct.h"   //自定义数据类型
#include <QList>


int getMachine_ID();                           //获取接收机ID
void getHardKey(char HardKey[16]);              //获取当前接收机的硬件密钥

int getLoginCounter();                         //获取已经登陆次数
void getSoftVersion(char SoftVersion[16]);     //获取当前接收机cinecast软件版本号
void getTime_Startup(char Time_Startup[20]);   //获取接收机linux系统启动时间

int getHardDiskNum();                          //获取接收机硬盘数量
unsigned long long getAllDiskCapacity();       //获取接收机硬盘总容量

//比较现在版本号和接收到的版本号如果收到的升级包比现有版本新则为0，表示有效,否则为1
int compareVersion(char OldVersion[16],char UpdateSerialNo[16]);

//比较现在版本号和接收到的版本号
//void getOldSoftVersion(char Version[16]);    //待填充接收机软件老版本号

void getSerialNo(char SerialNo[16]);         //接收到的升级包序列号
void getUpdateFileName(char name[256]);      //获取升级包文件名
void getinstall_sh(char path[256]);          //查找升级脚本路径 待实现
void getMeetKey(char MeetKey[16]);           //Sever端 计算会晤密钥 待实现
void getRandomCode(char RandomCode[16]);     //Sever端 计算随机码--暂不



bool getHardKeyuseID(int ID,char HardKey[16]);      //通过设备ID查找对应的硬件指纹
//获取已经注册的ID和对应的硬件指纹
void getRegisterIDAndHardKey(QList<RegisterIDHARDKEY>& ListRegisterIDHardKey);


#endif _BASE_OPERATION_H_



