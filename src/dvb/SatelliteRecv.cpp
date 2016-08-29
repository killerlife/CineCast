#include "SatelliteRecv.h"
#include "do_tuner.h"
#include "demux.h"
#ifdef LIBDVB
#include <libdvbv5/dvb-demux.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/param.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>
#include <log/Log.h>

extern ILog* gLog;
static uint32 gDebugCmd = 0;

uint32* GetDebugCommand()
{
	return &gDebugCmd;
}

class Tuner : public ITuner
{
public:
	Tuner();
	bool SetTunerConf(TUNER_CONF conf);
	bool Zapto();
	bool Zapto(TUNER_CONF conf);
	TUNER_INFO GetTunerInfo();

	~Tuner();
private:
	TUNER_CONF mConf;
	TUNER_INFO mInfo;
	int mFrontEnd;
	int mod;
	int count;
// 	ILog *pLog;
};

Tuner mTuner;

ITuner* CreateTuner()
{
	return &mTuner;
}

void ReleaseTuner(ITuner* tuner)
{

}

Tuner::Tuner():mFrontEnd(-1), mod(0), count(0)
{
// 	pLog = CreateLog();
}

Tuner::~Tuner()
{
	if(mFrontEnd > 0)
	{
		close(mFrontEnd);
	}
// 	ReleaseLog(pLog);
}

bool Tuner::SetTunerConf(TUNER_CONF conf)
{
// 	pLog->Write(LOG_DVB, "[Tuner] Set tuner Conf");
	syslog(LOG_INFO|LOG_USER, "[Tuner] Set tuner Conf");
	mConf = conf;
	return true;
}

