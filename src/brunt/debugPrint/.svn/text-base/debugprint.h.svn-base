// 修改于崔海军的DebugOutTrace
#ifndef _DEBUGPRINT_20081111_H__
#define _DEBUGPRINT_20081111_H__

#include <stdio.h>
#include <malloc.h>

#ifdef _WIN32
	#ifdef DEBUGPRINT_EXPORTS
	#define DEBUGPRINT_API __declspec(dllexport)
	#else
	#define DEBUGPRINT_API __declspec(dllimport)
	#endif
#else
	#define DEBUGPRINT_API
#endif//_WIN32


enum DP_LEVEL
{
	DP_All = 0 ,			// 等级最低，当设为DP_ALL时，所有类型的DebugPrint全部输出。
	DP_Info,
	DP_Debug,
	DP_Trace,
	DP_Warning,
	DP_Error,
	DP_Fatal			// 等级最高， 
};

class IDebugPrint
{
public:
	/**
	 * @brief 设置信息级别
	 * @param lev 输出Info级别 
	 */
	virtual void SetPrintLevel(int lev) = 0;

	virtual int GetPrintLevel() = 0;

	/**
	 * @brief 写日志
	 * @param lev          信息级别
	 * @param moduleName   模块名称
	 * @param sFormat      输出参数(等同printf的第一个参数)   
	 * @param ...          参数列表(等同printf的参数列表，支持 %d%f%s)
	 */
   	virtual void Print(int lev, const char* moduleName, const char *fmt, ...) = 0;
    
	virtual ~IDebugPrint(){};
};

DEBUGPRINT_API IDebugPrint& getDebugPrint();

DEBUGPRINT_API void DP_PrintF(FILE* fp, const char *fmt,...);
DEBUGPRINT_API void DP_PrintS(const char *fmt,...);

#define DEBUG_PRINT

#ifdef 	DEBUG_PRINT
	#define DPRINT(a)		getDebugPrint().Print a
#else
	#define DPRINT(a)		
#endif//DEBUG_PRINT


#endif//_DEBUGPRINT_20081111_H__


