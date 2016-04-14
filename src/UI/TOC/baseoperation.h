#ifndef _BASE_OPERATION_H_
#define _BASE_OPERATION_H_
#include "my_struct.h"   //�Զ�����������
#include <QList>


int getMachine_ID();                           //��ȡ���ջ�ID
void getHardKey(char HardKey[16]);              //��ȡ��ǰ���ջ���Ӳ����Կ

int getLoginCounter();                         //��ȡ�Ѿ���½����
void getSoftVersion(char SoftVersion[16]);     //��ȡ��ǰ���ջ�cinecast����汾��
void getTime_Startup(char Time_Startup[20]);   //��ȡ���ջ�linuxϵͳ����ʱ��

int getHardDiskNum();                          //��ȡ���ջ�Ӳ������
unsigned long long getAllDiskCapacity();       //��ȡ���ջ�Ӳ��������

//�Ƚ����ڰ汾�źͽ��յ��İ汾������յ��������������а汾����Ϊ0����ʾ��Ч,����Ϊ1
int compareVersion(char OldVersion[16],char UpdateSerialNo[16]);

//�Ƚ����ڰ汾�źͽ��յ��İ汾��
//void getOldSoftVersion(char Version[16]);    //�������ջ�����ϰ汾��

void getSerialNo(char SerialNo[16]);         //���յ������������к�
void getUpdateFileName(char name[256]);      //��ȡ�������ļ���
void getinstall_sh(char path[256]);          //���������ű�·�� ��ʵ��
void getMeetKey(char MeetKey[16]);           //Sever�� ���������Կ ��ʵ��
void getRandomCode(char RandomCode[16]);     //Sever�� ���������--�ݲ�



bool getHardKeyuseID(int ID,char HardKey[16]);      //ͨ���豸ID���Ҷ�Ӧ��Ӳ��ָ��
//��ȡ�Ѿ�ע���ID�Ͷ�Ӧ��Ӳ��ָ��
void getRegisterIDAndHardKey(QList<RegisterIDHARDKEY>& ListRegisterIDHardKey);


#endif _BASE_OPERATION_H_



