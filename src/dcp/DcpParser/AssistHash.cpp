#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AssistHash.h"
#include "assetMap.h"
using namespace std;

CAssistHash::CAssistHash(const CPackingList& packingList, const std::string& path):m_packingList(packingList),m_basePath(path+"/")
{
	
}

CAssistHash::~CAssistHash(){};
	
bool CAssistHash::GetHashInfo(std::vector<_hash_file_info>& infos)const
{
	try
	{
		CPack_AssetList assetList = m_packingList.GetAssetList();
		for(int i = 0;i < assetList.GetChildCountByName("Asset"); ++i)
		{
				CPack_Asset assert = assetList.GetAssetByIndex(i);
				const USERTEXT& utext = assert.GetOriginalFileNameElement();
  				string filename = utext.strValue;
				if(filename.empty())
				{
					CTaAssetMap assertMap;
					if(assertMap.open(m_basePath))
					{
						filename = assertMap.findFile(assert.GetUUIDElement());
						assertMap.close();
					}
					else 
					{
							continue;
					}
				}
				//no hash value,so don't need to check.	
				string hashValue = assert.GetHashElement();
              if(hashValue.empty())
                	continue;
				_hash_file_info info;
				info.fileName = m_basePath + filename;
				info.hashValue = hashValue;
				infos.push_back(info);
		}
		return true;
	}
	catch(std::exception&)
	{
	}
	catch(...)
	{
	}
	return false;
}
