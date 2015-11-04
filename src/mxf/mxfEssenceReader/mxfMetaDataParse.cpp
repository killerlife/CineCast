#include "stdafx.h"
#include "mxfMetaDataParse.h"
#include "mxfprint.h"
#include <iostream>
using namespace mxflib;
using namespace std;

#ifdef MPEG3D
#include <KM_fileio.h>
#include <KM_prng.h>
#include <PCMParserList.h>
#include <WavFileWriter.h>
#include <MXF.h>
#include <Metadata.h>
#endif

bool LoadMxfDictionary(const std::string& path)
{
	static bool loadDict=false;
	if(!loadDict)
	{
		static TaDumpDebugInfo dumpInfo;
		setDumpDebugInfo(&dumpInfo);
		if(!path.empty())
			SetDictionaryPath( path.c_str() );
		else
		{
			// liang modified 20081219 14:16 ->
			std::string currentpath = ".";
			currentpath += DIR_SEPARATOR;
			SetDictionaryPath(currentpath);
			// liang modified 20081219 14:16 <-
		}
		if( 0 == LoadDictionary("dict.xml") && 0==LoadDictionary("DMS_Crypto.xml") )
		{
			loadDict=true;
		}
	}
	return loadDict;

}
MXFESSENCEREADERDLL_API ITaMxfMetaDataParse* createMetaDataParse(const std::string& dictPath)
{
	LoadMxfDictionary(dictPath);
	return new TaMxfMetaDataParse();
}
MXFESSENCEREADERDLL_API void releaseMetaDataParse(ITaMxfMetaDataParse* parser)
{
	delete parser;
}

mxflib::Rational TaMxfMetaDataParse::getRational(const std::string& value)
{
	Rational rational;
	std::string str = value;
	rational.Numerator = atoi(str.c_str());
	str.erase(str.begin(), str.begin() + str.find('/') + 1);
	rational.Denominator = atoi(str.c_str());
	return rational;
}

const std::string TaMxfMetaDataParse::getOperationalPattern(const std::string OP)
{
	if(strncmp((OP.c_str() + 28), "0101", 4))
		return "OP-1a";
	if(strncmp((OP.c_str() + 28), "10", 2))
		return "OP-Atom";
	return "";
}


bool TaMxfMetaDataParse::getTemporalInfo()
{
	PackageList::iterator package_ite = m_data->Packages.begin();
	for(; package_ite != m_data->Packages.end(); ++package_ite)
	{
		if( (*package_ite)->IsA(MaterialPackage_UL)) 
			break;
	}
	if(package_ite == m_data->Packages.end())
		return false;
	
	TrackList::iterator ite_tracks = (*package_ite)->Tracks.begin();
	for(; ite_tracks != (*package_ite)->Tracks.end(); ++ite_tracks)
	{
		//if((*ite_tracks)->IsTimecodeTrack())
		//now , DCI use op-atom,and there's no timecodetrack!,so change to:
		if((*ite_tracks)->IsEssenceTrack())
		{
			m_temporalInfo.editUnitRate = getRational((*ite_tracks)->GetString(EditRate_UL));
			m_temporalInfo.origin = (*ite_tracks)->GetInt64(Origin_UL);
			
			MDObjectPtr Sequence = (*ite_tracks)[Sequence_UL];
			if(Sequence) 
				Sequence = Sequence->GetLink();
			if(!Sequence)
				return false;
			m_temporalInfo.duration = Sequence->GetInt64(Duration_UL);
			return true;
		}
	}
	return false;
}

