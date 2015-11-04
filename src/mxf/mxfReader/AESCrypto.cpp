#include "Rijndael.h"
#include "AESCrypto.h"

 bool CAESCrypto::setAesKey(unsigned char* key, unsigned int keylen)
 {
 	if(keylen != 16)
 		return false;
 	memcpy(m_key, key, keylen);	
 	return true;
  }

 bool CAESCrypto::setIV(const unsigned char* IV, unsigned int IVlen)
 {
 	if(IVlen != 16)
 		return false;
 	memcpy(m_IV, IV, IVlen);
 	return true;
 }

bool CAESCrypto::getIV(unsigned char* IV, unsigned int IVlen)
{
	if(IVlen < 16)
 		return false;
	memcpy(IV, m_IV, 16);
	return true;
}

int CAESCrypto::encrypt(unsigned char* data, int datalen, unsigned char* outdata)
{
	try
	{
		int num = (datalen % 16);
		//if(num != 0)
			//return -2;
		int enLen = (num == 0? datalen :(datalen + 16 -num));
		
		CRijndael aes;
		aes.MakeKey((char*)m_key, (char*)m_IV);
		aes.Encrypt((char*)data, (char*)outdata, enLen, CRijndael::CBC);
		return enLen;
	}
	catch(...)
	{
	}
	return -1;
}

int CAESCrypto::encryptInplace(unsigned char* data,int datalen)
{

	return -1;
}

int CAESCrypto::decrypt(unsigned char* data, int datalen, unsigned char* outdata)
{
	try
	{
		int num = (datalen % 16);
		if(num != 0)
			return -2;
			
		CRijndael aes;
		aes.MakeKey((char*)m_key, (char*)m_IV);
		aes.Decrypt((char*)data, (char*)outdata, datalen, CRijndael::CBC);
		return datalen;
	}
	catch(...)
	{
	}
	return -1;
}

int CAESCrypto::decrytpInplace(unsigned char* data,int datalen)
{

	return -1;
}
