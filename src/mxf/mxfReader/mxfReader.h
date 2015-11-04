#pragma once	
// 2009-05-14  added by lp

#include "IMxfReader.h"
#include "BufferPool.h"

#include <mxfEssenceReader/IEssenceReader.h>
using namespace mxflib;


class CMxfFileReader: public IMxfFileReader
{
public:
	// IMxfFileReader interface 
	virtual int open(TMxfFileItem* items, int count);
	virtual int readAudioFrame(MediaFrame* frame);
	virtual int readVideoFrame(MediaFrame* frame);
	virtual int releaseFrame(MediaFrame* frame);
	virtual int seek(long ms);
	virtual long duration();
	virtual MediaType getMediaType(){ return m_type; };
	virtual const TMediaProperty* getMediaProperty(unsigned int index);
	virtual void close();
	
	CMxfFileReader();
	virtual ~CMxfFileReader();

protected:
	int decrypt(mxflib::DataChunkPtr& data, unsigned char* key);	

	std::vector<bool>	m_EncryptFlags;
	std::vector<ITaMxfEssenceReader*>	m_files;
	std::vector<TMxfFileItem>			m_items;
	std::vector<TMediaProperty>			m_propertys;
	MediaType	m_type;
	unsigned int m_curIndex;
//	unsigned char*	m_buffer;
	CBufferPool		m_bufferPool;
	bool m_bDmsDecrypt;
};

