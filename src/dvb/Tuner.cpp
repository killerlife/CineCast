#include "SatelliteRecv.h"
#include "FilmDataProcess.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	ITuner *pTuner = CreateTuner();
	TUNER_CONF conf;
	if (argc > 1)
	{
		conf.strDevName = argv[1];
	}
	pTuner->SetTunerConf(conf);
	sleep(1);
	while(1)
	{
		TUNER_INFO info = pTuner->GetTunerInfo();
		printf("status = %02X | agc = %3u%% | snr = %3u%% | ber = %d | unc = %d | lock = %02X\n",
			info.nStatus, info.nAGC, info.nSNR, info.nBER, info.nUNC, info.nLock);
		sleep(1); 
	}
	return 0;
}