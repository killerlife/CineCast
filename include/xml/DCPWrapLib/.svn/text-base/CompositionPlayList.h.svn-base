// CompositionPlayList.h: interface for the CCompositionPlayList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPOSITIONPLAYLIST_H_
#define __COMPOSITIONPLAYLIST_H_

#include "DCIProjectElement.h"
#include "Composition_ReelList.h"
#include "Composition_RatingList.h"
#include "Composition_ContentVersion.h"

class DCPWRAPLIB_API CCompositionPlayList : public CDCIProjectElement
{
public:
	CCompositionPlayList();
	CCompositionPlayList(const char* szprefix,const char* sznamespace);
	CCompositionPlayList(const char* szInfo,SOURCEFLAG nFlag=OPENFILE_FLAG);
	virtual ~CCompositionPlayList();
	
public:
	SCOPEATTR GetContentKindElement()const;
	bool SetContentKindElement(const SCOPEATTR& szval,long lPos=-1);//attr:scope[op](URI)
	USERTEXT GetContentTitleTextElement()const;
	bool SetContentTitleTextElement(const USERTEXT& szval,long lPos=-1);
	std::string GetIconUUIDElement()const;
	bool SetIconUUIDElement(const char* szval,long lPos=-1);
public:
	CComposition_ReelList GetReelList()const;
	CComposition_ReelList CreateReelList(long lPos=-1);
	CComposition_RatingList GetRatingList()const;
	CComposition_RatingList CreateRatingList(long lPos=-1);
	CComposition_ContentVersion GetContentVersion()const;
	CComposition_ContentVersion CreateContentVersion(long lPos=-1);
	int RemoveContentVersion();
	int RemoveReelList();
	int RemoveRatingList();
	
};

#endif // __COMPOSITIONPLAYLIST_H_