bool Tuner::Zapto()
{
	char str[200];
	sprintf(str, 
		"[Tuner] Zapto: %s, %s, %s, %s, %s, %s, F%d, S%d, H%d, M%d, SN%d",
		mConf.strDevName.c_str(),
		mConf.strDelSys.c_str(),
		mConf.strFec.c_str(),
		mConf.strModulation.c_str(),
		mConf.strPolVert.c_str(),
		mConf.strRollOff.c_str(),
		mConf.nFreq,
		mConf.nSR,
		mConf.nHiBand,
		mConf.nMis,
		mConf.nSatNo);
	if (gLog)
	{
		gLog->Write(LOG_DVB, str);
	}
	if(mFrontEnd < 0)
	{
		if ((mFrontEnd = open (mConf.strDevName.c_str(), O_RDWR | O_NONBLOCK)) < 0)
		{
			sprintf(str, "[Tuner] Failed to open '%s': %d", mConf.strDevName.c_str(), errno);
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
	}

	clear_tuner(mFrontEnd);
	
	int polvet = 1;
	int i = 0;
	while(1)
	{
		if(polvert_values[i].user_value == -1)
		{
			sprintf(str, "[Tuner] Unsupport pol_vert mode: %s", mConf.strPolVert.c_str());
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
		if(polvert_values[i].user_string == mConf.strPolVert)
		{
			polvet = polvert_values[i].driver_value;
			DPRINTF("pol: %s\n", polvert_values[i].user_string.c_str());
			break;
		}
		i++;
	}

	set_dsec(mFrontEnd, 
		polvet, 
		(mConf.nFreq - mConf.nHiBand)?1:0,
		mConf.nSatNo);

	uint32 devsys;
	i = 0;
	while(1)
	{
		if(system_values[i].user_value == -1)
		{
			sprintf(str, "[Tuner] Unsupport system mode: %s", mConf.strDelSys.c_str());
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
		if(system_values[i].user_string == mConf.strDelSys)
		{
			devsys = system_values[i].driver_value;
			DPRINTF("system: %s\n", system_values[i].user_string.c_str());
			break;
		}
		i++;
	}

	uint32 fec;
	i = 0;
	while(1)
	{
		if(coderate_values[i].user_value == -1)
		{
			sprintf(str, "[Tuner] Unsupport fec mode: %s", mConf.strFec.c_str());
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
		if(coderate_values[i].user_string == mConf.strFec)
		{
			fec = coderate_values[i].driver_value;
			DPRINTF("fec coderate: %s\n", coderate_values[i].user_string.c_str());
			break;
		}
		i++;
	}

	uint32 modulation;
	i = 0;
	while(1)
	{
		if(modulation_values[i].user_value == -1)
		{
			sprintf(str, "[Tuner] Unsupport modulation: %s %d", mConf.strModulation.c_str(), i);
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
		if(modulation_values[i].user_string == mConf.strModulation)
		{
			modulation = modulation_values[i].driver_value;
			DPRINTF("mod: %s\n", modulation_values[i].user_string.c_str());
			break;
		}
		i++;
	}

	uint32 rolloff;
	i = 0;
	while(1)
	{
		if(rolloff_values[i].user_value == -1)
		{
			sprintf(str, "[Tuner] Unsupport rolloff: %s", mConf.strRollOff.c_str());
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return false;
		}
		if(rolloff_values[i].user_string == mConf.strRollOff)
		{
			rolloff = rolloff_values[i].driver_value;
			DPRINTF("rolloff: %s\n", rolloff_values[i].user_string.c_str());
			break;
		}
		i++;
	}

	if(do_tuner(mFrontEnd, 
		mConf.nFreq - mConf.nHiBand,
		mConf.nSR,
		devsys, 
		fec,
		modulation,
		rolloff,
		INVERSION_AUTO,
		mConf.nMis) != 0)
	{
		DPRINTF("[Tuner] Zapto faile\n");
		if (gLog)
		{
			gLog->Write(LOG_ERROR, "[Tuner] Zapto faile.");
		}
		return false;
	}
	DPRINTF("Set Tuner success\n");
	return true;
}

bool Tuner::Zapto(TUNER_CONF conf)
{
	SetTunerConf(conf);
	if(conf.strModulation == "8PSK")
	{
		mod = 0;
	}
	else if(conf.strModulation == "16APSK")
	{
		mod = 1;
	}
	else if(conf.strModulation == "32APSK")
	{
		mod = 2;
	}
	return Zapto();
}

TUNER_INFO Tuner::GetTunerInfo()
{
	if(mFrontEnd < 0)
	{
		if ((mFrontEnd = open (mConf.strDevName.c_str(), O_RDWR | O_NONBLOCK)) < 0)
		{
			char str[200];
			sprintf(str, "[Tuner] Failed to open '%s': %d", mConf.strDevName.c_str(), errno);
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
			return mInfo;
		}
	}
	get_tuner_status(mFrontEnd, 
		&mInfo.nStatus,
		&mInfo.nAGC, 
		&mInfo.nSNR, 
		&mInfo.nBER, 
		&mInfo.nUNC, 
		&mInfo.nLock);
#if 0	
	if(mInfo.nLock == 0)
	{
		if(count < 3)
		{
			count++;
		}
		else
		{
			if(mod == 0)
			{
				mod = 1;
				mConf.strModulation = "16APSK";
				printf("switch 16APSK\n");
				Zapto();
			}
			else if(mod == 1)
			{
				mod = 2;
				mConf.strModulation = "32APSK";
				printf("switch 32APSK\n");
				Zapto();
			}
			else if(mod == 2)
			{
				mod = 0;
				mConf.strModulation = "8PSK";
				printf("switch 8PSK\n");
				Zapto();
			}
			count = 0;
		}
	}
#endif
	return mInfo;
}

Filter::Filter():fd(-1), bLogFd(false)
{
	//pLog = CreateLog();
}

Filter::~Filter()
{
	Stop();
	//ReleaseLog(pLog);
}

bool Filter::SetStrDevName(std::string strDevName)
{
	if (fd > 0)
	{
		ioctl(fd, DMX_STOP);
		close(fd);
		fd = -1;
	}

	bLogFd = false;

	int flags = O_RDWR;
#ifdef NON_BLOCK
	flags |= O_NONBLOCK;
#endif

	if ((fd = open(strDevName.c_str(), flags)) <0)
	{
		char str[200];
		sprintf(str, "[Filter] Open device %s error: %d", strDevName.c_str(), errno);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}

#ifdef USE_POLL
		m_poll.fd = -1;
#endif // USE_POLL

		return false;
	}

#ifdef USE_POLL
	m_poll.fd = fd;
	m_poll.events = POLLIN|POLLPRI|POLLERR;
	//m_poll.revents = 0; 
#endif // USE_POLL

	return true;
}

bool Filter::SetFilterID(uint16 pid, uint16 tid)
{
	nPid = pid;
	nTid = tid;
	char str[200];
#ifdef LIBDVB
	uint8 filter[18];
	uint8 mask[18];
	uint8 mode[18];
	memset(filter, 0, 18);
	memset(mask, 0, 18);
	memset(mode, 0, 18);
	filter[0] = 0x80;
	mask[0] = 0xff;
	if(dvb_set_section_filter(fd,
		pid,
		1,
		filter,
		mask,
		mode,
		DMX_IMMEDIATE_START) == -1)
	{
		sprintf(str, "[Filter] dvb_set_section_filter failed, pid = 0x%x tid = 0x%x fd 0x%x", nPid, nTid, fd);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}

		ioctl(fd, DMX_STOP);
		close(fd);
		fd = -1;

#ifdef USE_POLL
		m_poll.fd = -1;
#endif // USE_POLL

		return false;
	}
#else
	struct dmx_sct_filter_params df;
	memset(&df.filter, 0, sizeof(struct dmx_filter));

	df.pid = nPid;
	df.filter.filter[0] = nTid;
	df.filter.mask[0] = 0xff;
	df.timeout = 0;
	df.flags = DMX_IMMEDIATE_START;//|DMX_CHECK_CRC;

	if(ioctl(fd, DMX_SET_BUFFER_SIZE, 2000*4096) == -1)
	{
		sprintf(str, "[Filter] DMX_SET_BUFFER_SIZE failed, pid = 0x%x tid = 0x%x fd = 0x%d", nPid, nTid, fd);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}
	}

	if(ioctl(fd, DMX_SET_FILTER, &df) == -1)
	{
		sprintf(str, "[Filter] DMX_SET_FILTER failed, pid = 0x%x tid = 0x%x fd 0x%x", nPid, nTid, fd);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}

		ioctl(fd, DMX_STOP);
		close(fd);
		fd = -1;

#ifdef USE_POLL
		m_poll.fd = -1;
#endif // USE_POLL

		return false;
	}
#endif
	DPRINTF("[Filter] DMX_SET_FILTER start pid = 0x%x tid = 0x%x fd 0x%x\n", nPid, nTid, fd);

	return true;
}

bool Filter::ReadFilter(uint8 *buf, uint16& count)
{
	if(fd <= 0)
	{
		DPRINTF("[Filter] invaild fd\n");
		if (gLog && (bLogFd == false))
		{
			bLogFd = true;
			gLog->Write(LOG_ERROR, "[Filter] ReadFilter: invaild fd");
		}
		return false;
	}

#ifdef USE_POLL
	m_poll.fd = fd;
	m_poll.events = POLLIN|POLLPRI|POLLERR;
	//m_poll.revents = 0; 

	//for poll function, poll for 1 second timeout
	int n = poll(&m_poll, 1, 300);
	if(n == -1)
		return false;
	if(!(m_poll.revents & (POLLIN|POLLPRI)))
		return false;
	//============================================
#endif // USE_POLL

	int section_length, cnt;
	if (((cnt = read(fd, buf, (count))) < 0) && errno == EOVERFLOW)
		cnt = read(fd, buf,(count));
	if (cnt < 0)
	{
		//DPRINTF("[Filter] ReadFilter : section read error\n");
		return false;
	}
	if (cnt < 4)
	{
		return false;
	}
	section_length = getBits(buf, 12, 12);

	if (cnt != section_length + 3)
	{
		//DPRINTF("[Filter] Ignoring section, read %d, while section length + 3 = %d\n", cnt, section_length + 3);
		return false;
	}

	//DPRINTF("[Filter] Read data \n");
	count = cnt;
	return true;
}

bool Filter::Stop()
{
	//pLog->Write(LOG_DVB, "[Filter] Stop filter");
	//DPRINTF("[Filter] Stop filter");
	if (fd > 0)
	{
		ioctl(fd, DMX_STOP);
		close(fd);
		fd = -1;
	}
	return true;
}

bool Filter::SetPesFilterID(uint16 pid, uint16 tid)
{
	nPid = pid;
	nTid = tid;
	char str[200];

	struct dmx_pes_filter_params df;

	df.pid = nPid;
	df.input = DMX_IN_FRONTEND;
	df.output = DMX_OUT_TAP;
	df.pes_type = DMX_PES_OTHER;
	df.flags = DMX_IMMEDIATE_START;//|DMX_CHECK_CRC;

	if(ioctl(fd, DMX_SET_PES_FILTER, &df) == -1)
	{
		sprintf(str, "[Filter] DMX_SET_PES_FILTER failed, pid = 0x%x tid = 0x%x fd 0x%x", nPid, nTid, fd);
		if (gLog)
		{
			gLog->Write(LOG_ERROR, str);
		}

		ioctl(fd, DMX_STOP);
		close(fd);
		fd = -1;

#ifdef USE_POLL
		m_poll.fd = -1;
#endif // USE_POLL

		return false;
	}
	DPRINTF("[Filter] DMX_SET_PES_FILTER start pid = 0x%x tid = 0x%x fd 0x%x\n", nPid, nTid, fd);

	return true;
}

