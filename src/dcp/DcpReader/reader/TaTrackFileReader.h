#ifndef MXFAPP_TATRACKFILE_READER_H
#define MXFAPP_TATRACKFILE_READER_H

#include <mxfEssenceReader/mxfEssenceReader.h>
#include <string>
#include <list>

class  CTaTrackFileReader
{
	ITaMxfEssenceReader* m_pReader;
	mxflib::UInt64       m_begTime,m_readOffset,m_dur,m_essenceDur;
public:		
	CTaTrackFileReader(ITaMxfEssenceReader* pReader);

	void setEssenceDuration(mxflib::UInt64 dur);
	mxflib::UInt64 getEssenceDuration();

	void setDuration(mxflib::UInt64 dur);
	mxflib::UInt64 getDuration();

	void setOffset(mxflib::UInt64 offset);

	void setBegTime(mxflib::UInt64 time);
    mxflib::UInt64 getBegTime();

	bool seek(mxflib::UInt64 time);

	mxflib::UInt64 nextPosition();

	FrameData readFrame();

	bool videoProperty(VideoProperty& property);
	bool audioProperty(AudioProperty& property);
	EncryptInfo encryptInfo();

    mxflib::Rational editRate();

	void release();
	
};

#endif

