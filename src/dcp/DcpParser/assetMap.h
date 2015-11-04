#ifndef MXFAPP_TAASSERTMAP_READER_H
#define MXFAPP_TAASSERTMAP_READER_H
#include <string>

class CElement;
class CTaAssetMap
{
public:
	CTaAssetMap();
	virtual ~CTaAssetMap();
	bool  open(const std::string& filePath);
	void  close();
	std::string findFile(const std::string& uuid);	
	std::string getAssetMap() { return m_filenameAssetMap; };	// get ASSETMAP file name
private:
	CElement*  m_prjElement;
	std::string m_filenameAssetMap;
};
#endif