bool TaMxfMetaDataParse::getPrefaceInfo(PartitionPtr MasterPartition)
{
	MDObjectList::iterator ite_master = MasterPartition->TopLevelMetadata.begin();
	for(; ite_master != MasterPartition->TopLevelMetadata.end(); ite_master++)
	{
		if((*ite_master)->IsA(Preface_UL))
			break;
	}
	
	if(ite_master == MasterPartition->TopLevelMetadata.end())
		return false;
	
	m_prefaceInfo.Version = (*ite_master)->GetString(Version_UL);
	m_prefaceInfo.InstanceUID = (*ite_master)->GetString(InstanceUID_UL);
	m_prefaceInfo.GenerationUID = (*ite_master)->GetString(GenerationUID_UL);
	m_prefaceInfo.LastModifiedDate = (*ite_master)->GetString(LastModifiedDate_UL);
	m_prefaceInfo.ObjectModelVersion = (*ite_master)->GetString(ObjectModelVersion_UL);
	m_prefaceInfo.OperationalPattern = getOperationalPattern((*ite_master)->GetString(OperationalPattern_UL));
	return true;
}

bool TaMxfMetaDataParse::getIdentifInfo(PartitionPtr MasterPartition)
{
	IdentifyInfo identifyInfo;
	MDObjectList::iterator ite_master = MasterPartition->TopLevelMetadata.begin();
	for(; ite_master != MasterPartition->TopLevelMetadata.end(); ite_master++)
	{
		if((*ite_master)->IsA(Preface_UL)) 
			break;
	}
	
	if(ite_master == MasterPartition->TopLevelMetadata.end())
		return false;
	
	MDObjectPtr IdentifyList = (*ite_master)[Identifications_UL];
	if(!IdentifyList)
		return false;
	MDObjectULList::iterator ite_identify = IdentifyList->begin();
	for(; ite_identify != IdentifyList->end(); ++ite_identify)
	{
		MDObjectPtr object = (*ite_identify).second->GetLink();
		if(object)
		{
			identifyInfo.Platform =  object->GetString(Platform_UL);
			identifyInfo.ProductUID =  object->GetString(ProductUID_UL);
			identifyInfo.InstanceUID = object->GetString(InstanceUID_UL);
			identifyInfo.CompanyName =  object->GetString(CompanyName_UL);
			identifyInfo.ProductName =  object->GetString(ProductName_UL);
			identifyInfo.VersionString =  object->GetString(VersionString_UL);
			identifyInfo.ProductVersion =  object->GetString(ProductVersion_UL);
			identifyInfo.GenerationUID =  object->GetString(ThisGenerationUID_UL);
			identifyInfo.ModificationDate =  object->GetString(ModificationDate_UL);
			m_identifyVector.push_back(identifyInfo);
		}
	}
	return true;
}

bool TaMxfMetaDataParse::getGenericPackageInfo()
{
	PackageList::iterator package_ite = m_data->Packages.begin();
	for(; package_ite != m_data->Packages.end(); ++package_ite)
	{
		if((*package_ite)->IsA(MaterialPackage_UL))
			break;
	}
	
	if(package_ite == m_data->Packages.end())
		return false;
	
	m_genericInfo.Name = (*package_ite)->GetString(Name_UL);
	m_genericInfo.PackageUID = (*package_ite)->GetString(PackageUID_UL);
	m_genericInfo.InstanceUID = (*package_ite)->GetString(InstanceUID_UL);
	m_genericInfo.GenerationUID = (*package_ite)->GetString(GenerationUID_UL);
	m_genericInfo.PackageCreationDate = (*package_ite)->GetString(PackageCreationDate_UL);
	m_genericInfo.PackageModifiedDate = (*package_ite)->GetString(PackageModifiedDate_UL);
	return true;
}

