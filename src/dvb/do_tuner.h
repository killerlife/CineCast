#ifndef _DO_TUNER_H_
#define _DO_TUNER_H_

#include "mytype.h"

#ifdef __cplusplus
extern "C" {
#endif

int clear_tuner(int frontend_fd);

int set_dsec(int frontend_fd,
			 unsigned int pol_vert,
			 int hi_band, int sat_no);

int do_tuner(int frontend_fd,
			 unsigned int ifreq,
			 unsigned int sr,
			 unsigned int delsys,
			 unsigned int fec,
			 unsigned int modulation,
			 unsigned int rolloff,
			 unsigned int inversion,
			 unsigned int mis
			 );

int get_tuner_status(int frontend_fd,
					 uint8 *status,
					 uint16 *agc,
					 uint16 *snr,
					 uint32 *ber,
					 uint32 *unc,
					 uint8 *lock);
#ifdef __cplusplus
};
#endif

#endif