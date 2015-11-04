#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxflib/mxflib.h>
#include <iostream>
using namespace std;
using namespace mxflib;
mxflib::EncryptObject::EncryptObject(Encrypt_Base *Enc,UUIDPtr ContextID, UInt64 encrptLen, int type)
{
					m_Enc=Enc;
					m_encrptContextID=ContextID;
					m_encrptLen= (encrptLen/16)*16;
					m_type = type;
					m_index = 0;
}
DataChunkPtr mxflib::EncryptObject::getEncryptData(DataChunkPtr& dataPtr)
{
		
		UInt64 ValueLength =dataPtr->Size;
		DataChunkPtr newDataPtr=new DataChunk(ValueLength+116+100);
		// Small buffer for header (note: max valid size of header should be 116 bytes)
		// Walking pointer for buffer writing
		UInt8 *pBuffer = newDataPtr->Data;
		
		// ** Write ContextID **
		pBuffer +=MakeBER(pBuffer, 4, 16, 4);		// Write BER length of ContextID (and index end of the length)
		memcpy(pBuffer, m_encrptContextID->GetValue(), 16);
		pBuffer += 16;
		// ** Write PlaintextOffset **
		// First adjust if required
		UInt64 enLen = m_encrptLen;
		if(m_type != 0)
		{
			bool IFrameFlag = false;
			UInt8 * pFrame = dataPtr->Data;
			while(1)
			{
				if(pFrame[0] == 0x00 && pFrame[1] == 0x00 
				&& pFrame[2] == 0x01 && pFrame[3] == 0x00)
				{
					int PictureData = (pFrame[4] << 8) | pFrame[5];
					int PictureType = (PictureData >> 3) & 0x07;
					if(PictureType == 1)
						IFrameFlag = true;
					break;
				}
				++pFrame; 
			}
			if(!IFrameFlag)
				enLen = 32;
			else
				cout <<"find IFrame ," << m_index <<", len=" << ValueLength<< endl;
		}
		++m_index;	
		UInt64 PlaintextOffset= (ValueLength<=enLen)?0:(ValueLength-enLen);
		
		pBuffer += MakeBER(pBuffer, 4, 8, 4);
		PutU64((UInt64)PlaintextOffset, pBuffer); pBuffer += 8;
		// ** Write SourceKey **
		pBuffer += MakeBER(pBuffer, 4, 16, 4);
		// Write a dummy value rather than just discarding all the data
		const UInt8 GCEssenceKey[16] =	{ 0x06, 0x0e, 0x2B, 0x34,
			0x01, 0x02, 0x01, 0x00,
			0x0d, 0x01, 0x03, 0x01,
			0x00, 0x00, 0x00, 0x00  };
		
		memcpy(pBuffer, GCEssenceKey, 16);
		//TODO: essence type and item number ,wrap type should be set by caller!
		pBuffer[7] = 0x01;
		pBuffer[12] = 0x15; // Treat GC Picture as "Picture"
		pBuffer[13] = 0x01;
		pBuffer[14] = 0x08;
		pBuffer[15] = 0x00;
		
		pBuffer += 16;
		// ** Write SourceLength **
		pBuffer += MakeBER(pBuffer, 4, 8, 4);
		PutU64(ValueLength, pBuffer);
		pBuffer += 8;
		// ** Calculate EncryptedLength **
		// There is padding from 1 to 16 bytes at the end of the data
		UInt64 EncryptedLength=_getEncryptLen(ValueLength, enLen);
		// ** Write the length of the encrypted source value
		// ** Including IV and Check as well as any padding
		const int EncryptionOverhead=32;
		pBuffer += MakeBER(pBuffer, 9, EncryptedLength + EncryptionOverhead, 0);
		// Set up the data offset
		// Note that we haven't yet written the IV and check value, but we count those as "header" bytes
		//(UInt32) DataOffset = (UInt32)(pBuffer - newDataPtr->Data) + EncryptionOverhead;
		UInt8 IV[16];
		int i; for(i=0; i<16; i++) IV[i] = (UInt8) rand();
		m_Enc->SetIV(16, IV, true);
		memcpy(pBuffer,&IV,16);
		pBuffer +=16;
		// Encrypt the check value... (Which is "CHUKCHUKCHUKCHUK" who ever said Chuck Harrison has no ego?)
		const UInt8 PlainCheck[16] = { 0x43, 0x48, 0x55, 0x4B, 0x43, 0x48, 0x55, 0x4B, 0x43, 0x48, 0x55, 0x4B, 0x43, 0x48, 0x55, 0x4B };
		DataChunkPtr CheckData = m_Enc->Encrypt(16, PlainCheck);
		
		memcpy(pBuffer,CheckData->Data,16);
		pBuffer +=16;
		memcpy(pBuffer,dataPtr->Data,ValueLength);//overlapped memory! EncryptedLength);
		//do encrpt;
		pBuffer+=PlaintextOffset;
		//m_Enc->EncryptInPlace(EncryptedLength-PlaintextOffset, pBuffer);
		if(m_Enc->CanEncryptInPlace())
		{
			m_Enc->EncryptInPlace(EncryptedLength-PlaintextOffset, pBuffer);
			
		}
		else
		{
			DataChunkPtr enData=m_Enc->Encrypt(EncryptedLength-PlaintextOffset, pBuffer);
			memcpy(pBuffer,enData->Data,EncryptedLength-PlaintextOffset);
			
		}
		pBuffer+=EncryptedLength-PlaintextOffset;
		newDataPtr->Resize( pBuffer-newDataPtr->Data );
		return newDataPtr;
}
UInt64	mxflib::EncryptObject::_getEncryptLen(UInt64 ValueLength, UInt64 enLen)
{
	UInt64 PlaintextOffset=(ValueLength <= enLen)?0:(ValueLength-enLen);
	const int EncryptionGranularity=16;
	UInt64 EncryptedLength = (ValueLength-PlaintextOffset);
	//FIXME: don't comment the line below?
	//if((ValueLength-PlaintextOffset)%EncryptionGranularity != 0)
	{
		EncryptedLength = ((ValueLength-PlaintextOffset) + EncryptionGranularity) / EncryptionGranularity;
		EncryptedLength *= EncryptionGranularity;
	}
	EncryptedLength += PlaintextOffset;
	return EncryptedLength;
				
}
UInt64	mxflib::EncryptObject::getEncryptLen(UInt64 ValueLength)
{
//	cout <<"m_index = " << m_index << endl;
	return _getEncryptLen(ValueLength, m_encrptLen)+100;
	
}




