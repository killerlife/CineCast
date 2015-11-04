#include "do_tuner.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <glob.h>
#include <ctype.h>
#include <syslog.h>

#include <linux/dvb/version.h>
#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h> 
#include <linux/dvb/audio.h>

#if DVB_API_VERSION < 5
#error scan-s2 requires Linux DVB driver API version 5.0!
#endif

int clear_tuner(int frontend_fd)
{
	struct dtv_property p_clear[] = {
		{ .cmd = DTV_CLEAR },
	};

	struct dtv_properties cmdseq_clear = {
		.num = 1,
		.props = p_clear
	};

	//clear tuner
	if((ioctl(frontend_fd, FE_SET_PROPERTY, &cmdseq_clear)) == -1)
	{
		perror("FE_SET_PROPERTY DTV_CLEAR failed");
		return -1;
	}
	return 0;
}

int do_tuner(int frontend_fd,
			 unsigned int ifreq,
			 unsigned int sr,
			 unsigned int delsys,
			 unsigned int fec,
			 unsigned int modulation,
			 unsigned int rolloff,
			 unsigned int inversion,
			 unsigned int mis)
{
	syslog(LOG_INFO|LOG_USER, "do_tuner: ifreq = %d sr = %d delsys = %d fec = %d mod = %d roll = %d inv = %d\n",
		ifreq, sr, delsys, fec, modulation, rolloff, inversion);
	if(frontend_fd <=0)
	{
	    perror("Invaild frontend fd");
	    return -1;
	}

	DPRINTF("Enter do_tuner\n");
	struct dvb_frontend_event ev;
	struct dtv_property p_tune[] = {
		{.cmd = DTV_DELIVERY_SYSTEM, .u.data = delsys },
		{.cmd = DTV_FREQUENCY, .u.data = ifreq},
		{.cmd = DTV_MODULATION, .u.data = modulation},
		{.cmd = DTV_SYMBOL_RATE, .u.data = sr},
		{.cmd = DTV_INNER_FEC, .u.data = fec},
		{.cmd = DTV_INVERSION, .u.data = inversion},
		{.cmd = DTV_ROLLOFF, .u.data = rolloff},
		{.cmd = DTV_PILOT, .u.data = PILOT_AUTO},
		{.cmd = DTV_DVBS2_MIS_ID, .u.data = mis },
		{.cmd = DTV_TUNE},
	};
	struct dtv_properties cmdseq_tune = {
		.num = 10,
		.props = p_tune
	};
	while (1)
	{
		if(ioctl(frontend_fd, FE_GET_EVENT, &ev) == -1)
			break;
	}

	if(delsys != SYS_DVBS2)
	{
	    perror("Not DVB-S2");
	    return -1;
	}

	if(ioctl(frontend_fd, FE_SET_PROPERTY, &cmdseq_tune) == -1)
	{
		perror("FE_SET_PROPERTY TUNE failed");
		return -1;
	}
#if 0
	do 
	{
		ioctl(frontend_fd, FE_GET_EVENT, &ev);
	} while (ev.status != 0);

	int i;
	for(i = 0; i < 10; i++)
	{
		usleep(200000);
		if (ioctl(frontend_fd, FE_GET_EVENT, &ev) == -1)
		{
			ev.status = (fe_status_t)0;
		}
		printf(">>> tuning status == 0x%02X\n", ev.status);

		if(ev.status & FE_HAS_LOCK)
		{
			struct dtv_property p[] = {
				{ .cmd = DTV_DELIVERY_SYSTEM },
				{ .cmd = DTV_MODULATION },
				{ .cmd = DTV_INNER_FEC },
				{ .cmd = DTV_INVERSION },
				{ .cmd = DTV_ROLLOFF },
			};

			struct dtv_properties cmdseq = {
				.num = 5,
				.props = p
			};

			// get the actual parameters from the driver for that channel
			if ((ioctl(frontend_fd, FE_GET_PROPERTY, &cmdseq)) == -1) {
				perror("FE_GET_PROPERTY failed");
				return -1;
			}
			return 0;
		}
	}
	printf(">>> tuning failed!!!\n");
#endif
	return 0;
}

