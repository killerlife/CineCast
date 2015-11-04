#ifndef MXFAPP_ITAMATADATAPARSE_H
#define MXFAPP_ITAMATADATAPARSE_H

#include <string>
#include <vector>
#include "exportdef.h"
#include "dataType.h"



class MXFESSENCEREADERDLL_API ITaMxfMetaDataParse
{
public:
	virtual bool parse(const char* filename)=0;
	
	virtual const TemporalInfo& temporalInfo()const =0;
	virtual const PrefaceInfo& prefaceInfo()const =0;
	virtual const std::vector<IdentifyInfo>& identifInfo()const =0;
	virtual const GenericPackageInfo& genericPackageInfo()const =0;
	virtual bool isEncrpt()const =0;
	virtual bool isHmac()const =0;
	//now return ["JPEG2000","MPEG2","MPEG2TS","Wave"]
	virtual std::string essenceType()const =0;
	virtual const VideoProperty& videoProperty()const =0;
	virtual const AudioProperty& audioProperty()const =0;
	virtual const EncryptInfo& encryptInfo()const =0;
};

MXFESSENCEREADERDLL_API ITaMxfMetaDataParse* createMetaDataParse(const std::string& dictPath="");
MXFESSENCEREADERDLL_API void releaseMetaDataParse(ITaMxfMetaDataParse* );

#endif

