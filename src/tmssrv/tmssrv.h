#pragma once

#define TMSSRV_PORT (20080)

class ITmsServer
{
public:
	virtual int Start(int port) = 0;
	virtual ~ITmsServer(){};
};

ITmsServer* getITmsServer();