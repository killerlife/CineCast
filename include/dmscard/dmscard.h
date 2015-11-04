#ifndef __DMSCARD_H__
#define __DMSCARD_H__

// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DMSCARD_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// DMSCARD_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef WIN32
#ifdef DMSCARD_EXPORTS
#define DMSCARD_API __declspec(dllexport)
#else
#define DMSCARD_API __declspec(dllimport)
#endif
#else
#define DMSCARD_API 
#endif
#ifdef __cplusplus
extern "C" {
#endif

// 设置使用的读卡器类型
DMSCARD_API void SetReaderType(int nType);

//
// 以下接口暂时不使用，为保持跟以外对接接口相同和兼容，固暂时保留，代码中
// 不给予实现
//
DMSCARD_API char* DMSExIcGetDevVersionInfo();
DMSCARD_API unsigned long DMSExIcInitProc(char* SerialDev, unsigned long baud);
DMSCARD_API int DMSExIcExitProc(unsigned long icdev);

// ================================ DMS 读卡器接口 ===============================

DMSCARD_API int DMSExIcCpu_ResetProc(unsigned long icdev, unsigned char *receive_data);
DMSCARD_API int DMSExIcCpu_ProtocolProc(unsigned long icdev, int len, unsigned char *send_data, unsigned char *receive_data);

#ifdef __cplusplus
}
#endif

#endif	//__DMSCARD_H__