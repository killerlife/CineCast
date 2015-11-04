#ifndef AESCRYPTO_H_
#define AESCRYPTO_H_

class CAESCrypto
{
public:
	 bool setAesKey(unsigned char* key, unsigned int keylen);
	 bool setIV(const unsigned char* IV, unsigned int IVlen);
	 bool getIV(unsigned char* IV, unsigned int IVlen);
	 //out datalen = datalen + padlen; 0:(16-datalen%16)
    int encrypt(unsigned char* data, int datalen, unsigned char* outdata);
    int encryptInplace(unsigned char* data,int datalen);
    int decrypt(unsigned char* data, int datalen, unsigned char* outdata);
    int decrytpInplace(unsigned char* data,int datalen);
private:
	  unsigned char m_key[16];
	  unsigned char m_IV[16];    
};

#endif /*AESCRYPTO_H_*/
