#ifndef MXFAPP_DATATYPE_H
#define MXFAPP_DATATYPE_H

#include <mxflib/mxflib.h>

#include <string>

struct VideoProperty		
{
		mxflib::Uint32			StoredHeight;				
		mxflib::Uint32			StoredWidth;				
		mxflib::Rational		AspectRatio;				
		mxflib::Rational		EditRate;					
		mxflib::Int64			BitRate;					
		
		VideoProperty():StoredHeight(0),
			StoredWidth(0),	
			BitRate(0)
		{
		}
};
	
struct AudioProperty		 
{
		std::string				EncodeType;			
		mxflib::Rational		SampleRate;		
		mxflib::UInt32			BitIndeepness;		
		mxflib::UInt32			Channels;		
		unsigned int			BitRate;		
		
		AudioProperty():EncodeType("Wave"),
			BitIndeepness(0),
			Channels(0),
			BitRate(0)
		{
		}
};
struct SubtileProperty
{

};
struct FrameData
{
	mxflib::DataChunkPtr frame;
	mxflib::UInt64       time;
	int					 reelIndex;
	FrameData():time(0),reelIndex(-1)
	{
	}
	bool empty()const
	{
		return !frame;
	}
	
};
struct EncryptInfo
{
	unsigned char  keyId[16]; 
	unsigned char  contextID[16]; 
	std::string    keyIdStr;
	EncryptInfo()
	{
		memset(keyId,0,16);
		memset(contextID,0,16);
	}
	//
	bool isValid()const 
	{
		EncryptInfo info;
		if(memcmp(keyId ,info.keyId,16) ==0  && memcmp(contextID ,info.contextID,16)==0)
			return false;
		return true;
	}
};



struct PrefaceInfo
{
	std::string InstanceUID;
	std::string GenerationUID;
	std::string LastModifiedDate;
	std::string Version;
	std::string ObjectModelVersion;
	std::string OperationalPattern;
};

struct GenericPackageInfo
{
	std::string InstanceUID;
	std::string PackageUID;
	std::string GenerationUID;
	std::string Name;
	std::string PackageCreationDate;
	std::string PackageModifiedDate;
};

struct IdentifyInfo
{
	std::string InstanceUID;
	std::string GenerationUID;
	std::string CompanyName;
	std::string ProductName;
	std::string VersionString;
	std::string ProductUID;
	std::string ModificationDate;
	std::string ProductVersion;
	std::string Platform;
};

struct TemporalInfo
{
	mxflib::Rational		editUnitRate;
	mxflib::Int64			origin;
	mxflib::UInt64			duration;
	mxflib::UInt64			fileSize;
	TemporalInfo():origin(0),
		duration(0),
		fileSize(0)
	{
	}
};

#endif
