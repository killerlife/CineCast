#include "../reader/ITaCompositionReader.h"
#include <ini/ini.h>
#include "t1.h"
#include <iostream>
#include <fstream>

#include "waveFile.h"

using namespace std;
using namespace brunt;

void MyTestReaderCase::setUp()
{
   m_pReader= createCompositionReader();
   m_repeatNum = 1;
   m_pIniFile =createMyini();
   
   CPPUNIT_ASSERT( m_pIniFile->load("testDcpReader.txt") );
   
   string indexName("MyTestReaderCase::setUp");
   m_pIniFile->read(indexName.c_str(),"compositionPlaylistFile",m_compositionFile);
   m_pIniFile->read(indexName.c_str(),"packinglistFile",m_packingFile);
   m_pIniFile->read(indexName.c_str(),"dumpPath",m_dumpPath);
   readIni(*m_pIniFile,indexName.c_str(), "readframe_repeat", m_repeatNum);
   string keystr;
   m_pIniFile->read(indexName.c_str(),"key",keystr);
   CPPUNIT_ASSERT_MESSAGE("get key error",keystr.length()>=32);
   char Buffer[32];
   memcpy(Buffer,keystr.c_str(),32);
   
   unsigned char KeyBuff[20];
   if(sscanf(Buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			(unsigned int*)&KeyBuff[0], (unsigned int*)&KeyBuff[1], (unsigned int*)&KeyBuff[2], 
			(unsigned int*)&KeyBuff[3], (unsigned int*)&KeyBuff[4], (unsigned int*)&KeyBuff[5], 
			(unsigned int*)&KeyBuff[6], (unsigned int*)&KeyBuff[7],	(unsigned int*)&KeyBuff[8], 
			(unsigned int*)&KeyBuff[9], (unsigned int*)&KeyBuff[10],(unsigned int*)&KeyBuff[11],
			(unsigned int*)&KeyBuff[12],(unsigned int*)&KeyBuff[13],(unsigned int*)&KeyBuff[14],
			(unsigned int*)&KeyBuff[15] ) == 16)
		{
			m_decrpt.SetKey(16, KeyBuff);
		}
		
   releaseMyini(m_pIniFile);
   
//   fstream f("testDcpReader.txt");
//   if( f.is_open())
//   {
//	   char buf[1024];
//	   f.getline(buf,sizeof(buf)-1);
//	   m_compositionFile=buf;
//	   f.getline(buf,sizeof(buf)-1);
//	   m_packingFile=buf;
//	   //
//	   f.getline(buf,sizeof(buf)-1);
//	   m_dumpPath=buf;
//	   cout << "dump path="<< m_dumpPath<<endl;
//	   buf[0]=0;
//	   //ger key
//	   f.getline(buf,sizeof(buf)-1);
//	   if(strlen(buf)>=32)
//	   {
//			char Buffer[32];
//			memcpy(Buffer,buf,32);
//			unsigned char KeyBuff[20];
//			if(sscanf(Buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
//				&KeyBuff[0], &KeyBuff[1], &KeyBuff[2], &KeyBuff[3], &KeyBuff[4], &KeyBuff[5], &KeyBuff[6], &KeyBuff[7], 
//				&KeyBuff[8], &KeyBuff[9], &KeyBuff[10], &KeyBuff[11], &KeyBuff[12], &KeyBuff[13], &KeyBuff[14], &KeyBuff[15] ) == 16)
//			{
//				m_decrpt.SetKey(16, KeyBuff);
//				
//			}
//	   }
//	   f.close();
//	
//	   cout<<endl;
//	   
//	   if(strlen(buf)<32)
//	   {
//		   	cout <<"get key fail from testDcpReader.txt"<<endl;
//		    CPPUNIT_ASSERT_MESSAGE("",false);
//	   }
//	   else
//		    cout<< "key=" << buf<<endl;
//	   cout<< "composition playlist file=" << m_compositionFile<<endl;
//	   cout<< "packing list file=" << m_packingFile<<endl;
//	   
//   }
//   else
//   CPPUNIT_ASSERT_MESSAGE("open testDcpReader.txt fail",false);
   
}
void MyTestReaderCase::tearDown() 
{
	releaseCompositionReader(m_pReader);
}