bool TaMxfMetaDataParse::getEncrptInfo()
{
	PackageList::iterator Package_it = m_data->Packages.begin();
	while(Package_it != m_data->Packages.end())
	{
		PackagePtr ThisPackage = *Package_it;  // Search for the crypto context
		TrackList::iterator it = ThisPackage->Tracks.begin();
		while(it!= ThisPackage->Tracks.end())
		{
			ComponentList::iterator comp_it = (*it)->Components.begin();
			while(comp_it!= (*it)->Components.end())
			{
				if((*comp_it)->IsA(DMSegment_UL))  // Found a DM segment
				{
					MDObjectPtr Framework = (*comp_it)->Child(DMFramework_UL);
					if(Framework) Framework = Framework->GetLink();
			
					if(Framework && Framework->IsA(CryptographicFramework_UL))	// Found a Crypto Framework
					{
						MDObjectPtr Context = Framework->Child(ContextSR_UL);
						if(Context) Context = Context->GetLink();
						if(Context)	 // Read the key ID
						{
							DataChunkPtr Key = Context[CryptographicKeyID_UL]->PutData();
							memcpy(m_encryptInfo.keyId,Key->Data,Key->Size);
							m_encryptInfo.keyIdStr = Context[CryptographicKeyID_UL]->GetString();
							DataChunkPtr ContextID = Context[ContextID_UL]->PutData();
							memcpy(m_encryptInfo.contextID, ContextID->Data, ContextID->Size);
							m_encrpt = true;
							MDObjectPtr MicIDPtr = Context[MICAlgorithm_UL];
							if(MicIDPtr)
							{
								DataChunkPtr MicID = MicIDPtr->PutData();
								if(memcmp(HMACAlgorithmSHA1128_UL_Data,MicID->Data,MicID->Size) == 0)
									m_hmac = true;
							}
							return true;
						}
					}
				}
				comp_it++;
			}
			it++;
		}
		Package_it++;
	}
	return true;
}

bool TaMxfMetaDataParse::isVideo(MDObjectPtr Descriptor)
{
	if(Descriptor->IsA(MPEG2VideoDescriptor_UL))
	{
		m_essenceType = "MPEG2";
		m_videoProperty.BitRate = Descriptor->GetUInt(BitRate_UL);
	}
	else if(Descriptor->IsA(MPEG2TSDescriptor_UL))
	{
		m_essenceType = "MPEG2TS";
		m_videoProperty.BitRate = Descriptor->GetUInt(BitRate_UL);
	}
	else if(Descriptor->IsA(RGBAEssenceDescriptor_UL) || Descriptor->IsA(CDCIEssenceDescriptor_UL))
	{
		MDObjectPtr SubDescriptor = (*Descriptor)[SubDescriptor_UL];
		if(!SubDescriptor)
			return false;
		SubDescriptor = SubDescriptor->GetLink();
		if(SubDescriptor->IsA(JPEG2000PictureSubDescriptor_UL))
			{
				m_essenceType = "JPEG2000";
				//TODO: get PictureEssenceCoding value: that indicate 2k or 4k (ref:s422m-5.5)
				//cout <<"picture essence coding ul=" << Descriptor->GetString(PictureEssenceCoding_UL)<<endl;
			}
	}
	else
		return false;
		
	
	m_videoProperty.StoredWidth = Descriptor->GetUInt(StoredWidth_UL);
	m_videoProperty.StoredHeight = Descriptor->GetUInt(StoredHeight_UL);
	m_videoProperty.EditRate = getRational(Descriptor->GetString(SampleRate_UL));
	m_videoProperty.AspectRatio = getRational(Descriptor->GetString(AspectRatio_UL));
	return true;
}

bool TaMxfMetaDataParse::isAudio(MDObjectPtr Descriptor)
{
	if(Descriptor->IsA(WaveAudioDescriptor_UL))
	{
		m_essenceType = "Wave";
		m_audioProperty.Channels = Descriptor->GetUInt(ChannelCount_UL);
		m_audioProperty.BitIndeepness = Descriptor->GetUInt(QuantizationBits_UL);
		m_audioProperty.SampleRate = getRational(Descriptor->GetString(AudioSamplingRate_UL));
		m_audioProperty.BitRate = m_audioProperty.SampleRate.Numerator * m_audioProperty.Channels * m_audioProperty.BitIndeepness;
		return true;
	}
	return false;
}

