#ifndef MY_AES_H
#define MY_AES_H

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

typedef unsigned char  uint8;
typedef char           int8;
typedef unsigned short uint16;
typedef short          int16;
typedef unsigned int   uint32;
typedef int            int32;

typedef unsigned __int64 uint64;
typedef __int64          int64;
/******************************
For _LINUX
typedef long   long    int64;
typedef unsigned long  long  uint64;
******************************/

//using namespace std;   //ò

//same as function rfc3686_inc
void ctr_inc(unsigned char ctr_buf[16])
{
    if(!(++(ctr_buf[15])))
        if(!(++(ctr_buf[14])))
            if(!(++(ctr_buf[13])))
                ++(ctr_buf[12]);
}

//same as function rfc3686_init
//4Bytes nounce+8Bytes iv+4Bytes counter
void ctr_init( unsigned char nonce[4], unsigned char iv[8], unsigned char ctr_buf[16])
{
    memcpy(ctr_buf, nonce, 4);
    memcpy(ctr_buf +  4, iv, 8);
    memset(ctr_buf + 12, 0, 4);
    ctr_inc(ctr_buf);
}

static void print_hex(uint8* buf, uint64 len) {
    //Print results:
    for(int i=0;i<len;i++) {
        printf("%02X ",buf[i]);
        if(15 == i%16)
            printf("\n");
    }
    printf("\n");
}



/*
AES_RETURN aes_cfb_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, aes_encrypt_ctx cx[1]);
AES_RETURN aes_cfb_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, aes_encrypt_ctx cx[1]);
*/


#define DO 1

//
void MyAes_ctr_encrypt(uint8* msg, uint32 fsize,uint8 key[16])
{
#if DO
// 	OpenSSL_add_all_ciphers();
// 	OpenSSL_add_all_algorithms();
// 	ERR_load_crypto_strings();

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);

	int outlen = 0, tmplen;
	EVP_EncryptUpdate(&ctx, msg+outlen, &outlen, msg, fsize);
// 	EVP_EncryptFinal_ex(&ctx, msg+outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
#endif
}

//
void MyAes_ctr_decrypt(uint8* msg, uint32 fsize,uint8 key[16])
{
#if DO
// 	OpenSSL_add_all_ciphers();
// 	OpenSSL_add_all_algorithms();
// 	ERR_load_crypto_strings();

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);

	int outlen = 0, tmplen;
	EVP_DecryptUpdate(&ctx, msg + outlen, &outlen, msg, fsize);
// 	EVP_DecryptFinal_ex(&ctx, msg + outlen, &tmplen);
	EVP_CIPHER_CTX_cleanup(&ctx);
#endif
}




#endif // MY_AES_H
