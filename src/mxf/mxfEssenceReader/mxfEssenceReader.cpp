#include "stdafx.h"
#include "mxfEssenceReader.h"
#include <iostream>
using namespace std;
using namespace mxflib;

//#define BUIDLINDEX_INMEMORY

MXFESSENCEREADERDLL_API ITaMxfEssenceReader* createEssenceReader(const std::string& dictPath)
{
	LoadMxfDictionary(dictPath);
	return new TaMxfEssenceReader();
}
MXFESSENCEREADERDLL_API void releaseEssenceReader(ITaMxfEssenceReader* pReader)
{
	delete pReader;
}

TaMxfEssenceReader::TaMxfEssenceReader():
					m_curPartIndex(0),
					m_KLSize(0),m_nextPos(0),m_indexReorder(false)
{
	
	m_File= new MXFFile;
//	SetDictionaryPath( "./" );
//	LoadDictionary("dict.xml");
//	LoadDictionary("DMS_Crypto.xml");
	
	
}
TaMxfEssenceReader::~TaMxfEssenceReader()
{
	close();	
}

bool TaMxfEssenceReader::open(const char* filename)
{
	if(!m_File)
		m_File= new MXFFile;

	if(!parseHeaderMetaData(filename))
		return false;

	if (!m_File->Open(filename, true))
		return false;
	m_File->GetRIP();
	if(m_metaData.essenceType() =="MPEG2")
		m_indexReorder=true;

	if(buildIndexTable() && buildBodyOffsetMap())
	{
		bool r=seekPartition(0);
		//get m_KLSize
		readElement();
		r=seekPartition(0);
		m_nextPos=0;
		return r;
	}
	return false;
}

bool TaMxfEssenceReader::seekPartition(unsigned int index)
{
	if(index >= m_bodyOffsetMap.size())
		return false;
	map<mxflib::UInt64,PartitionInfo>::iterator it=m_bodyOffsetMap.begin();

	unsigned int i=0;
	for(;i<index ;++i,++it);
	
	m_curPartIndex=i;
	m_curPartition = it->second.partition;
	return m_curPartition->StartElements();
	
}
void TaMxfEssenceReader::close()
{
	if(m_File)
	{
		m_File->Close();
		m_File = NULL;
	}
}

mxflib::DataChunkPtr TaMxfEssenceReader::readElement(bool ifGetKlv)
{
	DataChunkPtr dataPtr;
	KLVObjectPtr anElement;
	bool find=false;

	do
	{
		//TODO: if want get KLV , set param = true
		anElement = m_curPartition->NextElementEx(ifGetKlv);		// get KLV
		while(anElement)
		{	 
			GCElementKind kind = anElement->GetGCElementKind();
			//cout<<"element type=" << (int)kind.ElementType <<endl; 

			if( !isNeedEssence(kind))
				anElement = m_curPartition->NextElement();
			else
			{
				find=true;
				break;
			}
		}

		if(!find && !seekPartition(m_curPartIndex+1))
			return dataPtr;
	}while(!find);		

	if(!m_KLSize)		
		m_KLSize=anElement->GetKLSize();
	//anElement->ReadData();
	DataChunk &theEss = anElement->GetData();
	UInt32 granularity = theEss.GetGranularity();

	if(granularity>0 && (theEss.Size%granularity)!=0)
	{
		memset(theEss.Data+theEss.Size, 0, granularity-(theEss.Size%granularity));
	}

	dataPtr= new DataChunk;
	dataPtr->TakeBuffer(theEss,true);
	++m_nextPos;

	return dataPtr;
}

mxflib::DataChunkPtr  TaMxfEssenceReader::readElementEx(std::vector<unsigned char*> bufs, mxflib::UInt32 buflen, int& readSize, mxflib::UInt32& lastLen)
{


	DataChunkPtr dataPtr;
	KLVObjectPtr anElement;
       bool find=false;
       do
        {
			//TODO: if want get KLV , set param = true
			anElement = m_curPartition->NextElementEx2(bufs, buflen,readSize,lastLen);//(true);
			if(readSize > 0)
				break;
			if(anElement)
				break;
		
			if(!seekPartition(m_curPartIndex+1))
					return dataPtr;
        }while(1);		
	
	 if(readSize == 0)
	 {		
		 DataChunk &theEss = anElement->GetData();
		 UInt32 granularity = theEss.GetGranularity();
		 if(granularity > 0 && (theEss.Size%granularity) !=0)
		 {
			    memset(theEss.Data+theEss.Size,0,granularity-(theEss.Size%granularity));
		 }

		dataPtr= new DataChunk;
		dataPtr->TakeBuffer(theEss,true);
	 }
	++m_nextPos;
	return dataPtr;

}
mxflib::DataChunkPtr  TaMxfEssenceReader::readElementEx_SMP(mxflib::SMP_FrameBufferParam& param)
{
	DataChunkPtr dataPtr;
	KLVObjectPtr anElement;
       bool find=false;
       do
        {
			//TODO: if want get KLV , set param = true
			anElement = m_curPartition->NextElementEx_SMP(param);
			if(!param.result.empty())
				break;
			if(anElement)
				break;
		
			if(!seekPartition(m_curPartIndex+1))
					return dataPtr;
        }while(1);		
	
	 if(param.result.empty())
	 {		
		 DataChunk &theEss = anElement->GetData();
		 UInt32 granularity = theEss.GetGranularity();
		 if(granularity > 0 && (theEss.Size%granularity) !=0)
		 {
			    memset(theEss.Data+theEss.Size,0,granularity-(theEss.Size%granularity));
		 }

		dataPtr= new DataChunk;
		dataPtr->TakeBuffer(theEss,true);
	 }
	++m_nextPos;
	return dataPtr;
}