void MyTestReaderCase::testOpen()
{

	CPPUNIT_ASSERT(m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()));
	m_pReader->close();
}
void MyTestReaderCase::testOpenFail()
{

	CPPUNIT_ASSERT( !m_pReader->open(m_compositionFile.c_str(),"nothisfile.txt") );
	cout << m_pReader->getLastError() <<endl;
	CPPUNIT_ASSERT( !m_pReader->open("nothisfile.txt",m_packingFile.c_str()) );
	cout << m_pReader->getLastError() <<endl;
	
}

void MyTestReaderCase::testProperty()
{

	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	
	mxflib::Rational rate=	m_pReader->frameRate();
	CPPUNIT_ASSERT_EQUAL(24,rate.Numerator);
	CPPUNIT_ASSERT_EQUAL(1,rate.Denominator);

	//CPPUNIT_ASSERT_EQUAL(48,(int)m_pReader->duration());
	cout <<" composition duration =" <<(int)m_pReader->duration() <<endl;

	VideoProperty vp;
	CPPUNIT_ASSERT(m_pReader->videoProperty(vp));
	cout << "video property"<<endl;
	cout << "	AspectRatio=" <<vp.AspectRatio.Numerator <<"/" << vp.AspectRatio.Denominator <<endl;
	cout << "	BitRate=" << (int)vp.BitRate <<endl;
	cout << "	DisplayWidth=" << vp.StoredWidth <<endl;
	cout << "	DisplayHeight=" << vp.StoredHeight <<endl;
	cout << "	EditRate=" <<vp.EditRate.Numerator <<"/" << vp.EditRate.Denominator <<endl;
	
	AudioProperty ap;
	if(m_pReader->audioProperty(ap))
	{
	   	CPPUNIT_ASSERT_EQUAL(6,(int)ap.Channels);
	    CPPUNIT_ASSERT_EQUAL(48000,(int)ap.SampleRate.Numerator);
		cout << "audio property"<<endl;
		cout << "	BitIndeepness=" << ap.BitIndeepness <<endl;
		cout << "	BitRate=" << ap.BitRate <<endl;
		cout << "	Channels=" << ap.Channels <<endl;	
		cout << "	EncodeType=" << ap.EncodeType <<endl;	
		cout << "	SampleRate=" <<ap.SampleRate.Numerator <<"/" << ap.SampleRate.Denominator  <<endl;	
	}
	else
		cout <<" no audio file in the DCP" <<endl;
	m_pReader->close();

	
}
void MyTestReaderCase::testReadFrame()
{

	for(int k = 0; k < m_repeatNum; ++k)
	{
		CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
		cout<<" start read" <<endl;
		mxflib::UInt64 dur=m_pReader->duration();
		int num=0;
		mxflib::UInt64 i=0;
	#ifdef _WIN32
		mxflib::UInt32 time=GetTickCount();
	#else
		mxflib::UInt32 time=0;
		struct timeval	tv,tv1;
		gettimeofday(&tv, NULL);
	#endif
		AudioProperty ap;
		bool haveAudio = m_pReader->audioProperty(ap);
			
		for(;i<dur;++i,++num)
		{
			
			FrameData vData= m_pReader->readVideoFrame();
			CPPUNIT_ASSERT(!vData.empty());
			
			if(haveAudio)
			{
				FrameData aData= m_pReader->readAudioFrame();
				if(!aData.empty())
				{
					CPPUNIT_ASSERT( vData.time <= aData.time);
				}
				if(num%10 ==0)
				{
					cout<<" read " << num << "  time="<< (int)vData.time 
					    <<" video frame size=" <<vData.frame->Size << endl;
	                cout <<"audio time= " << (int)aData.time 
						<<" audio frame size=" << (aData.frame? aData.frame->Size: 0)<<endl;
				}
	
			}
			else
			{
				if(num%10 ==0)
					cout<<" read " << num << "  time="<< (int)vData.time <<" video frame size=" <<vData.frame->Size <<endl;
				
			}
			
			
		}
	#ifdef _WIN32
		time=GetTickCount()-time;
	#else
		gettimeofday(&tv1, NULL);
		time=(tv1.tv_sec-tv.tv_sec)*1000 + (tv1.tv_usec-tv.tv_usec)/1000;		
	#endif
		cout<<"!"<<endl;
			
		time =time/1000+1;
		cout<<" read " << num << "read time="<< time  <<" read rate="<< (num/time)  <<" /sec"  <<endl;
		
		cout<<" end read" <<endl;
		m_pReader->close();
	}
	
}
void MyTestReaderCase::testSeek()
{

	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	cout<<" seek and start read" <<endl;
	AudioProperty ap;
	bool haveAudio=m_pReader->audioProperty(ap);
	
	mxflib::UInt64 dur=m_pReader->duration();

	for(int m=1;m<=10;++m)	
	{	
		mxflib::UInt64 pos=dur/10*m;
		cout<<"seek to "<< pos<<endl;
		m_pReader->seek(pos);
		int num=0;
	
		for(int i=0;i< (int)(dur-pos) && num <10;++i)
		{
			
			++num;
			FrameData vData= m_pReader->readVideoFrame();
			CPPUNIT_ASSERT(!vData.empty());
			
			if(haveAudio)
			{
				FrameData aData= m_pReader->readAudioFrame();
                if(!aData.empty())
				{
					CPPUNIT_ASSERT( vData.time <= aData.time);
					cout<<" read " << (int)(i) << "  time="<< (int)vData.time <<" video frame size=" <<vData.frame->Size <<" audio frame size=" <<aData.frame->Size<<endl;
				}
			}
			else
			{
				cout<<" read " << (int)(i) << "  time="<< (int)vData.time <<" video frame size=" <<vData.frame->Size <<endl;
			}
	
		}
	}
	cout<<" seek  read end" <<endl;
	m_pReader->close();
	
	
}
void MyTestReaderCase::testReel()
{

	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	const vector<ReelInfo>& info=m_pReader->getReelInfo();
	AudioProperty ap;
	bool haveAudio=m_pReader->audioProperty(ap);
	
	CPPUNIT_ASSERT( info.size()>0 );
	for(unsigned int i=0;i<info.size();++i)
	{
		CPPUNIT_ASSERT( info[i].m_reelIndex==(int)i );
		
		cout<< "reel " << i <<endl;
		if( info[i].isViedoEncrypt() )
			{
				cout<< "video was encrypted!"<<endl;
				cout << " keyid= " << info[i].m_videoEncInfo.keyIdStr << endl; 
			}
		else
			cout<< "video wasn't encrypted!"<<endl;
		if( info[i].isAudioEncrypt() )
		{
			cout<< "audio was encrypted!"<<endl;
			cout << " keyid= " << info[i].m_audioEncInfo.keyIdStr << endl; 
		}
		else if(haveAudio)
			cout<< "audio wasn't encrypted! "<<endl;
		else 
			cout << " no audio"<<endl;
		cout<< "start frame=" <<(unsigned int)info[i].m_startFrame <<" end frame=" << (unsigned int)info[i].m_endFrame<<endl;
		
	}

}
void MyTestReaderCase::testDumpPicture()
{

	return;
	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	mxflib::UInt64 dur=m_pReader->duration();
	int num=0;
	const vector<ReelInfo>& info=m_pReader->getReelInfo();
	bool isEncrypt =info[0].isViedoEncrypt();
	
	
	for(;num< (int)dur && num<20;++num)
	//for(;num< (int)dur;++num)
	{
		
		FrameData vData= m_pReader->readVideoFrame();
		CPPUNIT_ASSERT(!vData.empty());
		char savefile[200];
    
		if(isEncrypt)
		{
			sprintf(savefile,"%stestDumpPicture_de_%07d.jpc",m_dumpPath.c_str(),num);
			cout<<"dumping jpeg2k... "<< savefile<<endl;
			dumpDecrypt(vData.frame,savefile);
		}
		else
		  {
		  		sprintf(savefile,"%stestDumpPicture_%07d.jpc",m_dumpPath.c_str(),num);
		  		cout<<"dumping non-encrypted jpeg2k... "<< savefile<<endl;
				ofstream file(savefile,ios_base::binary);
				if(file.is_open())
				{
					file.write((const char*)vData.frame->Data,vData.frame->Size);
					file.close();
				}
				
				sprintf(savefile,"%stestDumpPicture_%07d_32.jpc",m_dumpPath.c_str(),num);
				ofstream file1(savefile,ios_base::binary);
				if(file1.is_open())
				{
					int v = vData.frame->Size%32;
					if(v != 0)
					   v = 32 - v;
					file1.write((const char*)vData.frame->Data,vData.frame->Size + v );
					file1.close();
				}
				
		  }
	}
	m_pReader->close();
}

