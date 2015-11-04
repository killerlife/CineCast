#include   "stdafx.h "
#include   <afx.h>
#include   <afxwin.h>
#include   <openssl\evp.h>
#include   <openssl\rsa.h>
#include   <openssl\pem.h>
#include   <openssl\err.h>
#include "rsa_SpriteLW.h"

char   ct[]= "Hello   World!\n ";

EVP_PKEY*   g_pEvpPubKey_i = NULL;
EVP_PKEY*   g_pEvpPrvKey_r = NULL;


char   *   OpenSSL_ERR_Print()
{
    static   char   buf[512]={0};
    BIO   *bio   =   BIO_new(BIO_s_mem());
    if(   bio   ==   NULL)
        return   "Create   bio   error ";
    
    ERR_print_errors(bio);
    int   len   =   BIO_gets(bio,buf,sizeof(buf));
    buf[len-1]   =   '\0 ';
    BIO_free(bio);
    return   buf;
}

//去掉/t   和空格
CString   TrimKeyToPem(const   char*   pSrc,int   nLen)
{
    CString   csRet= " ";
    bool   bCanEatWhile=true;
    for(int   i=0;   i <nLen;   i++)
    {
#if 1       // delete by liang
        if(   pSrc[i]   ==   '\n')
        {
            bCanEatWhile   =   true;
            csRet   +=   '\n';
        }
        else   if(   (pSrc[i]   ==   ' '   ||   pSrc[i]   ==   '\t'   )   &&   bCanEatWhile)
        {
            continue;
        }
        else
        {
            bCanEatWhile   =   false;
            csRet   +=   pSrc[i];
        }
#else
        if(pSrc[i] == ' '|| pSrc[i] == '\t' || pSrc[i]=='\n' || pSrc[i]=='\r') 
            continue;
        csRet   +=   pSrc[i];
#endif
    }
    return   csRet;
}

//提取私钥
EVP_PKEY   *   ExtractPrivateKey(CString   &csKey)
{
    if(   csKey.GetLength()   ==   0)
    {
        return   NULL;
    }
    
    BIO   *bio;
    void   *ret = NULL;
    const   unsigned   char   *p=NULL;
    unsigned   char   *data=NULL;
    long   len;
    
    if   ((bio=BIO_new(BIO_s_mem()))   ==   NULL)
    {
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        return   NULL;
    }
    
    csKey   =   TrimKeyToPem(csKey.GetBuffer(0),csKey.GetLength());
    BIO_puts(bio,csKey.GetBuffer(0));
    
    if   (PEM_bytes_read_bio(&data,   &len,   NULL,   PEM_STRING_RSA,   bio,   0,   0))
    {
        p   =   data;
        ret=d2i_PrivateKey(EVP_PKEY_RSA,NULL,&p,len);
        
        OPENSSL_free(data);
    }
    
    BIO_free(bio);
    if(   ret   ==   NULL)
    {
        printf( "%s\r\n ",OpenSSL_ERR_Print());
    }
    
    return   (EVP_PKEY   *)ret;
}

//提取公钥
EVP_PKEY   *   ExtractPublicKey(CString   &csKey)
{
    if(   csKey.GetLength()   ==   0)
    {
        return   NULL;
    }
    
    BIO   *bio;
    void   *ret = NULL;
    const   unsigned   char   *p=NULL;
    unsigned   char   *data=NULL;
    long   len;
    
    if   ((bio=BIO_new(BIO_s_mem()))   ==   NULL)
    {
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        return   NULL;
    }
    
    csKey   =   TrimKeyToPem(csKey.GetBuffer(0),csKey.GetLength());
    BIO_puts(bio,csKey.GetBuffer(0));
    
    if   (PEM_bytes_read_bio(&data,   &len,   NULL,   PEM_STRING_RSA_PUBLIC,   bio,   0,   0))
    {
        p   =   data;
        ret=d2i_PublicKey(EVP_PKEY_RSA,NULL,&p,len);
        
        OPENSSL_free(data);
    }
    
    BIO_free(bio);
    if(   ret   ==   NULL)
    {
        printf( "%s\r\n ",OpenSSL_ERR_Print());
    }
    
    return   (EVP_PKEY   *)ret;
}

UINT   THREAPROC(PVOID   pvoid)
{
    static   int   s_pid;
    static   int   s_cnt;
    s_pid++;
    int   pid   =   s_pid;
    unsigned   char   *buf;      
    unsigned   char   *buf2;
    /*   No   error   checking   */
    buf   =   (unsigned   char*)malloc(EVP_PKEY_size(g_pEvpPubKey_i));
    buf2   =   (unsigned   char*)malloc(EVP_PKEY_size(g_pEvpPrvKey_r));
    int   len;
    while(1)
    {
        len   =   RSA_public_encrypt(strlen(ct)+1,(unsigned   char*)ct,   buf,   g_pEvpPubKey_i-> pkey.rsa,RSA_PKCS1_PADDING);
        if   (len   !=   EVP_PKEY_size(g_pEvpPubKey_i))
        {
            printf( "%s\r\n ",OpenSSL_ERR_Print());
            fprintf(stderr, "Error:   ciphertext   should   match   length   of   key\n ");
        }
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        
        RSA_private_decrypt(len,   buf,   buf2,   g_pEvpPrvKey_r-> pkey.rsa,RSA_PKCS1_PADDING);
        if   (len   !=   EVP_PKEY_size(g_pEvpPrvKey_r))
        {
            printf( "%s\r\n ",OpenSSL_ERR_Print());
            fprintf(stderr, "Error:   ciphertext   should   match   length   of   key\n ");
        }
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        printf( "pub   pid=%d   %d--%s ",pid,++s_cnt,(char*)buf2);
        
        len   =   RSA_private_encrypt(strlen(ct)+1,(unsigned   char*)ct,   buf,   g_pEvpPrvKey_r-> pkey.rsa,RSA_PKCS1_PADDING);
        if   (len   !=   EVP_PKEY_size(g_pEvpPrvKey_r))
        {
            printf( "%s\r\n ",OpenSSL_ERR_Print());
            fprintf(stderr, "Error:   ciphertext   should   match   length   of   key\n ");
        }
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        
        RSA_public_decrypt(len,   buf,   buf2,   g_pEvpPubKey_i-> pkey.rsa,RSA_PKCS1_PADDING);
        if   (len   !=   EVP_PKEY_size(g_pEvpPubKey_i))
        {
            printf( "%s\r\n ",OpenSSL_ERR_Print());
            fprintf(stderr, "Error:   ciphertext   should   match   length   of   key\n ");
        }
        printf( "%s\r\n ",OpenSSL_ERR_Print());
        printf( "prv   pid=%d   %d--%s ",pid,++s_cnt,(char*)buf2);
    }
    
    free(buf);
    free(buf2);
    return   0;
}

int  main000(int   argc,   char*   argv[])
{
    g_pEvpPubKey_i   =   ExtractPublicKey(CString(PUBKEY));
    g_pEvpPrvKey_r   =   ExtractPrivateKey(CString(PRVKEY_r));
    
//    CWinThread   *   pThread1;
    int   i=   100;
    while(i--)
    {
//        pThread1   =   AfxBeginThread(THREAPROC,0);
        THREAPROC(0);
    }
    
//    WaitForSingleObject(pThread1-> m_hThread,(DWORD)-1);
    
    
    return   0;
} 

