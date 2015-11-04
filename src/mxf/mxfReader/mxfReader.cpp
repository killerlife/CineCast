// mxfReader.cpp : 定义 DLL 应用程序的入口点。
//

#include "AESCrypto.h"
#include "mxfReader.h"
#include "brunt/debugprint.h"
#include "dms/interface.h"
#include "error/error.h"
#include "config/config.h"
#include "../dmscard/dmscard.h"
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <iostream>

using namespace std;

#define FRAME_BUFFSIZE		(1024*1024)
#define BUFFPOOLSIZE		50
//#define DMS_DECRYPT			// use DMS decrypting...

#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif

std::string  bin2keyStr(const unsigned char* inbuf)
{
	char Buffer[40];
	sprintf (Buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
					inbuf[0], inbuf[1], inbuf[2], inbuf[3], inbuf[4], inbuf[5], inbuf[6], inbuf[7],
					inbuf[8], inbuf[9], inbuf[10], inbuf[11], inbuf[12], inbuf[13], inbuf[14], inbuf[15]
					);
	
	return std::string(Buffer);

}


IMxfFileReader* createIMxfFileReader()
{
	CMxfFileReader* p = new CMxfFileReader;
	return p;
}

void releaseIMxfFileReader(IMxfFileReader*& p)
{
	if(p)
		delete p;
	p = NULL;
}

CMxfFileReader::CMxfFileReader()
:m_curIndex(-1), m_type(MEDIA_UNKNOWN), m_bufferPool(FRAME_BUFFSIZE, BUFFPOOLSIZE)
{
//	m_buffer = NULL;
	m_bDmsDecrypt = false;
}

CMxfFileReader::~CMxfFileReader()
{
	close();
}

int CMxfFileReader::open(TMxfFileItem* items, int count)
{
	if(!items || count==0)
		return -1;

	if(m_files.size()>0)
		return 0;			// has been opened

	m_type = items[0].type;
	m_bDmsDecrypt = false;

	bool bDmsTest = false;
	int error = 0;

	try
	{
		for(int i=0; i<count; ++i)
		{
			// check if type of items is same
			if(items[i].type!=m_type)
			{
				DPRINT((DP_Error, "CMxfFileReader", "open item i=%d type is not same as the first.", i));
				throw (int)MFR_E_OPENNOTSAMETYPE;
			}
			string strKey = bin2keyStr((unsigned char*)items[i].key);
			DPRINT((DP_Trace, "CMxfFileReader", "mxf file: %s", items[i].filename));
			DPRINT((DP_Trace, "CMxfFileReader", "entry point: %d", items[i].entry));
			DPRINT((DP_Trace, "CMxfFileReader", "duration: %d", items[i].duration));
			DPRINT((DP_Trace, "CMxfFileReader", "key: %s", strKey.c_str()));
			DPRINT((DP_Trace, "CMxfFileReader", "createEssenceReader(%s)", getConfig().getBinRootPath().c_str()));

			ITaMxfEssenceReader* pReader = createEssenceReader(getConfig().getBinRootPath());//("~");
			if(!pReader || !pReader->open(items[i].filename))
			{
				DPRINT((DP_Error, "CMxfFileReader", "open %s failed.", items[i].filename));
				throw (int)MFR_E_OPENESSENCEFILEFAILED;
			}

			int keyFrameOffset = 0;
			pReader->seekElement(items[i].entry, keyFrameOffset);
			m_EncryptFlags.push_back(pReader->metaData().isEncrpt());

			if(pReader->metaData().isEncrpt() && "00000000000000000000000000000000"==strKey)
				bDmsTest = true;

			TMediaProperty prop;
			prop.type = pReader->metaData().essenceType()=="Wave"?MEDIA_AUDIO:MEDIA_VIDEO;
			if(prop.type != m_type)
			{
				if(m_type==MEDIA_UNKNOWN)
					m_type = prop.type;
				else
					throw (int)MFR_E_OPENNOTSAMETYPE;
			}

			if(prop.type==MEDIA_AUDIO)
			{
				prop.property.audio.BitIndeepness = pReader->metaData().audioProperty().BitIndeepness;
				prop.property.audio.Channels = pReader->metaData().audioProperty().Channels;
				prop.property.audio.BitRate = pReader->metaData().audioProperty().BitRate;
				prop.property.audio.SampleRate.Numerator = pReader->metaData().audioProperty().SampleRate.Numerator;
				prop.property.audio.SampleRate.Denominator = pReader->metaData().audioProperty().SampleRate.Denominator;
			}
			else
			{
				prop.property.video.AspectRatio.Numerator = pReader->metaData().videoProperty().AspectRatio.Numerator;
				prop.property.video.AspectRatio.Denominator = pReader->metaData().videoProperty().AspectRatio.Denominator;
				prop.property.video.StoredHeight = pReader->metaData().videoProperty().StoredHeight;
				prop.property.video.StoredWidth = pReader->metaData().videoProperty().StoredWidth;
			}

			m_propertys.push_back(prop);
			m_files.push_back(pReader);
			m_items.push_back(items[i]);

			DPRINT((DP_Trace, "CMxfFileReader", "encrypted: %s", pReader->metaData().isEncrpt()?"true":"false"));
			DPRINT((DP_Trace, "CMxfFileReader", "media type: %s", prop.type==MEDIA_AUDIO?"audio":"video"));
		}

//		if(!m_buffer) delete m_buffer;
//		m_buffer = new unsigned char[FRAME_BUFFSIZE];
//		if(!m_buffer)
//		{
//			throw (int)MFR_E_ALLOCMEMORYFAILED;
//		}
	}
	catch(int e)
	{
		error = e;
		close();
	}

//#ifdef DMS_DECRYPT
	if(m_type==MEDIA_VIDEO)
	{
		char nullkey[16];
		memset(nullkey, 0, sizeof(nullkey));

		if(bDmsTest)	// use DMS decryption
		{
			m_bDmsDecrypt = true;
			{// reset the sam card
				unsigned char buffer[32];
				memset(buffer, 0, 32);
				int ret = DMSExIcCpu_ResetProc(1, buffer);
			}
	//		int ret = CinemaInit((unsigned char*)"ec847337-ae98-822a-f6ae-87aeab949f4c");
			int ret = CinemaInit((unsigned char*)items[0].uuid);	//"ec847337-ae98-822a-f6ae-87aeab949f4c";
			if(ret!=0)
			{
				DPRINT((DP_Error, "CMxfFileReader", "open items i=0, CinemaInit error: %d", ret));
				close();
				return MFR_E_DMSHEADER(ret);
			}
		}
		DPRINT((DP_Trace, "CMxfFileReader", "VIDEO DECRYPTION : %s", m_bDmsDecrypt?"DMS":"KDM"));
	}
	/*
	*/
//#endif

	m_curIndex = 0;

	return error;
}


