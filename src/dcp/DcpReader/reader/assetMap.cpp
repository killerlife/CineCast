#include <dcpwrapinclude.h>
#include "assetMap.h"
using namespace std;

CTaAssetMap::CTaAssetMap():m_prjElement(NULL)
{
}

CTaAssetMap::~CTaAssetMap()
{
	close();
}

bool  CTaAssetMap::open(const std::string& filePath)
{
	string fileName = filePath + "ASSETMAP.xml";
	close();
	std::string rootName("AssetMap");
	try
	{
		m_prjElement = new  CElement(rootName.c_str(), fileName.c_str(), OPENFILE_FLAG);
	}
	catch(std::exception&)
	{
		return false;
	}
	return true;
}

void  CTaAssetMap::close()
{
	if(m_prjElement)
	{
		delete m_prjElement;
		m_prjElement = NULL;
	}
}

std::string CTaAssetMap::findFile(const std::string& uuid)
{
	try
	{
		CElement assertList = m_prjElement->GetChildByNameIndex("AssetList");
		int num = assertList.GetChildCountByName("Asset");
		for(int i = 0; i < num; ++i)
		{
			CElement assert = assertList.GetChildByNameIndex("Asset", i);
			CElement id = assert.GetChildByNameIndex("Id");
			if(id.GetTextcontent() == uuid)
			{
				CElement chunklist = assert.GetChildByNameIndex("ChunkList");
				CElement chunk = chunklist.GetChildByNameIndex("Chunk");
				return chunk.GetChildByNameIndex("Path").GetTextcontent();
			}

		}
	}
	catch(std::exception&)
	{
	}
	return "";
}
