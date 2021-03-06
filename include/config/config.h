﻿#ifndef _CONFIG_H_200812151005__
#define _CONFIG_H_200812151005__

#include <string>
#include <vector>
#include "dvb/mytype.h"

using namespace std;

class IConfig
{
public:
	virtual string getRemoteUrl() = 0;
	virtual int getRemotePort() = 0;
	virtual const string getLeonisUrl() = 0;
	virtual const int getLeonisPort() = 0;
	virtual const string getProgramRootPath() = 0;
	virtual const string getUsbRootPath() = 0;
	virtual const string getBinRootPath() = 0;
	virtual const string getLogPath() = 0;
	virtual const string getLogOutPath() = 0;
	virtual const string getBreakPointPath() = 0;
	virtual uint32 getMachineId() = 0;
	virtual string getHardKey() = 0;
	virtual ~IConfig(){};
};


IConfig* getConfig();
void ReleaseConfig(IConfig*);


#endif//_CONFIG_H_200812151005__

