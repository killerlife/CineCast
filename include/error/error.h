#pragma once	// 2009-01-21 10:49 lp

#ifdef  _WIN32
	#ifdef ERROR_EXPORTS
	#define ERROR_API __declspec(dllexport)
	#else
	#define ERROR_API __declspec(dllimport)
	#endif
#else
	#define ERROR_API 
#endif//_WIN32

/////////////////////////////////////////////////////////////////////////////////
// mxfReader error defination
#define MFR_E_DMSHEADER(a)		(-20080000-(a))
#define MFR_E_OFFSET(a)			(-20090000-(a))

#define MFR_E_HEAD							MFR_E_OFFSET(5)

#define MFR_E_OPENESSENCEFILEFAILED			MFR_E_OFFSET(5)
#define MFR_E_NOTOPENED						MFR_E_OFFSET(6)
#define MFR_E_READBEYOND					MFR_E_OFFSET(7)
#define MFR_E_ALLOCMEMORYFAILED				MFR_E_OFFSET(8)
#define MFR_E_FRAMETYPEINVALID				MFR_E_OFFSET(9)
#define MFR_E_READTYPEINVALID				MFR_E_OFFSET(10)
#define MFR_E_OPENNOTSAMETYPE				MFR_E_OFFSET(11)
#define MFR_E_DECRYPTERROR					MFR_E_OFFSET(12)
#define MFR_E_GETBUFFERERROR				MFR_E_OFFSET(13)
#define MFR_E_OPENONLYONEITEM				MFR_E_OFFSET(14)

#define MFR_E_TAIL							MFR_E_OFFSET(100)


ERROR_API bool FormatErrMsg(int error, char* buff, int buffsize);
