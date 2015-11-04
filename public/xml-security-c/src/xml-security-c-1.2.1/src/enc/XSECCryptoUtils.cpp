/*
 * Copyright 2002-2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * XSEC
 *
 * XSECCryptoUtils:= Helper crypo utilities that make life easier
 *
 * Author(s): Berin Lautenbach
 *
 * $Id: XSECCryptoUtils.cpp,v 1.1.1.1 2006/08/21 08:15:01 baiqian Exp $
 *
 */

#include <xsec/framework/XSECDefs.hpp>
#include <xsec/framework/XSECError.hpp>
#include <xsec/enc/XSECCryptoUtils.hpp>
#include <xsec/enc/XSECCryptoKeyHMAC.hpp>
#include <xsec/utils/XSECPlatformUtils.hpp>

#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

// --------------------------------------------------------------------------------
//           XKMS Limited-Use Shared Secret handling
// --------------------------------------------------------------------------------

int CleanXKMSPassPhrase(unsigned char * input, int inputLen, safeBuffer &output) {

	int j = 0;
	unsigned char c;
	for (int i = 0; i < inputLen; ++i) {

		c = input[i];

		if (c >= 'A' && c <= 'Z') {
			output[j++] = c - 'A' + 'a';
		}
		else if (c != '\n' && c != '\r' && c != '\t' && c != ' ') {
			output[j++] = c;
		}

	}

	return j;

}

int DSIG_EXPORT CalculateXKMSAuthenticationKey(unsigned char * input, int inputLen, unsigned char * output, int maxOutputLen) {

	unsigned char keyVal[] = {XKMSAuthenticationValue};

	XSECCryptoKeyHMAC * k = XSECPlatformUtils::g_cryptoProvider->keyHMAC();
	Janitor<XSECCryptoKeyHMAC> j_k(k);
	k->setKey(keyVal, 1);

	XSECCryptoHash *h = XSECPlatformUtils::g_cryptoProvider->hashHMACSHA1();
	Janitor<XSECCryptoHash> j_h(h);
	h->setKey(k);

	// Clean the input
	safeBuffer sb;
	int l = CleanXKMSPassPhrase(input, inputLen, sb);

	h->hash((unsigned char *) sb.rawBuffer(), l);
	return h->finish(output, maxOutputLen);

}
	

int DSIG_EXPORT CalculateXKMSRevocationCodeIdentifierEncoding1(unsigned char * input, int inputLen, unsigned char * output, int maxOutputLen) {

	unsigned char keyVal[] = {XKMSRevocationCodeIdenfitierEncoding1};

	XSECCryptoKeyHMAC * k = XSECPlatformUtils::g_cryptoProvider->keyHMAC();
	Janitor<XSECCryptoKeyHMAC> j_k(k);
	k->setKey(keyVal, 1);

	XSECCryptoHash *h = XSECPlatformUtils::g_cryptoProvider->hashHMACSHA1();
	Janitor<XSECCryptoHash> j_h(h);

	h->setKey(k);

	// Clean the input
	safeBuffer sb;
	int l = CleanXKMSPassPhrase(input, inputLen, sb);

	h->hash((unsigned char *) sb.rawBuffer(), l);
	return h->finish(output, maxOutputLen);

}

int DSIG_EXPORT CalculateXKMSRevocationCodeIdentifierEncoding2(unsigned char * input, int inputLen, unsigned char * output, int maxOutputLen) {


	unsigned char tmpBuf[XSEC_MAX_HASH_SIZE];
	int tmpLen = CalculateXKMSRevocationCodeIdentifierEncoding1(input, inputLen, tmpBuf, XSEC_MAX_HASH_SIZE);
	return CalculateXKMSRevocationCodeIdentifierEncoding2From1(tmpBuf, tmpLen, output, maxOutputLen);

}

int DSIG_EXPORT CalculateXKMSRevocationCodeIdentifierEncoding2From1(unsigned char * input, int inputLen, unsigned char * output, int maxOutputLen) {

	unsigned char keyVal[] = {XKMSRevocationCodeIdenfitierEncoding2};

	XSECCryptoKeyHMAC * k = XSECPlatformUtils::g_cryptoProvider->keyHMAC();
	Janitor<XSECCryptoKeyHMAC> j_k(k);
	k->setKey(keyVal, 1);

	XSECCryptoHash *h = XSECPlatformUtils::g_cryptoProvider->hashHMACSHA1();
	Janitor<XSECCryptoHash> j_h(h);

	h->setKey(k);

	h->hash(input, inputLen);
	return h->finish(output, maxOutputLen);

}

int DSIG_EXPORT CalculateXKMSKEK(unsigned char * input, int inputLen, unsigned char * output, int maxOutputLen) {

	unsigned char keyVal[] = {XKMSKeyEncryption};

	XSECCryptoKeyHMAC * k = XSECPlatformUtils::g_cryptoProvider->keyHMAC();
	k->setKey(keyVal, 1);

	XSECCryptoHash *h = XSECPlatformUtils::g_cryptoProvider->hashHMACSHA1();
	Janitor<XSECCryptoHash> j_h(h);

	h->setKey(k);

	// Clean the input
	safeBuffer sb;
	int l = CleanXKMSPassPhrase(input, inputLen, sb);

	h->hash((unsigned char *) sb.rawBuffer(), l);
	return h->finish(output, maxOutputLen);

}

// --------------------------------------------------------------------------------
//           Some Base64 helpers
// --------------------------------------------------------------------------------

XMLCh DSIG_EXPORT * EncodeToBase64XMLCh(unsigned char * input, int inputLen) {

	XSECCryptoBase64 * b64 = XSECPlatformUtils::g_cryptoProvider->base64();
	Janitor<XSECCryptoBase64> j_b64(b64);
	unsigned char * output;
	int outputLen = ((4 * inputLen) / 3) + 5;
	XSECnew(output, unsigned char[outputLen]);
	ArrayJanitor<unsigned char> j_output(output);

	b64->encodeInit();
	int j = b64->encode(input, inputLen, output, outputLen - 1);
	j += b64->encodeFinish(&output[j], outputLen - j - 1);

	// Strip any trailing \n\r
	while (j > 0 && (output[j-1] == '\n' || output[j-1] == '\r'))
		j--;

	// Now transcode and get out of here
	output[j] = '\0';
	return XMLString::transcode((char *) output);

}


// --------------------------------------------------------------------------------
//           Some stuff to help with wierd signatures
// --------------------------------------------------------------------------------

const unsigned char ASNDSAProlog[] = {0x30, 0x2c, 0x02, 0x14};
const unsigned char ASNDSAMiddle[] = {0x02, 0x14};

bool ASN2DSASig(const unsigned char * input, unsigned char * r, unsigned char * s) {

	if (memcmp(ASNDSAProlog, input, 4) != 0 ||
		memcmp(ASNDSAMiddle, &input[24], 2) != 0)

		return false;

	memcpy(r, &input[4], 20);
	memcpy(s, &input[26], 20);

	return true;

}





