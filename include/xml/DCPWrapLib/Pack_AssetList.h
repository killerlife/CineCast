// Pack_AssetList.h: interface for the CPack_AssetList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PACK_ASSETLIST_H_
#define __PACK_ASSETLIST_H_
#include "Pack_Asset.h"

class DCPWRAPLIB_API CPack_AssetList : public CElement
{
public:
	CPack_AssetList(IXMLNode* pNode);
	virtual ~CPack_AssetList();

public:
	int RemoveAssetByIndex(int nIndex);
	int RemoveAssetByid(int id);
	int RemoveAsset(CPack_Asset* pTrack);
	CPack_Asset GetAssetByIndex(int nIndex)const;
	CPack_Asset GetAssetByid(int id)const;
	CPack_Asset CreateAsset(long lPos=-1);
	
};

#endif // __PACK_ASSETLIST_H_
