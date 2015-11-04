#ifndef MXFAPP_HEADERMATADATAPARSE_H
#define MXFAPP_HEADERMATADATAPARSE_H
#include "IMetaDataParse.h"


class TaMxfMetaDataParse : public ITaMxfMetaDataParse
{
public:
	TaMxfMetaDataParse():m_encrpt(false),m_hmac(false)
	{
	}
	virtual ~TaMxfMetaDataParse()
	{
	}
	bool parse(const char* filename);
	
	const TemporalInfo& temporalInfo()const
	{
		return m_temporalInfo;
	}
	const PrefaceInfo& prefaceInfo()const
	{
		return m_prefaceInfo;
	}
	const std::vector<IdentifyInfo>& identifInfo()const
	{
		return m_identifyVector;
	}
	const GenericPackageInfo& genericPackageInfo()const
	{
		return m_genericInfo;
	}
	bool isEncrpt()const
	{
		return m_encrpt;
	}
	bool isHmac()const
	{
		return m_hmac;
	}
	std::string essenceType()const
	{
		return m_essenceType;
	}
	const VideoProperty& videoProperty()const 
	{
		return m_videoProperty;
	}
	const AudioProperty& audioProperty()const
	{
		return m_audioProperty;
	}
	const EncryptInfo& encryptInfo()const
	{
		return m_encryptInfo;
	}
	
private:
	bool getTemporalInfo();
	bool getPrefaceInfo(mxflib::PartitionPtr MasterPartition);
	bool getGenericPackageInfo();
	bool getIdentifInfo(mxflib::PartitionPtr MasterPartition);
	bool getEncrptInfo();
	bool getEssenceType(mxflib::PartitionPtr MasterPartition);
#ifdef MPEG3D
	bool getEssenceType(const char* filename);
#endif
	bool isVideo(mxflib::MDObjectPtr Descriptor);
	bool isAudio(mxflib::MDObjectPtr Descriptor);
	const std::string getOperationalPattern(const std::string OP);
	mxflib::Rational getRational(const std::string& value);
   
private:
	mxflib::MetadataPtr		m_data;
    TemporalInfo			m_temporalInfo;
	PrefaceInfo				m_prefaceInfo;
	GenericPackageInfo		m_genericInfo;
	std::vector<IdentifyInfo>	m_identifyVector;
	std::string				m_essenceType;
	VideoProperty			m_videoProperty;
	AudioProperty			m_audioProperty;
	bool					m_encrpt;
	bool                    m_hmac;
	EncryptInfo				m_encryptInfo;
};
bool LoadMxfDictionary(const std::string& path="");

#endif
