#include "TaTrackFileReader.h"
#include <iostream>
using namespace std;

CTaTrackFileReader::CTaTrackFileReader(ITaMxfEssenceReader* pReader):m_pReader(pReader),m_begTime(0),m_readOffset(0),m_dur(0)
{
		
}


mxflib::UInt64 CTaTrackFileReader::getEssenceDuration()
{
	return m_essenceDur;
}

void CTaTrackFileReader::setEssenceDuration(mxflib::UInt64 dur)
{
	//m_essencDur: means frame number actully.
	//m_dur: means frame number to play, this value is equal to video frame number.
	m_essenceDur = dur;
	m_dur = dur;
}

void CTaTrackFileReader::setDuration(mxflib::UInt64 dur)
{
	m_dur = dur;
}
mxflib::UInt64 CTaTrackFileReader::getDuration()
{
	return m_dur;
}

void CTaTrackFileReader::setOffset(mxflib::UInt64 offset)
{
	m_readOffset = offset;
	if(!m_pReader)
		return;
	mxflib::UInt64 dur = m_pReader->duration();
	if(m_readOffset >= dur)
    {
		m_readOffset = 0;
		m_dur = dur;
	}
	
	int keyFrameOffset = 0;
	m_pReader->seekElement(m_readOffset, keyFrameOffset);
}

void CTaTrackFileReader::setBegTime(mxflib::UInt64 time)
{
	m_begTime = time;
}

mxflib::UInt64 CTaTrackFileReader::getBegTime()
{
	return m_begTime;
}

bool CTaTrackFileReader::seek(mxflib::UInt64 time)
{
	if(!m_pReader)
		return false;
	int keyFrameOffset = 0;
	if(!m_pReader->seekElement(time + m_readOffset, keyFrameOffset))
	{
		return m_pReader->seekElement(m_pReader->duration(), keyFrameOffset);
	}
	return true;
}

mxflib::UInt64 CTaTrackFileReader::nextPosition()
{
	if(m_pReader)
		return m_pReader->nextPosition() - m_readOffset;
	return 0;
}
	
FrameData CTaTrackFileReader::readFrame()
{
	FrameData  frameData;
	if(m_pReader)
	{
		frameData.frame = m_pReader->readElement();
		if(frameData.frame)
		{
			frameData.time =  (m_pReader->nextPosition() - m_readOffset);
			if(frameData.time > m_dur
			  || frameData.time > m_essenceDur)
			{
				//audio dur > video,so strip it.
				return FrameData();
			}
			frameData.time += m_begTime;
		}
	}
	return frameData;
}

bool CTaTrackFileReader::videoProperty(VideoProperty& property)
{
	if(!m_pReader)
		return false;
	property = m_pReader->metaData().videoProperty();
	return true;
}

bool CTaTrackFileReader::audioProperty(AudioProperty& property)
{
	if(!m_pReader)
		return false;
	property = m_pReader->metaData().audioProperty();
	return true;
}

EncryptInfo CTaTrackFileReader::encryptInfo()
{
	if(!m_pReader)
	  return EncryptInfo();
	const ITaMxfMetaDataParse& vmedataParse = m_pReader->metaData();
	return vmedataParse.encryptInfo();
}

mxflib::Rational CTaTrackFileReader::editRate()
{
	if(m_pReader)
		return m_pReader->editRate();	
	return mxflib::Rational();
}

void CTaTrackFileReader::release()
{
	if(m_pReader)
	{
		m_pReader->close();
		releaseEssenceReader(m_pReader);
	}
	m_pReader = NULL;
}