bool TaMxfMetaDataParse::getEssenceType(mxflib::PartitionPtr MasterPartition)
{
	PackageList::iterator package_ite = m_data->Packages.begin();
	for(; package_ite != m_data->Packages.end(); ++package_ite)
	{
		if((*package_ite)->IsA(SourcePackage_UL))
			break;
	}
	
	if(package_ite == m_data->Packages.end())
		return false;
	
	MDObjectPtr Descriptor = (*package_ite)[Descriptor_UL];
	if(!Descriptor)
		return false;
	
	Descriptor = Descriptor->GetLink();
	if(isVideo(Descriptor))
	{
		return true;
	}
	if(isAudio(Descriptor))
		return true;
	
	return false;
}

#ifdef MPEG3D
bool TaMxfMetaDataParse::getEssenceType(const char* filename)
{
	ASDCP::EssenceType_t EssenceType;
	ASDCP::MPEG2::VideoDescriptor MVDesc;
	ASDCP::JP2K::PictureDescriptor JPDesc;
	ASDCP::MPEG2::MXFSReader   MSReader;
	ASDCP::MPEG2::MXFReader    MReader;
	ASDCP::JP2K::MXFSReader    JSReader;
	ASDCP::JP2K::MXFReader     JReader;
	ASDCP::PCM::MXFReader     AReader;
	ASDCP::PCM::AudioDescriptor ADesc;

	PackageList::iterator package_ite = m_data->Packages.begin();
	for(; package_ite != m_data->Packages.end(); ++package_ite)
	{
		if((*package_ite)->IsA(SourcePackage_UL))
			break;
	}

	if(package_ite == m_data->Packages.end())
		return false;

	MDObjectPtr Descriptor = (*package_ite)[Descriptor_UL];
	if(!Descriptor)
		return false;


	if (ASDCP_SUCCESS(ASDCP::EssenceType(filename, EssenceType)))
	{
		switch(EssenceType)
		{
		case ASDCP::ESS_MPEG2_VES:
			m_essenceType = "MPEG2";
			if (ASDCP_FAILURE(MReader.OpenRead(filename)))
				return false;
			MReader.FillVideoDescriptor(MVDesc);
			m_videoProperty.BitRate = MVDesc.BitRate;
			m_videoProperty.StoredWidth = MVDesc.StoredWidth;
			m_videoProperty.StoredHeight = MVDesc.StoredHeight;
			m_videoProperty.EditRate = mxflib::Rational(MVDesc.EditRate.Numerator, MVDesc.EditRate.Denominator);
			m_videoProperty.AspectRatio = mxflib::Rational(MVDesc.AspectRatio.Numerator, MVDesc.AspectRatio.Denominator);
			break;
		case ASDCP::ESS_MPEG2_VES_S:
			m_essenceType = "MPEG2_3D";
			if (ASDCP_FAILURE(MSReader.OpenRead(filename)))
				return false;
			MSReader.FillVideoDescriptor(MVDesc);
			m_videoProperty.StoredWidth = MVDesc.StoredWidth;
			m_videoProperty.StoredHeight = MVDesc.StoredHeight;
			m_videoProperty.EditRate = mxflib::Rational(MVDesc.EditRate.Numerator, MVDesc.EditRate.Denominator);
			m_videoProperty.AspectRatio = mxflib::Rational(MVDesc.AspectRatio.Numerator, MVDesc.AspectRatio.Denominator);
			break;
		case ASDCP::ESS_JPEG_2000:
			m_essenceType = "JPEG2000";
			if (ASDCP_FAILURE(JReader.OpenRead(filename)))
				return false;
			JReader.FillPictureDescriptor(JPDesc);
			m_videoProperty.StoredWidth = JPDesc.StoredWidth;
			m_videoProperty.StoredHeight = JPDesc.StoredHeight;
			m_videoProperty.EditRate = mxflib::Rational(JPDesc.EditRate.Numerator, JPDesc.EditRate.Denominator);
			m_videoProperty.AspectRatio = mxflib::Rational(JPDesc.AspectRatio.Numerator, JPDesc.AspectRatio.Denominator);
			break;
		case ASDCP::ESS_JPEG_2000_S:
			m_essenceType = "JPEG2000_3D";
			if (ASDCP_FAILURE(JSReader.OpenRead(filename)))
				return false;
			JSReader.FillPictureDescriptor(JPDesc);
			m_videoProperty.StoredWidth = JPDesc.StoredWidth;
			m_videoProperty.StoredHeight = JPDesc.StoredHeight;
			m_videoProperty.EditRate = mxflib::Rational(JPDesc.EditRate.Numerator, JPDesc.EditRate.Denominator);
			m_videoProperty.AspectRatio = mxflib::Rational(JPDesc.AspectRatio.Numerator, JPDesc.AspectRatio.Denominator);
			break;
		case ASDCP::ESS_PCM_24b_48k:
		case ASDCP::ESS_PCM_24b_96k:
#if 0
			Descriptor = Descriptor->GetLink();
			if(isAudio(Descriptor))
				return true;
#endif
#if 1
			m_essenceType = "Wave";
			if (ASDCP_FAILURE(AReader.OpenRead(filename)))
				return false;
			AReader.FillAudioDescriptor(ADesc);
			m_audioProperty.Channels = ADesc.ChannelCount;
			m_audioProperty.BitIndeepness = ADesc.SampleRate.Numerator / ADesc.SampleRate.Denominator;
			m_audioProperty.SampleRate = mxflib::Rational(ADesc.AudioSamplingRate.Numerator, ADesc.AudioSamplingRate.Denominator);
			m_audioProperty.BitRate = m_audioProperty.SampleRate.Numerator * m_audioProperty.Channels * m_audioProperty.BitIndeepness;
			break;
#endif
		default:
			return false;
		}
		return true;
	}
	return false;
}
#endif

