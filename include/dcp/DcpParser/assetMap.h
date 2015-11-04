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
private:
	CElement*  m_prjElement;

};
#endif

