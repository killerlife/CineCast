/*! \file	crypto_asdcp.cpp
 *	\brief	AS-DCP compatible encryption and decryption
 *
 *	\version $Id: crypto_asdcp.cpp,v 1.3 2007/04/17 04:02:38 baiqian Exp $
 *
 */
/*
 *  Copyright (c) 2004, Matt Beard
 *
 *	This software is provided 'as-is', without any express or implied warranty.
 *	In no event will the authors be held liable for any damages arising from
 *	the use of this software.
 *
 *	Permission is granted to anyone to use this software for any purpose,
 *	including commercial applications, and to alter it and redistribute it
 *	freely, subject to the following restrictions:
 *
 *	  1. The origin of this software must not be misrepresented; you must
 *	     not claim that you wrote the original software. If you use this
 *	     software in a product, an acknowledgment in the product
 *	     documentation would be appreciated but is not required.
 *	
 *	  2. Altered source versions must be plainly marked as such, and must
 *	     not be misrepresented as being the original software.
 *	
 *	  3. This notice may not be removed or altered from any source
 *	     distribution.
 */

#include "crypto_asdcp.h"

using namespace mxflib;

using namespace std;

//! True if we are doing hashing calculations
bool Hashing = false;


//! Build an AS-DCP hashing key from a given crypto key
/*  The hashing key is: 
 *  - trunc( HMAC-SHA-1( CipherKey, 0x00112233445566778899aabbccddeeff ) )
 *  Where trunc(x) is the first 128 bits of x
 */
