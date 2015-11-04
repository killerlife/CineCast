#pragma once
#include <dvb/mytype.h>
#pragma pack(1)
typedef enum
{
	//0x100 Status function
	S_GET_SATELLITE = 0x100, //include AGC/SNR/LOCK
	S_GET_RECEIVE,
	S_GET_REMOTE,
	S_GET_HARDDRIVER,
	S_GET_TMS,

	//0x150 Config function
	S_GET_CONFIG = 0x150, //include DelSys/FEC/MOD/ROL/POL
	S_SET_CONFIG,		//FRQ/LNB/SR/MI/SATNO/DEVNO

	N_GET_CONFIG,	//include dhcp/ip/netmask/gateway
	N_SET_CONFIG,	//dns/devno

	N_GET_REMOTE,	//remote ip
	N_SET_REMOTE,

	//0x200 Manager function
	M_GET_USB_MOUNT = 0x200,
	M_SET_USB_MOUNT,
	M_GET_HDD_CONTENT_LIST,
	M_SET_HDD_CONTENT_DEL,
	M_GET_USB_CONTENT_LIST,
	M_SET_USB_CONTENT_DEL,

	M_SET_HDD_CONTENT_EXPORT = 0x300,
	M_GET_HDD_CONTENT_EXPORT,
	M_SET_HDD_FORMAT,
	M_GET_HDD_FORMAT,

} UiProtocalKey;

typedef struct _KL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
public:
	_KL_()
	{
		m_pkgHead = 0x7585;
	}
} KL;