#ifndef __DCULC_H__
#define __DCULC_H__

#ifdef __cplusplus
extern "C"
{
#endif

HANDLE   __stdcall  IC_InitComm(__int16 port);
__int16  __stdcall  IC_ExitComm(HANDLE idComDev);
__int16  __stdcall  IC_Status(HANDLE idComDev);
__int16  __stdcall  IC_Down(HANDLE idComDev);
__int16  __stdcall  IC_InitType (HANDLE idComDev,__int16 type);
__int16  __stdcall  IC_Read(HANDLE idComDev,__int16 offset,__int16 len,unsigned char * databuffer);
__int16  __stdcall  IC_Write(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
__int16  __stdcall  IC_CheckCard(HANDLE idComDev);
__int16  __stdcall  IC_ReadVer(HANDLE idComDev,unsigned char *Ver);
__int16 __stdcall   hex2asc(unsigned char *strhex,unsigned char *strasc,__int16 length);
__int16 __stdcall   asc2hex(unsigned char *strasc,unsigned char *strhex,__int16 length);

//
__int16  __stdcall  IC_CpuGetProtocol(HANDLE idComDev);

__int16  __stdcall  IC_CpuReset(HANDLE idComDev,unsigned char *rlen, unsigned char *databuffer);
__int16  __stdcall  IC_CpuApdu(HANDLE idComDev,unsigned char slen,unsigned char * sendbuffer,unsigned char *rlen,unsigned char * databuffer);
__int16  __stdcall  IC_CpuReset_Hex(HANDLE idComDev,unsigned char *rlen, unsigned char *databuffer);
__int16  __stdcall  IC_CpuApdu_Hex(HANDLE idComDev,unsigned char slen,unsigned char * sendbuffer,unsigned char *rlen,unsigned char * databuffer);

__int16  __stdcall  IC_ReadProtection(HANDLE idComDev,__int16 offset,__int16 len,unsigned char* protbuffer);
__int16  __stdcall  IC_ReadProtection_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char* protbuffer);
__int16  __stdcall  IC_ReadWithProtection(HANDLE idComDev,__int16 offset,__int16 len,unsigned char* protbuffer);
__int16  __stdcall  IC_ReadWithProtection_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char* protbuffer);
__int16  __stdcall  IC_WriteProtection(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *protbuffer);
__int16  __stdcall  IC_WriteWithProtection(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
__int16  __stdcall  IC_WriteProtection_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *protbuffer);
__int16  __stdcall  IC_WriteWithProtection_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char * writebuffer);

__int16 __stdcall   IC_ReadPass_SLE4442(HANDLE idComDev,unsigned char*password);
__int16  __stdcall  IC_ReadCount_SLE4442(HANDLE idComDev);
__int16  __stdcall  IC_CheckPass_SLE4442(HANDLE idComDev,unsigned char* password);
__int16  __stdcall  IC_ChangePass_SLE4442(HANDLE idComDev,unsigned char*password);
//
__int16  __stdcall  IC_CheckPass_4442hex(HANDLE idComDev,unsigned char*password);
__int16  __stdcall  IC_ChangePass_4442hex(HANDLE idComDev,unsigned char*password);
__int16  __stdcall   IC_ReadPass_4442hex(HANDLE idComDev,unsigned char*password);
//
__int16  __stdcall  IC_ReadCount_SLE4428(HANDLE idComDev);
__int16  __stdcall  IC_CheckPass_SLE4428(HANDLE idComDev,unsigned char* password);
__int16  __stdcall  IC_ChangePass_SLE4428(HANDLE idComDev,unsigned char* password);
//
__int16  __stdcall  IC_CheckPass_4428hex(HANDLE idComDev,unsigned char* password);
__int16  __stdcall  IC_ChangePass_4428hex(HANDLE idComDev,unsigned char* password);
//


__int16  __stdcall  IC_Write24(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
__int16  __stdcall  IC_Write64(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
//hex read and write.
__int16  __stdcall  IC_Write_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
__int16  __stdcall  IC_Read_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char * databuffer);
__int16  __stdcall  IC_Write24_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
__int16  __stdcall  IC_Write64_Hex(HANDLE idComDev,__int16 offset,__int16 len,unsigned char *writebuffer);
//

__int16  __stdcall  Read_24C(HANDLE idComDev,short offset,short len, unsigned char *reData);
__int16  __stdcall  Write_24C(HANDLE idComDev,short offset,short len, unsigned char *wrData);
__int16  __stdcall  Read_24C64(HANDLE idComDev,short offset,short len, unsigned char *reData);
__int16  __stdcall  Write_24C64(HANDLE idComDev,short offset,short len, unsigned char *wrData);
// 

__int16  __stdcall  IC_Check_4442(HANDLE idComDev);
__int16  __stdcall  IC_Check_4428(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C01(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C02(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C04(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C08(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C16(HANDLE idComDev);
__int16  __stdcall  IC_Check_24C64(HANDLE idComDev);
__int16  __stdcall  IC_Check_CPU(HANDLE idComDev);

 
#ifdef __cplusplus
}
#endif

#endif	//__DCULC_H__