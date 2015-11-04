//////////////////////////////////////////////////////////////////////////
//Macro define
#ifndef __MY_MACRO_DEF_H_
#define __MY_MACRO_DEF_H_

namespace _xmlDataType
{
#ifdef _WIN32
typedef __int64 Int64;				//!< Signed 64-bit integer
typedef unsigned __int64 UInt64;	//!< Unsigned 64-bit integer
#define atoInt64(str) _atoi64(str);
#define StrNoCaseCompare(szval1,szval2)		stricmp(szval1,szval2)
#define SprintfInt64(szdes,nval)	sprintf(szdes,"%I64d",nval)
#else
typedef long long Int64;			//!< Signed 64-bit integer
typedef unsigned long long UInt64;	//!< Unsigned 64-bit integer
#define  atoInt64(str) atoll(str);
#define StrNoCaseCompare(szval1,szval2)		strcasecmp(szval1,szval2)
#define SprintfInt64(szdes,nval)	sprintf(szdes,"%lld",nval)
#endif

#define _CheckPointer(p,ret) {if((p)==NULL) return (ret);}
#define DCI_RELEASE(p)	{if((p)!=NULL) {p->release();p=NULL;}}
#define DCI_DELETE(p) {if((p)!=NULL) {delete p;p=NULL}}

#define _CheckPointer(p,ret) {if((p)==NULL) return (ret);}
#define DCI_RELEASE(p)	{if((p)!=NULL) {p->release();p=NULL;}}
#define DCI_DELETE(p) {if((p)!=NULL) {delete p;p=NULL}}

};

#define STR_COMPOSITION_PLAYLIST "CompositionPlaylist"
#define STR_PACKINGLIST			"PackingList"
#define STR_DCPMAKER			"dcpmaker"
/*
#define STR_CONTENT_TITLE		"ContentTitleText"
#define STR_CONTENT_KIND		"ContentKind"
#define STR_CONTENT_VERSION		"ContentVersion"
*/

#define STR_MAIN_PICTURE		"MainPicture"
#define STR_MAIN_SOUND			"MainSound"
#define STR_MAIN_SUBTITLE		"MainSubtitle"
#define STR_AUX_SOUND			"AuxiliarySound"

//////////////////////////////////////////////////////////////////////////
//packinglist
#define ASSETTYPE_COMPOSITIONPLAYLIST	"text/xml;asdcpKind=CPL"
#define ASSETTYPE_PICTURE				"application/x-smpte.mxf;asdcpKind=Picture"
#define ASSETTYPE_SOUND					"application/x-smpte.mxf;asdcpKind=Sound"
#define ASSETTYPE_SUBTITLE				"text/xml;asdcpKind=Subtitle"
#define ASSETTYPE_TTF_SUBTITLE			"application/ttf;asdcpKind=TTF"
#define ASSETTYPE_PNG_SUBTITLE			"image/png;asdcpKind=PNG"


//////////////////////////////////////////////////////////////////////////
//content_kind
#define KIND_FEATURE			"feature"
#define KIND_TRAILER			"trailer"
#define KIND_TEST				"test"
#define KIND_TEASER				"teaser"
#define KIND_RATING				"rating"
#define KIND_ADVERTISEMENT		"advertisement"
#define KIND_SHORT				"short"
#define KIND_TRANSITIONAL		"transitional"
#define KIND_PSA				"psa"
#define KIND_POLICY				"policy"
//////////////////////////////////////////////////////////////////////////

#endif //__MY_MACRO_DEF_H_