UInt64 getFileSize(const char* filename)
{
	UInt64 size=0;
	FileHandle file=FileOpenRead(filename);
	if( FileValid(file) )
	{
		FileSeekEnd(file);
		size=FileTell(file);
		FileClose(file);
	}
	return size;
}
bool TaMxfMetaDataParse::parse(const char* filename)
{
	m_temporalInfo.fileSize =getFileSize(filename);

	mxflib::MXFFilePtr m_File = new MXFFile;
	if (!m_File->Open(filename,true))
	{
		error("TaMxfMetaDataParse::parse,Could not open mxf file %s !\n",filename);
		return false;
	}
	m_File->GetRIP();
	PartitionPtr MasterPartition = m_File->ReadMasterPartition();		// Read the header partition pack
	
	if(!MasterPartition)
	{
		m_File->Seek(0);
		MasterPartition = m_File->ReadPartition();
		
		if(!MasterPartition)
		{
			error("Could not read the Header!\n");
			return false;
		}
		warning("Could not locate a closed partition containing header metadata - attempting to process using open header\n");
	}
	
	//Length Bytes = 
	MasterPartition->ReadMetadata();		// Read the metadata from the header
	m_data = MasterPartition->ParseMetadata();
	if(!m_data)
		return false;
	if(!getPrefaceInfo(MasterPartition))
		return false;
	if(!getTemporalInfo())
		return false;
	if(!getGenericPackageInfo())
		return false;
	if(!getIdentifInfo(MasterPartition))
		return false;
	if(!getEncrptInfo())
		return false;
#ifdef MPEG3D
	if (!getEssenceType(filename))
	{
		warning("%s: Essence type error!\n", filename);
		return false;
	}
#else
	if(!getEssenceType(MasterPartition))
	{
		warning("%s: Essence type error!\n", filename);
		return false;
	}
#endif
	return true;
}