DataChunkPtr BuildHashKey(int Size, const UInt8 *CryptoKey)
{
	//! Constant value to be hashed with cypher key to produce the hashing key
	const UInt8 KeyConst[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

	HashPtr Hasher = new HashHMACSHA1();

	// Hash the constant data with the crypto key
	Hasher->SetKey(Size, CryptoKey);
	Hasher->HashData(16, KeyConst);
	DataChunkPtr Ret = Hasher->GetHash();

	// Truncate the hashed key to 128-bits (16-bytes)
	Ret->Resize(16);

	return Ret;
}


//! Set the key and start hashing
/*  \return True if key is accepted
 */
bool HashHMACSHA1::SetKey(UInt32 Size, const UInt8 *Key)
{
	if(Size > 64)
	{
		error("Key size > 64 bytes not supported by HashHMACSHA1\n");
		return false;
	}

	// Clear the key buffers
	memset(KeyBuffer_i, 0, 64);
	memset(KeyBuffer_o, 0, 64);

	// Copy the hash key to the key buffers
	memcpy(KeyBuffer_i, Key, Size);
	memcpy(KeyBuffer_o, Key, Size);

	// Exclusive-or the keys with the required constants
	int i;
	for(i=0; i<64; i++)
	{
		KeyBuffer_i[i] ^= 0x36;
		KeyBuffer_o[i] ^= 0x5c;
	}

	// Initialize the SHA-1 algorythm and inject the inner key
	SHA1_Init(&Context);
	SHA1_Update(&Context, KeyBuffer_i, 64);

	KeyInited = true;

/*
int n = 1;
char Name[128];
sprintf(Name, "Hash%03d.dat", n);
while(FileExists(Name)) 
{
n++;
sprintf(Name, "Hash%03d.dat", n);
}
OutFile = FileOpenNew(Name);
printf("Writing hash data to \"%s\"\n", Name);
*/
	return true;
};


//! Add the given data to the current hash being calculated
void HashHMACSHA1::HashData(UInt32 Size, const UInt8 *Data)
{
//FileWrite(OutFile, Data, Size);
	if(!KeyInited)
	{
		error("HashHMACSHA1::HashData() called without setting the key\n");
		return;
	}

	SHA1_Update(&Context, Data, Size);
}


//! Get the finished hash value
DataChunkPtr HashHMACSHA1::GetHash(void)
{
//FileClose(OutFile);
	// Build a data chunk for the output
	DataChunkPtr Ret = new DataChunk;
	Ret->Resize(20);

	SHA1_Final(Ret->Data, &Context);

	// Hash the inner hash with the outer key
	SHA1_Init(&Context);
	SHA1_Update(&Context, KeyBuffer_o, 64);
	SHA1_Update(&Context, Ret->Data, Ret->Size);
	SHA1_Final(Ret->Data, &Context);

/*
printf("Hash is:");
for(int i=0; i<20; i++)
{
  printf(" %02x", Ret->Data[i]);
}
printf("\n");
*/
	return Ret;
}



//! Encrypt data and return in a new buffer
/*! \return NULL pointer if the encryption is unsuccessful
 */
DataChunkPtr AESEncrypt::Encrypt(UInt32 Size, const UInt8 *Data)
{
	// Calculate size of encrypted data (always a multiple of 16-bytes)
	UInt32 RetSize = (Size + 15) / 16;
	RetSize *= 16;
	
	DataChunkPtr Ret = new DataChunk(RetSize);
	AES_cbc_encrypt(Data, Ret->Data, Size, &CurrentKey, CurrentIV, AES_ENCRYPT);
	
	return Ret;
}
bool AESEncrypt::EncryptInPlace(UInt32 Size, UInt8 *Data)
{
	if(Size%16!=0)
		return false;
	AES_cbc_encrypt(Data, Data, Size, &CurrentKey, CurrentIV, AES_ENCRYPT);
	
	return true;
}

//! Decrypt data and return in a new buffer
/*! \return NULL pointer if the encryption is unsuccessful
 */
DataChunkPtr AESDecrypt::Decrypt(UInt32 Size, const UInt8 *Data)
{
	DataChunkPtr Ret = new DataChunk(Size);
	AES_cbc_encrypt(Data, Ret->Data, Size, &CurrentKey, CurrentIV, AES_DECRYPT);

	return Ret;
}
bool AESDecrypt::DecryptInPlace(UInt32 Size, UInt8 *Data)
{
	if(Size%16!=0)
		return false;
	AES_cbc_encrypt(Data, Data, Size, &CurrentKey, CurrentIV, AES_DECRYPT);
	return true;
}
bool AESEncryptConfig::buildKey(std::string KeyFileName) 
{
	char *NameBuffer = new char[KeyFileName.size() + 1];
	strcpy(NameBuffer, KeyFileName.c_str());
	// Scan back for the last directory seperator to find the filename
	char *NamePtr = &NameBuffer[strlen(NameBuffer)];
	while(NamePtr > NameBuffer)
	{
		if((*NamePtr == '/') || (*NamePtr == DIR_SEPARATOR))
		{
			NamePtr++;
			break;
		}
		NamePtr--;
	}
	
	// DRAGONS: Build in an int array for type-safety
	int KeyBuff[20];
	int Count = sscanf(NamePtr, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
					   &KeyBuff[0], &KeyBuff[1], &KeyBuff[2], &KeyBuff[3], &KeyBuff[4], &KeyBuff[5], &KeyBuff[6], &KeyBuff[7], 
					   &KeyBuff[8], &KeyBuff[9], &KeyBuff[10], &KeyBuff[11], &KeyBuff[12], &KeyBuff[13], &KeyBuff[14], &KeyBuff[15] );
	
	delete[] NameBuffer;
	
	if(Count != 16)
	{
		error("Key filename is not in the correct hex format of: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx\n");
		return false;
	}

	// Copy the datat into a byte buffer
	{
		int i;
		for(i=0; i<16; i++) m_ContextKey[i] = (UInt8)KeyBuff[i];
	}
	///////////////////////////////////////

	char Buffer[45];
	
	FileHandle KeyFile = FileOpenRead(KeyFileName.c_str());
	int Bytes = 0;
	if(FileValid(KeyFile))
	{
		Bytes = (int)FileRead(KeyFile, (UInt8*)Buffer, 32);
		FileClose(KeyFile);
		
		if(Bytes != 32) error("Failed to read key from key-file \"%s\"\n", Buffer);
	}
	if(Bytes == 32)
	{
		UInt8 KeyBuff[20];
		if(sscanf(Buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			(unsigned int*)&KeyBuff[0], (unsigned int*)&KeyBuff[1], (unsigned int*)&KeyBuff[2], 
			(unsigned int*)&KeyBuff[3], (unsigned int*)&KeyBuff[4], (unsigned int*)&KeyBuff[5], 
			(unsigned int*)&KeyBuff[6], (unsigned int*)&KeyBuff[7],	(unsigned int*)&KeyBuff[8], 
			(unsigned int*)&KeyBuff[9], (unsigned int*)&KeyBuff[10],(unsigned int*)&KeyBuff[11],
			(unsigned int*)&KeyBuff[12],(unsigned int*)&KeyBuff[13],(unsigned int*)&KeyBuff[14],
			(unsigned int*)&KeyBuff[15] ) == 16)
		{
			m_aesEncrypt.SetKey(16, KeyBuff);
			m_complete=true;
			return true;
		}
	}
	return false;
}

