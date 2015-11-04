#include <iostream>
#include <vector>
#include <string>
#include "crypto_asdcp.h"

class ITaCompositionReader;
class brunt::ICMyini;
class MyTestReaderCase 
{
private:
	
public:		
	MyTestReaderCase();
	~MyTestReaderCase();

	void testOpen();
	void testOpenFail();
	void testProperty();
	void testReadFrame();
	void testSeek();
	void testReel();
	void testDumpPicture();
	void testDumpPictureFile();
	void testDumpAudio();
	void testDumpPCM();
	
	void testGreateNumber();
private:
	void dumpDecrypt(DataChunkPtr data,const std::string& filename);		
private:
	brunt::ICMyini*  m_pIniFile;
	ITaCompositionReader* m_pReader;
	std::string		      m_compositionFile,m_packingFile;
	AESDecrypt			  m_decrpt;
	std::string           m_dumpPath;
	int                   m_repeatNum;
	int					  m_StartTime;
	int					  m_EndTime;
};
