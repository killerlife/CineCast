#ifndef _BASE_OPERATION_H_
#define _BASE_OPERATION_H_

#include <list>
#include "UiProtocal.h"

class NetOperation
{
public:
	NetOperation();
	virtual ~NetOperation();
	
	std::list<NETWORK_CONF>& GetNetConfig();
	bool SetNetConfig(std::list<NETWORK_CONF>& m_listNetConf);

	REMOTE_CONF& GetRemoteConfig();
	bool SetRemoteConfig(REMOTE_CONF &m_remoteConf);

private:
	std::list<NETWORK_CONF> m_list;
	REMOTE_CONF m_rc;
};

class SatelliteConfOperation
{
public:
	SatelliteConfOperation();
	virtual ~SatelliteConfOperation();
	bool ReadConfig();
	bool WriteConfig();
};
#endif _BASE_OPERATION_H_