void MyTestReaderCase::testDumpPictureFile()
{
	return;
	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	mxflib::UInt64 dur=m_pReader->duration();
	int num=0;
	const vector<ReelInfo>& info=m_pReader->getReelInfo();
	bool isEncrypt =info[0].isViedoEncrypt();
	
	
	char savefile[200],savefile1[200];
    sprintf(savefile,"%stestDumpPicture.j2i",m_dumpPath.c_str());
    sprintf(savefile1,"%stestDumpPicture_32.j2i",m_dumpPath.c_str());
	cout<<"dumping jpeg2k to file... "<< savefile<<endl;
	
	ofstream file(savefile,ios_base::binary);
	ofstream file1(savefile1,ios_base::binary);
	
	if(file.is_open() && file1.is_open())
	{
		for(;num< (int)dur;++num)
		{
			
			FrameData vData= m_pReader->readVideoFrame();
			CPPUNIT_ASSERT(!vData.empty());
				
			if(isEncrypt)
			{
				//	dumpDecrypt(vData.frame,savefile);
			}
			else
			  {
					
				file.write((const char*)vData.frame->Data,vData.frame->Size);
				int v = vData.frame->Size%32;
				if(v != 0)
				   v = 32 - v;
				file1.write((const char*)vData.frame->Data,vData.frame->Size + v );
			  }
		}
	}
	file.close();
	file1.close();				
	m_pReader->close();

}
void MyTestReaderCase::dumpDecrypt(DataChunkPtr data,const string& filename)
{

			// Max length of AS-DCP header is:
			//   BER-Item-Length(9) + ContextID(16)
			//   BER-Item-Length(9) + PlaintextOffset(8)
			//   BER-Item-Length(9) + SourceULKey(16)
			//   BER-Item-Length(9) + SourceLength(9)
			//   BER-Item-Length(9) + IV(32) + Check(16)
			// So we load 116 bytes if we can
			Length Bytes=116;
			CPPUNIT_ASSERT_MESSAGE("error data size",data->Size > Bytes);
			// Min length of AS-DCP header is:
			//   BER-Item-Length(1) + ContextID(16)
			//   BER-Item-Length(1) + PlaintextOffset(8)
			//   BER-Item-Length(1) + SourceULKey(16)
			//   BER-Item-Length(1) + SourceLength(1)
			//   BER-Item-Length(1) + IV(16) + Check(16)
			// So we barf for less than 76 bytes
			// Index the start of the data
			UInt8 *p = data->Data;
			// ** Load the ContextID **

			// Read the BER length and move the pointer
			UInt8 *Prev_p = p;
			Length ItemLength = ReadBER(&p, (int)Bytes);
			Bytes -= (p - Prev_p);
			if((ItemLength != 16) || (Bytes < 16))
			{
				CPPUNIT_ASSERT_MESSAGE("error contextID ",false);
			}

			// Build the ContextID
			//ContextID = new UUID(p);

			// Update pointer and count
			p += 16;
			Bytes -= 16;
			// ** Load the PlaintextOffset **
			// Read the BER length and move the pointer
			Prev_p = p;
			ItemLength = ReadBER(&p, (int)Bytes);
			Bytes -= (p - Prev_p);

			if((ItemLength != 8) || (Bytes < 8))
			{
				CPPUNIT_ASSERT_MESSAGE("PlaintextOffset not 8 bytes",false);
			}
			// Read the PlaintextOffset
			// DRAGONS: The format used in the file is UInt64, but as KLVObject used "Length" (Int64) for its lengths
			//          we impose a limit of 2^63 bytes on Plaintext offset and keep everything signed internally
			Length PlaintextOffset = (Length)GetU64(p);

			// Update pointer and count
			p += 8;
			Bytes -= 8;


				// ** Load the SourceKey **

				// Read the BER length and move the pointer
				Prev_p = p;
				ItemLength = ReadBER(&p, (int)Bytes);
				Bytes -= (p - Prev_p);

				if((ItemLength != 16) || (Bytes < 16))
				{	
					CPPUNIT_ASSERT_MESSAGE("SourceKey not 16 bytes",false);
				}

				// Build the SourceKey
				//SourceKey = new UL(p);

				p += 16;
				Bytes -= 16;


			// ** Load the SourceLength **
			//
			// Note: The source length in the original specification was BER coded
			//       A later version of the specification uses UInt64
			//       This code attempts to determine which is used

			// Read the BER length and move the pointer
			Prev_p = p;
			ItemLength = ReadBER(&p,(int) Bytes);
			Bytes -= (p - Prev_p);

			if((ItemLength <= 0) || (ItemLength >= 9) || (Bytes < ItemLength))
			{
				CPPUNIT_ASSERT_MESSAGE("Invalid SourceLength size",false);
			}

			// If not 8-bytes it can't be UInt64 - otherwise it may be BER or UInt64 so see if it is valid BER
			// This will fail for unimaginably large UInt64 coded SourceLengths
			UInt64 ValueLength=0;
			if((ItemLength != 8) || (*p == 0x87))
			{
				// Read the BER SourceLength
				Prev_p = p;
				ValueLength = ReadBER(&p, (int)Bytes);
				Bytes -= (p - Prev_p);
			}
			else
			{
				// Read the UInt64 SourceLength
				ValueLength = GetU64(p);
				p += 8;
				Bytes -= 8;
			}

			if((UInt64)PlaintextOffset > ValueLength)
			{
				CPPUNIT_ASSERT_MESSAGE("Invalid AS-DCP data: PlaintextOffset",false);
			}

			// Read and the BER length of the Encrypted Source Value
			Prev_p = p;
			Length ESVLength = ReadBER(&p, (int)Bytes);
			Bytes -= (p - Prev_p);

			// Check that we have enough left for the IV and Check Value
			const int EncryptionGranularity=16;
			const int EncryptionOverhead=32;
			if(Bytes < EncryptionGranularity)
			{
				CPPUNIT_ASSERT_MESSAGE("Invalid AS-DCP data  ,Not enough bytes for IV and Check",false);
			}


			UInt8 IV[16],Check[16];
			// Store the initialization value
			memcpy(IV, p, 16);

				// Store the check value
			memcpy(Check, &p[16], 16);

				// Record offset to the encrypted data
			Length DataOffset = &p[32] - data->Data;

			// Check there is enough left for the specified data
			// There is padding from 1 to 16 bytes at the end of the data

			Length EncryptedLength = ((ValueLength-PlaintextOffset) + EncryptionGranularity) / EncryptionGranularity;
			EncryptedLength *= EncryptionGranularity;
			EncryptedLength += PlaintextOffset;

			CPPUNIT_ASSERT_EQUAL_MESSAGE("error Size of Encrypted Source ",(int)ESVLength,(int)(EncryptedLength + EncryptionOverhead) );
					
			Length BytesLeft = data->Size - DataOffset;
			if(BytesLeft < EncryptedLength)
					CPPUNIT_ASSERT_MESSAGE("Not enough bytes for encrypted value ", false );
			

			// All seems OK
			m_decrpt.SetIV(16,IV,true);
			UInt8* encryptData=data->Data+DataOffset+PlaintextOffset;
			UInt64 enLen=EncryptedLength - PlaintextOffset;
			m_decrpt.DecryptInPlace(enLen,encryptData);
			
				
			ofstream file(filename.c_str());
			if(file.is_open())
			{
				file.write((const char*)data->Data+DataOffset,ValueLength);
				file.close();
			}
}
void MyTestReaderCase::testDumpAudio()
{
	return;
	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	AudioProperty ap;
	bool haveAudio = m_pReader->audioProperty(ap);
	if(!haveAudio)
		return;	
 
	mxflib::UInt64 dur=m_pReader->duration();
	int num=0;
	mxflib::UInt64 i=0;
	string  savefile(m_dumpPath);
	savefile +="testDumpAudio.wav";
	CWaveFile waveFile(ap.Channels);
 	if(!waveFile.open(savefile))
	{
		CPPUNIT_ASSERT(false);
		return;	
	}	
	cout<<"dumping audio... "<< savefile<<endl;		
	for(;i<dur;++i,++num)
	{
		
		
			FrameData aData= m_pReader->readAudioFrame();
			if(!aData.empty())
				waveFile.writeData((const char*)aData.frame->Data,aData.frame->Size);

	}
    waveFile.close();
    m_pReader->close();
	
}

