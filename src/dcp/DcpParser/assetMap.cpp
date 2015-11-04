#include <DCPWrapLib/PackingList.h>
#include "assetMap.h"
#include <iostream>

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
	close();

	bool result = true;	
	string rootName("AssetMap");
	string fileName;

	static char* assetFile[] = {
		"ASSETMAP",
		"assetmap",
		"ASSETMAP.XML"
		"assetmap.xml"
	};

	// find the ASSETMAP file and open it
	for(int i=0; i<(sizeof(assetFile)/sizeof(assetFile[0])); i++)
	{
		string fileName = filePath + assetFile[i];
		try
		{
			m_prjElement = new  CElement(rootName.c_str(), fileName.c_str(), OPENFILE_FLAG);
			m_filenameAssetMap = assetFile[i];
			break;
		}
		catch(std::exception&)
		{
			if (m_prjElement)
			{
				delete m_prjElement;
				m_prjElement = NULL;
			}
			result = false;
		}
	}
/*
	// open file 'ASSETMAP'
	try
	{
		string fileName = filePath + "ASSETMAP";
		m_prjElement = new  CElement(rootName.c_str(), fileName.c_str(), OPENFILE_FLAG);
	}
	catch(std::exception&)
	{
		result = false;
	}
	if(result) return result;
	// open file 'ASSETMAP.xml'
	try
	{
		string fileName = filePath + "ASSETMAP.xml";
		m_prjElement = new  CElement(rootName.c_str(), fileName.c_str(), OPENFILE_FLAG);
	}
	catch(std::exception&)
	{
		result = false;
	}
*/

	return result;
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