int CMxfFileReader::readVideoFrame(MediaFrame* frame)
{
	frame->len = 0;

	if(m_curIndex<0)
		return MFR_E_NOTOPENED;
	if(m_type!=MEDIA_VIDEO)
		return MFR_E_READTYPEINVALID;
	if(m_curIndex>=m_files.size())
		return 0;

	mxflib::DataChunkPtr videoData; 
	do{
//#ifdef DMS_DECRYPT
		if(m_bDmsDecrypt)
			videoData = m_files[m_curIndex]->readElement(true);
//#else
		else
			videoData = m_files[m_curIndex]->readElement(false);
//#endif
		if(videoData)
		{
			int offset = 0;
			int videoLeftLen = videoData->Size;

			unsigned char* m_buffer = 0;
			m_bufferPool.GetBuffer((void**)&m_buffer);
			if(!m_buffer)
			{
				DPRINT((DP_Error, "CMxfFileReader", "m_bufferPool.GetBuffer() get buffer failed!"));
				return MFR_E_GETBUFFERERROR;
			}
			frame->buflen = m_bufferPool.GetBufferSize();

			// decrypting
			if(m_EncryptFlags[m_curIndex])
			{
//#ifdef DMS_DECRYPT
				if(m_bDmsDecrypt)
				{
					int ret = 0;
					uint64_t u64offset = 0, u64size = 0;

					assert(videoData->Size<=FRAME_BUFFSIZE);
					memcpy(m_buffer, videoData->Data, videoData->Size);

					ret = CinemaDecryptTripletKey(m_buffer, &u64offset, &u64size);
					if(ret)
					{
						m_bufferPool.ReleaseBuffer(m_buffer);
						DPRINT((DP_Error, "CMxfFileReader", "CinemaDecryptTripletKey error: %d", ret));
						return MFR_E_DMSHEADER(ret);
					}

					assert(u64size<=FRAME_BUFFSIZE);
					assert(u64size+u64offset<=FRAME_BUFFSIZE);
					frame->data = m_buffer;
					frame->offset = u64offset;
					frame->len  = u64size;
//#else
				}
				else
				{
					offset = decrypt(videoData, (unsigned char*)m_items[m_curIndex].key);
					if(offset<0)
					{
						m_bufferPool.ReleaseBuffer(m_buffer);
						DPRINT((DP_Error, "CMxfFileReader", "decrypt error: %d", offset));
						return MFR_E_DECRYPTERROR;
					}
				//	offset = 0;
					videoLeftLen = videoData->Size - offset;
					if(videoLeftLen>FRAME_BUFFSIZE)
					{
						DPRINT((DP_Error, "CMxfFileReader", "video date length slop over: %d", videoLeftLen));
						videoLeftLen = FRAME_BUFFSIZE;
					}
					memcpy(m_buffer, videoData->Data + offset , videoLeftLen);
					frame->data = m_buffer;
					frame->offset = 0;
					frame->len  = videoLeftLen;
//#endif			
				}

			}
			else
			{
				memcpy(m_buffer, videoData->Data + offset , videoLeftLen);
				frame->data = m_buffer;
				frame->offset = 0;
				frame->len  = videoLeftLen;
			}

			/*
			if(offset >= 0)
				videoLeftLen = videoData->Size - offset;

			if(videoLeftLen)
			{
			//	frame->data = new unsigned char[videoLeftLen];
				if(videoLeftLen>FRAME_BUFFSIZE)
				{
					DPRINT((DP_Error, "CMxfFileReader", "video date length slop over: %d", videoLeftLen));
					videoLeftLen = FRAME_BUFFSIZE;
				}
			}
			*/

			frame->type = m_type;

			break;
		}

		if(++m_curIndex>=m_files.size())
		{
			break;
		}

#ifdef DMS_DECRYPT______________________DOITNOTEXACTLY
		CinemaEnd();
		int ret = CinemaInit((unsigned char*)&m_items[m_curIndex].uuid);
		if(ret!=0)
		{
			DPRINT((DP_Error, "CMxfFileReader", "m_curIndex=%d, CinemaInit error: %d", m_curIndex, ret));
			return MFR_E_DMSHEADER(ret);
		}
#endif	

	}while(1);

	return 0;
}

