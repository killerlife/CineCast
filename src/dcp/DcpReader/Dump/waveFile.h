#ifndef  _WAVEFILE_20051112
#define  _WAVEFILE_20051112

#include  <mxflib/mxflib.h>
#include <string>

struct CWaveFile
{
	mxflib::UInt64 m_dataLen;
	short		   m_channels;
	short		   m_bitRate;
	mxflib::FileHandle	m_waveFile;
    short        m_channelIndex;
	CWaveFile(int channelIndex,int channelNum=6):m_dataLen(0),m_channels(channelNum),m_bitRate(24),m_waveFile(NULL),m_channelIndex(channelIndex)
	{
	}
	~CWaveFile()
	{
		close();
	}
	bool open(std::string& filename);
	void close();

	bool writeData(const char* data,int datalen);

private:
	bool writeWaveHeader();

};

#endif
