#ifndef _RSA_SPRITELW_20080717_H__
#define _RSA_SPRITELW_20080717_H__

#include   <openssl\evp.h>
#include   <openssl\rsa.h>
#include   <openssl\pem.h>
#include   <openssl\err.h>


extern EVP_PKEY*   g_pEvpPubKey_i;
extern EVP_PKEY*   g_pEvpPrvKey_r;


#define   PUBKEY   \
    "-----BEGIN RSA PUBLIC KEY-----\n "   \
    "MIGJAoGBAMaxj/Eped3CcxHrm0dNGPowNqCxMlMgxf/+uL67O3vg0ZB4zP5aTuiN\n "   \
    "E7vC+3h2kLBRY4VyJGQUmyysyogaDRRI+c1iXLYsNcQhB7uCSyOMnnluRuwKu38O\n "   \
    "nT+GO5hh1jqii/8ya5pviinx64mvY8jxhqtYOYPGaO849y4hbdB1AgMBAAE=\n "   \
    "-----END RSA PUBLIC KEY-----\n "

#define   PRVKEY_r   \
    "-----BEGIN RSA PRIVATE KEY-----\n "   \
    "MIICXgIBAAKBgQDGsY/xKXndwnMR65tHTRj6MDagsTJTIMX//ri+uzt74NGQeMz+\n "   \
    "Wk7ojRO7wvt4dpCwUWOFciRkFJssrMqIGg0USPnNYly2LDXEIQe7gksjjJ55bkbs\n "   \
    "Crt/Dp0/hjuYYdY6oov/Mmuab4op8euJr2PI8YarWDmDxmjvOPcuIW3QdQIDAQAB\n "   \
    "AoGBAKfat3SPinFqZRW9icd5fWFWu7+TEqWhaUPzp5URzZXAfqnjM6KpHkJddE9J\n "   \
    "0HGbEG6zwzSoEdAujEz4iHzkLSj5z9aU81ya360B3BdNAYRdNN9OyxElVdJVY3D1\n "   \
    "0sP/Zi6360fIRDIbmWFwrzJ9IgIdYh0igEWBNb0L4l4OL8s9AkEA4rzwuNLPKidc\n "   \
    "w6Rs7oJfMhEmaJghNEJckN/dBWbmAZTeVQbp9B/+TM3nlYOoMlfJ4WUcZ5YkNxKx\n "   \
    "eMYmH4qkkwJBAOBWFCPrLASfB4c4Mq0suVFeMz5z6sEiXDujXC5jgWnGZv9RB2kt\n "   \
    "WFK3BSxRvt9qzFZju46xlkWVzxEIfoJ8o9cCQQCgsGKxucVUDJ1zkd4Zko/AveLI\n "   \
    "kbhGfxl3GxBaubgcOtLu032Oo/XnnK2ZirrzIsBSWwQsbX2xAjCAdX2jMPC9AkEA\n "   \
    "2mBwspZW/xifh22ZRyJmM2LzlzPXrbH4/sUDEtUiQQk1cgw6npPZiotGqeHZYDri\n "   \
    "NqZXCYytr4cpTndqTzNteQJAH0P9s8z+BUYj3ZQ8XzeZTxWPISCbLeVi/wO79STp\n "   \
    "7wB0F2pLVWitOpRY9EpZcDHx2kF4XmwVfHKwTHQpyaI8DA==\n "   \
    "-----END RSA PRIVATE KEY-----\n "

extern EVP_PKEY*   g_pEvpPubKey_i;
extern EVP_PKEY*   g_pEvpPrvKey_r;

EVP_PKEY   *   ExtractPublicKey(CString   &csKey);
EVP_PKEY   *   ExtractPrivateKey(CString   &csKey);
UINT   THREAPROC(PVOID pvoid);

#endif//_RSA_SPRITELW_20080717_H__