bool TaMxfEssenceReader::isNeedEssence(GCElementKind& kind)
{
		//TODO: sure this's wanted,
		//if(!kind.IsValid) //for encrypted mxf, now is invalid!
		//		return false;
		//if there is only one type of element, return true;
//		if( kind.ElementType != m_essenceType)
//				return false;
		return true;
}

bool TaMxfEssenceReader::seekElement(mxflib::UInt64 editUnitIndex, int& keyFrameOffset)
{
	
	if(m_nextPos == editUnitIndex)
	{
		keyFrameOffset = 0;
		return true;
	
	}
	
	//step0: find essence position by index table
	IndexPosPtr  essencePos;
#ifdef BUIDLINDEX_INMEMORY
	 essencePos= m_indexTable->Lookup(editUnitIndex,0,m_indexReorder);  
#else
	{
		
		bool find = false;
		RIP::iterator it=m_File->FileRIP.begin();
		//cout <<"parttition num =" << m_File->FileRIP.size()<< endl;
		unsigned int i=0;
		for(;it != m_File->FileRIP.end() && !find; ++i ,++it)
		{
			//TODO: compare bodySID , be sure that that is wanted;
			m_File->Seek((*it).second->ByteOffset);
			PartitionPtr partition = m_File->ReadPartition();
			DataChunkPtr indexData = partition->ReadIndexChunk();
			if(indexData && indexData->Size > 0)
			{
				mxflib::IndexTablePtr indexTable = new IndexTable;
				essencePos= indexTable->LookupFromDataChunk(find , indexData, editUnitIndex,0,m_indexReorder);
				if(find)
					break;
			}
		}
		if(!find)
		{
			cout<<"TaMxfEssenceReader::seekElement fail" << endl;
			return false;
		}
	}
#endif		
	
	//mxflib ,for CBR essencePos=editUnitIndex* unitLen, mean that is only V ,not KLV,so do this:
	//modify: these is just for clip wrap,now I don't read mxf of clipwrap;
//	if(m_indexTable->EditUnitByteCount)//CBR
//	{
//		//now suppose there are no  system element and data element
//		essencePos->Location+=(m_KLSize)*editUnitIndex;
//	}
	//step1:find pcout <<"here" << endl;
	//Ret->KeyLocation
	UInt64 loc = m_indexReorder? essencePos->KeyLocation: essencePos->Location;
	map<mxflib::UInt64,PartitionInfo>::iterator it=m_bodyOffsetMap.upper_bound(loc);
	if(it!=m_bodyOffsetMap.begin())
	{
		--it;
		//step2: find pos in partition
	    UInt64 essenceOffset = loc-it->first;
	    m_curPartIndex = distance(m_bodyOffsetMap.begin(),it);
	    m_curPartition = it->second.partition;
	    m_curPartition->StartElements(essenceOffset);
	    
	    m_nextPos=editUnitIndex;
	    keyFrameOffset = essencePos->KeyFrameOffset;
	    return true;
	}
	return false;
}
bool TaMxfEssenceReader::buildIndexTable()
{
#ifndef BUIDLINDEX_INMEMORY
	return true;
#endif		
	m_indexTable = new IndexTable;
	RIP::iterator it=m_File->FileRIP.begin();
	unsigned int i=0;
	for(;it != m_File->FileRIP.end();++i,++it)
	{
		//TODO: compare bodySID , be sure that that is wanted;
		m_File->Seek((*it).second->ByteOffset);
		PartitionPtr partition = m_File->ReadPartition();
	
		MDObjectListPtr Segments = partition->ReadIndex();
		if(! Segments->empty())
		{
			
			MDObjectList::iterator it = Segments->begin();
			while(it != Segments->end())
			{
				m_indexTable->AddSegment(*it);
				++it;
			}
		}
		
	}
	return true;
}
bool TaMxfEssenceReader::buildBodyOffsetMap()
{
	RIP::iterator it=m_File->FileRIP.begin();
	unsigned int i=0;
	for(;it != m_File->FileRIP.end();++i,++it)
	{
		//TODO: compare bodySID , be sure that that are I want;
		//find if there are essence data in this partition?
		m_File->Seek((*it).second->ByteOffset);
		PartitionPtr partition = m_File->ReadPartition();
		if(!partition->SeekEssence())//no essence data;
			continue;
		PartitionInfo info;
		info.bodyOffset=partition->GetInt64(BodyOffset_UL);

		info.partition=partition;
		m_bodyOffsetMap.insert(pair<mxflib::UInt64,PartitionInfo>(info.bodyOffset,info));
		if(info.bodyOffset == 0 && i>0) //footer partition
			break;
	}
	return m_bodyOffsetMap.size()>0;
}

UInt64 TaMxfEssenceReader::duration()
{
	return m_metaData.temporalInfo().duration;
}

Rational TaMxfEssenceReader::editRate()
{
	return m_metaData.temporalInfo().editUnitRate;
}

bool TaMxfEssenceReader::parseHeaderMetaData(const char* filename)
{
	if( m_metaData.parse(filename) )
	{
		//m_metaData.essenceType();
		return true;
	}
	return false;
}