int CMxfFileReader::readAudioFrame(MediaFrame* frame)
{
	frame->len = 0;

	if(m_curIndex<0)
		return MFR_E_NOTOPENED;
	if(m_type!=MEDIA_AUDIO)
		return MFR_E_READTYPEINVALID;
	if(m_curIndex>=m_files.size())
		return 0;

	mxflib::DataChunkPtr videoData; 
	do{
		videoData = m_files[m_curIndex]->readElement(false);
		if(videoData)
		{
			int offset = 0;
			int videoLeftLen = videoData->Size;

			unsigned char* m_buffer = 0;
			m_bufferPool.GetBuffer((void**)&m_buffer);
			if(!m_buffer)
			{
				DPRINT((DP_Error, "CMxfFileReader", "m_bufferPool.GetBuffer() get buffer failed!"));
				return MFR_E_GETBUFFERERROR;
			}
			frame->buflen = m_bufferPool.GetBufferSize();

			if(videoLeftLen)
			{
			//	frame->data = new unsigned char[videoLeftLen];
				if(videoLeftLen>FRAME_BUFFSIZE)
				{
					DPRINT((DP_Error, "CMxfFileReader", "audio date length slop over: %d", videoLeftLen));
					videoLeftLen = FRAME_BUFFSIZE;
				}
				frame->data = m_buffer;
				memcpy(frame->data, videoData->Data + offset , videoLeftLen);
			}

			frame->type = m_type;
			frame->len  = videoLeftLen;
			frame->offset = 0;
			break;
		}

		if(++m_curIndex>=m_files.size())
		{
			break;
		}

	}while(1);

	return 0;
}


int CMxfFileReader::releaseFrame(MediaFrame* frame)
{
//	if(frame && frame->data==m_buffer)
//		frame->data = 0;
	if(frame)
	{
		m_bufferPool.ReleaseBuffer(frame->data);
		frame->data = 0;
	}
	return 0;
}

int CMxfFileReader::seek(long ms)
{
	// must be opened first 
	if(m_curIndex<0)
		return -1;

#ifdef DMS_DECRYPT______________________DOITNOTEXACTLY
	if(m_type==MEDIA_VIDEO)
		CinemaEnd();
#endif

	unsigned int frameOffset = ms/1000L*24;
	unsigned int dur = 0;
	for(unsigned int i = 0; i < m_files.size(); ++i)
	{
#ifdef DMS_DECRYPT______________________DOITNOTEXACTLY
		if(m_type==MEDIA_VIDEO)
		{
			int ret = CinemaInit((unsigned char*)&m_items[i].uuid);
			if(ret!=0)
			{
				DPRINT((DP_Error, "CMxfFileReader", "seek i=%d, CinemaInit error: %d", i, ret));
				return MFR_E_DMSHEADER(ret);
			}
		}
#endif
		dur += m_items[i].duration;
		if(frameOffset < dur)
		{
			m_curIndex = i;
			unsigned int offset = frameOffset - (dur - m_items[i].duration);
			DPRINT((DP_Trace, "CMxfFileReader", "seek to: index=%d, offset=%d", i, offset));
			int keyFrameOffset = 0;
			m_files[i]->seekElement(m_items[i].entry + offset, keyFrameOffset);
			break;
		}

#ifdef DMS_DECRYPT______________________DOITNOTEXACTLY
		if(m_type==MEDIA_VIDEO)
			CinemaEnd();
#endif

	}
	return 0;
}

