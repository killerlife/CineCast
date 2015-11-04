#ifndef MXFAPP_ITAESSENCEREADER_H
#define MXFAPP_ITAESSENCEREADER_H

#include "exportdef.h"
#include "IMetaDataParse.h"


class MXFESSENCEREADERDLL_API ITaMxfEssenceReader
{
public:
	virtual ~ITaMxfEssenceReader(){};
	
	virtual bool open(const char* filename)=0;
	virtual void close()=0;
	virtual mxflib::DataChunkPtr readElement(bool ifGetKlv = false)=0;
	virtual mxflib::DataChunkPtr readElementEx(std::vector<unsigned char*> bufs, mxflib::UInt32 buflen, int& readSize, mxflib::UInt32& lastLen) = 0;
	virtual mxflib::DataChunkPtr readElementEx_SMP(mxflib::SMP_FrameBufferParam& param) = 0;

	virtual bool 		     seekElement(mxflib::UInt64 editUnitIndex, int& keyFrameOffset)=0;	
	virtual mxflib::UInt64	    nextPosition()=0;
	virtual mxflib::UInt64       duration()=0;
	virtual mxflib::Rational     editRate()=0;
	virtual const ITaMxfMetaDataParse&  metaData()=0;
};

MXFESSENCEREADERDLL_API ITaMxfEssenceReader* createEssenceReader(const std::string& dictPath="");
MXFESSENCEREADERDLL_API void releaseEssenceReader(ITaMxfEssenceReader*);
#endif

