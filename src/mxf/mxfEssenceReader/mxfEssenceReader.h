#ifndef MXFAPP_TAESSENCEREADER_H
#define MXFAPP_TAESSENCEREADER_H
#include "IEssenceReader.h"
#include "mxfMetaDataParse.h"
#include <map>

class TaMxfEssenceReader : public ITaMxfEssenceReader
{
	struct PartitionInfo
	{
		mxflib::UInt64 bodySID;
		mxflib::UInt64 indexSID;
		mxflib::UInt64 bodyOffset;
		mxflib::PartitionPtr	partition;
	};
public:
	TaMxfEssenceReader();
	virtual ~TaMxfEssenceReader();
	
	bool open(const char* filename);
	void close();
	mxflib::DataChunkPtr readElement(bool ifGetKlv = false);
	mxflib::DataChunkPtr readElementEx(std::vector<unsigned char*> bufs, mxflib::UInt32 buflen, int& readSize, mxflib::UInt32& lastLen);
	mxflib::DataChunkPtr readElementEx_SMP(mxflib::SMP_FrameBufferParam& param);

	//editUnitIndex: [0,duration)
	bool 				 seekElement(mxflib::UInt64 editUnitIndex, int& keyFrameOffset);	
	mxflib::UInt64		 nextPosition()
	{
		return m_nextPos;
	}
	mxflib::UInt64       duration();
	mxflib::Rational     editRate();
	const ITaMxfMetaDataParse&  metaData()
	{
		return m_metaData;
	}


private:
	bool parseHeaderMetaData(const char* filename);
		
	bool buildIndexTable();
	bool buildBodyOffsetMap();
	bool seekPartition(unsigned int index);
	inline bool isNeedEssence(mxflib::GCElementKind& kind);


private:
	mxflib::MXFFilePtr m_File;
	
	unsigned int    		m_curPartIndex;
	mxflib::PartitionPtr    m_curPartition;
	
	std::map<mxflib::UInt64,PartitionInfo> m_bodyOffsetMap;
	mxflib::IndexTablePtr 	m_indexTable;
	unsigned int			m_KLSize;

	TaMxfMetaDataParse		m_metaData;
	mxflib::UInt64			m_nextPos;
	bool					    m_indexReorder;
};

#endif