long CMxfFileReader::duration()
{
	long dur = 0;
	for(unsigned int i = 0; i < m_items.size(); ++i)
	{
		//TODO: judge vDur is valid. m_playItem[i].vDur <= (m_vfiles[i]->duration() - m_playItem[i].vEntryPoint)
		dur += m_items[i].duration * 1000 / 24;
	}
	return dur;
}

const TMediaProperty* CMxfFileReader::getMediaProperty(unsigned int index)
{
	if(index>=0 && index<m_propertys.size())
		return &m_propertys[index];
	return 0;
}

void CMxfFileReader::close()
{

//#ifdef DMS_DECRYPT
	if(m_type==MEDIA_VIDEO)
		if(m_bDmsDecrypt)
			CinemaEnd();
//#endif

	for(unsigned int i=0; i<m_files.size(); ++i)
	{
		m_files[i]->close();
		releaseEssenceReader(m_files[i]);
	}
	m_items.clear();
	m_files.clear();
	m_EncryptFlags.clear();

	m_curIndex = -1;
	m_type = MEDIA_UNKNOWN;
//	if(m_buffer)
//		delete m_buffer;
//	m_buffer = NULL;

//	m_bufferPool.ReleasePool();
}

int CMxfFileReader::decrypt(mxflib::DataChunkPtr& data, unsigned char* key)
{

	// Max length of AS-DCP header is:
	//   BER-Item-Length(9) + ContextID(16)
	//   BER-Item-Length(9) + PlaintextOffset(8)
	//   BER-Item-Length(9) + SourceULKey(16)
	//   BER-Item-Length(9) + SourceLength(9)
	//   BER-Item-Length(9) + IV(32) + Check(16)
	// So we load 116 bytes if we can
	Length Bytes=116;
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
		//error;
		return -1;
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
		//CPPUNIT_ASSERT_MESSAGE("PlaintextOffset not 8 bytes",false);
		return -1;
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
		//CPPUNIT_ASSERT_MESSAGE("SourceKey not 16 bytes",false);
		return -1;
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
		//CPPUNIT_ASSERT_MESSAGE("Invalid SourceLength size",false);
		return -1;
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
		//CPPUNIT_ASSERT_MESSAGE("Invalid AS-DCP data: PlaintextOffset",false);
		return -1;
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
		//CPPUNIT_ASSERT_MESSAGE("Invalid AS-DCP data  ,Not enough bytes for IV and Check",false);
		return -1;
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
	//FIXME:
	if((ValueLength-PlaintextOffset)%EncryptionGranularity == 0)
	{
		EncryptedLength = (ValueLength-PlaintextOffset);
	}
	else
		EncryptedLength *= EncryptionGranularity;			
	EncryptedLength += PlaintextOffset;

	if(ESVLength != (EncryptedLength + EncryptionOverhead))
	{
		//may be 16 bytes more.
		//cout <<ESVLength <<"!=" << (EncryptedLength + EncryptionOverhead) << endl;
		//return -1;
	}
	Length BytesLeft = data->Size - DataOffset;
	if(BytesLeft < EncryptedLength)
	{
		return -1;
	}

	// All seems OK
	CAESCrypto m_decrpt;
	m_decrpt.setAesKey(key, 16);
	//m_decrpt.setIV(IV, 16);
	m_decrpt.setIV(Check,16);
	int tempLen = EncryptedLength - PlaintextOffset;
	static int g_i = 0;
	if(g_i == 0)
	{
		//			cout <<"enlen=" << tempLen << endl;
		g_i++;
	}
	unsigned char* m_enBuf = new unsigned char[tempLen];
	m_decrpt.decrypt(data->Data+DataOffset+PlaintextOffset, tempLen, m_enBuf);
	memcpy(data->Data+DataOffset+PlaintextOffset, m_enBuf,tempLen);
	delete []m_enBuf;
	data->Resize(DataOffset+ValueLength);
	return (int)DataOffset;
}
