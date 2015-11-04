#ifndef LOG_STRUCT_H
#define LOG_STRUCT_H

#include <vector>
#include <list>
#include <string>

typedef enum {
	LOG_ALL = 0,
	LOG_UI,
	LOG_SYSTEM,
	LOG_ERROR,
	LOG_DVB,
	LOG_TSPARSER,
	LOG_NETCOMMU,
	LOG_SATELLITE,
	LOG_BASIC,
	LOG_CONFIG,
	LOG_END
}LOGTYPE;

typedef enum {
	EXPORT_GDC=0,
	EXPORT_EXCEL,
	EXPORT_TEXT
}EXPORTTYPE;

typedef struct logDateTime{
	unsigned short year;
	unsigned char  month;
	unsigned char  day;
}LOGDATETIME;

typedef struct logTime{
	unsigned char hour;
	unsigned char minute;
	unsigned char seconds;
}LOGDAYTIME;

typedef struct logQueryResult{
	LOGTYPE         type;
	//LOGDATETIME     ymd;
	std::string     time; //yyyy-mm-dd/hh:mm:ss   
	std::string     text;   
}LOGQUERYRESULT;

typedef std::vector<LOGQUERYRESULT> TLogQueryResultArray;
typedef std::list<std::string> TLogExportContextList;


#endif //LOG_STRUCT_H

