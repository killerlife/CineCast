#include "waveFile.h"
#include <iostream>

using namespace mxflib;

bool CWaveFile::open(std::string& filename)
{
	m_waveFile = FileOpenNew(filename.c_str());
	
	if(!FileValid(m_waveFile))
	{
		std::cout<<"CWaveFile::open : create wave file:"<< filename <<" fail!"<< std::endl;
			return false;
	}
	return writeWaveHeader();
	
}
void CWaveFile::close()
{
	if(FileValid(m_waveFile))
	{
		
		writeWaveHeader();
		FileClose(m_waveFile);
		m_waveFile=NULL;
	}
}
bool CWaveFile::writeWaveHeader()
{
		
		const int hLength = 44;				// wave header length
		unsigned char header[hLength] = {0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00,
			0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 
			0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 
			0x80, 0xBB, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 
			0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 
			0x00, 0x00, 0x00, 0x00};
		unsigned int index = 20;
		*(unsigned long*)(header + 4) = m_dataLen+hLength-8;
		index += 2;
		*(short*)(header + index) = m_channels;
		index += 2;
		*(int*)(header + index) = 48000;
		index += 4;
		*(int*)(header + index) = 48000* 2 * m_channels;
		index += 4;
		*(short*)(header + index) = m_bitRate * m_channels/8;
		index += 2;
		*(short*)(header + index) = m_bitRate;
		
		*(int*)(header + 40) = m_dataLen;	
		FileSeek(m_waveFile,0);
		UInt64 written =FileWrite(m_waveFile,(unsigned char*)&header, hLength);
	    return written ==(UInt64)hLength;
}
bool CWaveFile::writeData(const char* data,int datalen)
{
	UInt64 written =FileWrite(m_waveFile,(unsigned char*)data, datalen);
	m_dataLen+=written;  
	return written == (UInt64)datalen;
}
