#include "cppunit.h"
#include <iostream>
#include <vector>
#include <string>
#include "crypto_asdcp.h"

class ITaCompositionReader;
class brunt::ICMyini;
class MyTestReaderCase : public CppUnit::TestCase
{
private:
public:	
	CPPUNIT_TEST_SUITE( MyTestReaderCase );
	CPPUNIT_TEST( testOpen );
	CPPUNIT_TEST( testOpenFail );
	CPPUNIT_TEST( testProperty );
	CPPUNIT_TEST( testReadFrame );
	CPPUNIT_TEST( testSeek );
	CPPUNIT_TEST( testReel );
	CPPUNIT_TEST( testDumpPicture );
	CPPUNIT_TEST( testDumpPictureFile);
	CPPUNIT_TEST( testDumpAudio );
	CPPUNIT_TEST( testDumpPCM);
	CPPUNIT_TEST( testGreateNumber);
	CPPUNIT_TEST_SUITE_END();
	
	
public:		
	void setUp();
	void tearDown(); 

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
};
CPPUNIT_TEST_SUITE_REGISTRATION( MyTestReaderCase );