int get_tuner_status(int frontend_fd,
					 uint8 *status,
					 uint16 *agc,
					 uint16 *snr,
					 uint32 *ber,
					 uint32 *unc,
					 uint8 *lock)
{
	fe_status_t status1;
	if (ioctl(frontend_fd, FE_READ_STATUS, &status1) == -1)
	{
		perror("FE_READ_STATUS failed");
	}
	if(ioctl(frontend_fd, FE_READ_SIGNAL_STRENGTH, agc) == -1)
		*(agc) = -2;
	if(ioctl(frontend_fd, FE_READ_SNR, snr) == -1)
		*(snr) = -2;
	if(ioctl(frontend_fd, FE_READ_BER, ber) == -1)
		*(ber) = -2;
	if(ioctl(frontend_fd,  FE_READ_UNCORRECTED_BLOCKS, unc) == -1)
		*snr = -2;
	*(status) = status1;
	*(agc) = ((*(agc)) * 100 / 0xffff);
	*(snr) = ((*(snr)) * 100 / 0xffff);
	if (status1 & FE_HAS_LOCK)
	{
		*(lock) = 1;
	}
	else
		*(lock) = 0;
	return *(lock);
}

struct diseqc_cmd {
	struct dvb_diseqc_master_cmd cmd;
	unsigned int wait;
}; 

int set_dsec(int frontend_fd,
			 unsigned int pol_vert,
			 int hi_band, int sat_no)
{
	struct diseqc_cmd cmd =
				{ {{0xe0, 0x10, 0x38, 0xf0, 0x00, 0x00}, 4}, 0 }; 

	cmd.cmd.msg[3] = 
		0xf0 | (((sat_no * 4) & 0x0f) | (hi_band ? 1:0) | (pol_vert? 0:2));
		//0xf0 | (((sat_no * 4) & 0x0f) | (0 ? 1:0) | (pol_vert? 0:2));
		//0xf0 | ((hi_band ? 1:0) | (pol_vert? 0:2));

	fe_sec_voltage_t v = pol_vert ? SEC_VOLTAGE_13 : SEC_VOLTAGE_18;
	//fe_sec_tone_mode_t t = 0 ? SEC_TONE_ON : SEC_TONE_OFF;
	fe_sec_tone_mode_t t = hi_band ? SEC_TONE_ON : SEC_TONE_OFF;
	fe_sec_mini_cmd_t b = (sat_no / 4) % 2 ? SEC_MINI_B : SEC_MINI_A;
	//fe_sec_mini_cmd_t b = SEC_MINI_B;

	syslog(LOG_INFO|LOG_USER,
		"set_dec: pol_vert = %d hi_band = %d\n",
		pol_vert,
		hi_band);

	if (ioctl(frontend_fd, FE_SET_TONE, SEC_TONE_OFF) == -1)
		perror("FE_SET_TONE failed");
	if (ioctl(frontend_fd, FE_SET_VOLTAGE, v) == -1)
		perror("FE_SET_VOLTAGE failed");
	usleep(15 * 1000);
	if (ioctl(frontend_fd, FE_DISEQC_SEND_MASTER_CMD, &cmd.cmd) == -1)
		perror("FE_DISEQC_SEND_MASTER_CMD failed");
	usleep(cmd.wait * 1000);
	usleep(15 * 1000);
	if (ioctl(frontend_fd, FE_DISEQC_SEND_BURST, b) == -1)
		perror("FE_DISEQC_SEND_BURST failed");
	usleep(15 * 1000);
	if (ioctl(frontend_fd, FE_SET_TONE, t) == -1)
		perror("FE_SET_TONE failed");
	return 0;
}