void MyTestReaderCase::testDumpPCM()
{

	CPPUNIT_ASSERT( 	m_pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()) );
	AudioProperty ap;
	bool haveAudio = m_pReader->audioProperty(ap);
	if(!haveAudio)
		return;	
 
	mxflib::UInt64 dur=m_pReader->duration();
	int num=0;
	mxflib::UInt64 i=0;
	string  savefile(m_dumpPath);
	savefile += "testDumpAudio.pcm";
	cout<<"dumping audio pcm... "<< savefile << endl;	
		
	ofstream file(savefile.c_str(),ios_base::binary);
	if(file.is_open())
	{
		
		for(;i<dur;++i,++num)
		{
			FrameData aData= m_pReader->readAudioFrame();
			if(!aData.empty())
				file.write((const char*)aData.frame->Data,aData.frame->Size);
		}			
	
	}
	file.close();
   m_pReader->close();
	
}

void MyTestReaderCase::testGreateNumber()
{
	for(int i = 0; i< 2000; ++i)
	{
		cout <<" index = " << i << endl;
		ITaCompositionReader* pReader = createCompositionReader();
		if(pReader == NULL)
		{
			cout << "create fail!" << endl;
				break;
		}
		if(pReader->open(m_compositionFile.c_str(),m_packingFile.c_str()))
		{
		cout <<" open succeed " << i << endl;
			
		const vector<ReelInfo>& info = pReader->getReelInfo();
		AudioProperty ap;
		bool haveAudio = pReader->audioProperty(ap);
		
		CPPUNIT_ASSERT( info.size()>0 );
		for(unsigned int i=0;i<info.size();++i)
		{
			CPPUNIT_ASSERT( info[i].m_reelIndex==(int)i );
			
			cout<< "reel " << i <<endl;
			if( info[i].isViedoEncrypt() )
				cout<< "video was encrypted!"<<endl;
			else
				cout<< "video wasn't encrypted!"<<endl;
			if( info[i].isAudioEncrypt() )
				cout<< "audio was encrypted!"<<endl;
			else if(haveAudio)
				cout<< "audio wasn't encrypted! "<<endl;
			else 
				cout << " no audio"<<endl;
			cout<< "start frame=" <<(unsigned int)info[i].m_startFrame <<" end frame=" << (unsigned int)info[i].m_endFrame<<endl;
			
		}
		pReader->close();
		}
		
		releaseCompositionReader(pReader);
	
	}
	
}
