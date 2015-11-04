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
 * DSIGKeyX509 := A "Super" key that defines a certificate with a sub-key that defines
 *                the signing key
 *
 * $Id: DSIGKeyInfoX509.cpp,v 1.1.1.1 2006/08/21 08:15:01 baiqian Exp $
 *
 */

#include <xsec/dsig/DSIGKeyInfoX509.hpp>
#include <xsec/framework/XSECError.hpp>
#include <xsec/utils/XSECPlatformUtils.hpp>
#include <xsec/utils/XSECDOMUtils.hpp>
#include <xsec/dsig/DSIGSignature.hpp>
#include <xsec/framework/XSECEnv.hpp>

#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_USE

// --------------------------------------------------------------------------------
//           Constructors and Destructors
// --------------------------------------------------------------------------------


DSIGKeyInfoX509::DSIGKeyInfoX509(const XSECEnv * env, DOMNode *X509Data) :
DSIGKeyInfo(env),
mp_X509IssuerName(NULL),
mp_X509SerialNumber(NULL),
mp_X509SubjectName(NULL),
mp_X509CRL(NULL),
mp_X509SKI(NULL),
mp_rawRetrievalURI(NULL),
mp_X509SubjectNameTextNode(0),
mp_X509IssuerNameTextNode(0),
mp_X509SerialNumberTextNode(0),
mp_X509CRLTextNode(0),
mp_X509SKITextNode(0) {

	mp_keyInfoDOMNode = X509Data;
	m_X509List.clear();

}

DSIGKeyInfoX509::DSIGKeyInfoX509(const XSECEnv * env) :
DSIGKeyInfo(env),
mp_X509IssuerName(NULL),
mp_X509SerialNumber(NULL),
mp_X509SubjectName(NULL),
mp_X509CRL(NULL),
mp_X509SKI(NULL),
mp_rawRetrievalURI(NULL),
mp_X509SubjectNameTextNode(0),
mp_X509IssuerNameTextNode(0),
mp_X509SerialNumberTextNode(0),
mp_X509CRLTextNode(0),
mp_X509SKITextNode(0) {

	mp_keyInfoDOMNode = 0;
	m_X509List.clear();

}


DSIGKeyInfoX509::~DSIGKeyInfoX509() {

	// SubjectName and IssuerName are local (decoded) copies of the
	// encoded DName held in the DOM

	if (mp_X509IssuerName != NULL)
		XSEC_RELEASE_XMLCH(mp_X509IssuerName);

	if (mp_X509SubjectName != NULL)
		XSEC_RELEASE_XMLCH(mp_X509SubjectName);

	X509ListType::iterator i;

	for (i = m_X509List.begin(); i != m_X509List.end(); ++i) {

		if ((*i)->mp_cryptoX509 != NULL)
			delete ((*i)->mp_cryptoX509);
		delete (*i);

	}

	m_X509List.clear();

	if (mp_rawRetrievalURI != NULL)
		XSEC_RELEASE_XMLCH(mp_rawRetrievalURI);

	
};

// --------------------------------------------------------------------------------
//           Load and Get
// --------------------------------------------------------------------------------


// Methods unique to DSIGKeyInfoX509

void DSIGKeyInfoX509::load(void) {

	// Assuming we have a valid DOM_Node to start with, load the signing key so that it can
	// be used later on

	if (mp_keyInfoDOMNode == NULL) {

		// Attempt to load an empty signature element
		throw XSECException(XSECException::LoadEmptyX509);

	}

	if (!strEquals(getDSIGLocalName(mp_keyInfoDOMNode), "X509Data")) {

		throw XSECException(XSECException::LoadNonX509);

	}

	// Now check for an X509 Data Element we understand

	DOMNode *tmpElt = mp_keyInfoDOMNode->getFirstChild();
	DOMNode *child;	// Worker

	while (tmpElt != 0) {

		if (tmpElt->getNodeType() == DOMNode::ELEMENT_NODE) {

			// See if it's a known element type
			if (strEquals(getDSIGLocalName(tmpElt), "X509Certificate")) {

				X509Holder * h;

				DOMNode *certElt = findFirstChildOfType(tmpElt, DOMNode::TEXT_NODE);

				if (certElt != 0) {
	
					XSECnew(h, X509Holder);

					// Add to the list
					
					m_X509List.push_back(h);

					h->mp_encodedX509 = certElt->getNodeValue();
					h->mp_cryptoX509 = XSECPlatformUtils::g_cryptoProvider->X509();
					char * charX509 = XMLString::transcode(h->mp_encodedX509);
					ArrayJanitor<char> j_charX509(charX509);
					h->mp_cryptoX509->loadX509Base64Bin(charX509, (int) strlen(charX509));

				}
			}

			else if (strEquals(getDSIGLocalName(tmpElt), "X509SubjectName")) {

				child = findFirstChildOfType(tmpElt, DOMNode::TEXT_NODE);

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected TEXT_NODE child of <X509SubjectName>");

				}

				mp_X509SubjectName = decodeDName(child->getNodeValue());

			}

			else if (strEquals(getDSIGLocalName(tmpElt), "X509IssuerSerial")) {

				child = tmpElt->getFirstChild();
				while (child != 0 && child->getNodeType() != DOMNode::ELEMENT_NODE &&
					!strEquals(getDSIGLocalName(child), "X509IssuerName"))
					child = child->getNextSibling();

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected <X509IssuerName> child of <X509IssuerSerial>");

				}

				child = child->getFirstChild();
				while (child != 0 && child->getNodeType() != DOMNode::TEXT_NODE)
					child = child->getNextSibling();

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected TEXT_NODE child of <X509IssuerSerial>");

				}

				mp_X509IssuerName = decodeDName(child->getNodeValue());

				// Now find the serial number
				child = tmpElt->getFirstChild();
				while (child != 0 && (child->getNodeType() != DOMNode::ELEMENT_NODE ||
					!strEquals(getDSIGLocalName(child), "X509SerialNumber")))
					child = child->getNextSibling();

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected <X509SerialNumber> child of <X509IssuerSerial>");

				}

				child = child->getFirstChild();
				while (child != 0 && child->getNodeType() != DOMNode::TEXT_NODE)
					child = child->getNextSibling();

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected TEXT_NODE child of <X509IssuerSerial>");

				}

				mp_X509SerialNumber = child->getNodeValue();

			}

			else if (strEquals(getDSIGLocalName(tmpElt), "X509CRL")) {

				child = findFirstChildOfType(tmpElt, DOMNode::TEXT_NODE);

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected TEXT_NODE child of <X509CRL>");

				}

				mp_X509CRLTextNode = child;
				mp_X509CRL = child->getNodeValue();

			}
			else if (strEquals(getDSIGLocalName(tmpElt), "X509SKI")) {

				child = findFirstChildOfType(tmpElt, DOMNode::TEXT_NODE);

				if (child == NULL) {

					throw XSECException(XSECException::ExpectedDSIGChildNotFound,
						"Expected TEXT_NODE child of <X509SKI>");

				}

				mp_X509SKITextNode = child;
				mp_X509SKI = child->getNodeValue();

			}
		}

		// Go to next data element to load if we understand

		tmpElt = tmpElt->getNextSibling();

	}

}

const XMLCh * DSIGKeyInfoX509::getKeyName(void) {

	return mp_X509SubjectName;

	// Should return DN

}

const XMLCh * DSIGKeyInfoX509::getX509SubjectName(void) {


	return mp_X509SubjectName;

}

const XMLCh * DSIGKeyInfoX509::getX509IssuerName(void) {

	return mp_X509IssuerName;

}

const XMLCh * DSIGKeyInfoX509::getX509CRL(void) {

	return mp_X509CRL;

}

const XMLCh * DSIGKeyInfoX509::getX509SKI(void) {

	return mp_X509SKI;

}

const XMLCh * DSIGKeyInfoX509::getX509IssuerSerialNumber(void) {

	return mp_X509SerialNumber;

}

int DSIGKeyInfoX509::getCertificateListSize(void) {

	return (int) m_X509List.size();

}



const XMLCh * DSIGKeyInfoX509::getCertificateItem(int item) {

	if (item >=0 && (unsigned int) item < m_X509List.size())
		return m_X509List[item]->mp_encodedX509;

	return 0;

}

XSECCryptoX509 * DSIGKeyInfoX509::getCertificateCryptoItem(int item) {

	if (item >=0 && (unsigned int) item < m_X509List.size())
		return m_X509List[item]->mp_cryptoX509;

	return 0;
}

const XMLCh * DSIGKeyInfoX509::getRawRetrievalURI(void) {

	return mp_rawRetrievalURI;

}

// --------------------------------------------------------------------------------
//           Create and Set
// --------------------------------------------------------------------------------

DOMElement * DSIGKeyInfoX509::createBlankX509Data(void) {

	// Create the DOM Structure

	safeBuffer str;
	DOMDocument *doc = mp_env->getParentDocument();
	const XMLCh * prefix = mp_env->getDSIGNSPrefix();

	makeQName(str, prefix, "X509Data");

	DOMElement *ret = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
	mp_keyInfoDOMNode = ret;
	mp_env->doPrettyPrint(ret);

	return ret;

}

void DSIGKeyInfoX509::setX509CRL(const XMLCh * crl) {

	if (mp_X509CRLTextNode == 0) {

		safeBuffer str;
		DOMDocument *doc = mp_env->getParentDocument();
		const XMLCh * prefix = mp_env->getDSIGNSPrefix();

		makeQName(str, prefix, "X509CRL");

		DOMElement * s = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		mp_X509CRLTextNode = doc->createTextNode(crl);
		s->appendChild(mp_X509CRLTextNode);

		// Add to the over-arching node
		mp_keyInfoDOMNode->appendChild(s);
		mp_env->doPrettyPrint(mp_keyInfoDOMNode);

	}

	else {

		mp_X509CRLTextNode->setNodeValue(crl);

	}

	mp_X509CRL = mp_X509CRLTextNode->getNodeValue();

}

void DSIGKeyInfoX509::setX509SKI(const XMLCh * ski) {

	if (mp_X509SKITextNode == 0) {

		safeBuffer str;
		DOMDocument *doc = mp_env->getParentDocument();
		const XMLCh * prefix = mp_env->getDSIGNSPrefix();

		makeQName(str, prefix, "X509SKI");

		DOMElement * s = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		mp_X509SKITextNode = doc->createTextNode(ski);
		s->appendChild(mp_X509SKITextNode);

		// Add to the over-arching node
		mp_keyInfoDOMNode->appendChild(s);
		mp_env->doPrettyPrint(mp_keyInfoDOMNode);

	}

	else {

		mp_X509SKITextNode->setNodeValue(ski);

	}

	mp_X509SKI = mp_X509SKITextNode->getNodeValue();

}

void DSIGKeyInfoX509::setX509SubjectName(const XMLCh * name) {

	if (mp_X509SubjectName != NULL)
		XSEC_RELEASE_XMLCH(mp_X509SubjectName);

	mp_X509SubjectName = XMLString::replicate(name);
	
	XMLCh * encodedName = encodeDName(name);
	ArrayJanitor<XMLCh> j_encodedName(encodedName);

	if (mp_X509SubjectNameTextNode == 0) {

		// Does not yet exist in the DOM

		safeBuffer str;
		DOMDocument *doc = mp_env->getParentDocument();
		const XMLCh * prefix = mp_env->getDSIGNSPrefix();

		makeQName(str, prefix, "X509SubjectName");

		DOMElement * s = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		mp_X509SubjectNameTextNode = doc->createTextNode(encodedName);
		s->appendChild(mp_X509SubjectNameTextNode);

		// Add to the over-arching node
		mp_keyInfoDOMNode->appendChild(s);
		mp_env->doPrettyPrint(mp_keyInfoDOMNode);

	}

	else {

		mp_X509SubjectNameTextNode->setNodeValue(encodedName);

	}
}

void DSIGKeyInfoX509::setX509IssuerSerial(const XMLCh * name, const XMLCh * serial) {

	if (mp_X509IssuerName != NULL)
		XSEC_RELEASE_XMLCH(mp_X509IssuerName);

	mp_X509IssuerName = XMLString::replicate(name);
	
	XMLCh * encodedName = encodeDName(name);
	ArrayJanitor<XMLCh> j_encodedName(encodedName);

	if (mp_X509IssuerNameTextNode == 0) {

		// Does not yet exist in the DOM

		safeBuffer str;
		DOMDocument *doc = mp_env->getParentDocument();
		const XMLCh * prefix = mp_env->getDSIGNSPrefix();

		makeQName(str, prefix, "X509IssuerSerial");

		DOMElement * s = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		mp_env->doPrettyPrint(s);

		// Create the text nodes with the contents

		mp_X509IssuerNameTextNode = doc->createTextNode(encodedName);
		mp_X509SerialNumberTextNode = doc->createTextNode(serial);
	
		// Create the sub elements

		makeQName(str, prefix, "X509IssuerName");
		DOMElement * t = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		t->appendChild(mp_X509IssuerNameTextNode);
		
		s->appendChild(t);
		mp_env->doPrettyPrint(s);
		
		makeQName(str, prefix, "X509SerialNumber");
		t = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
		t->appendChild(mp_X509SerialNumberTextNode);
		
		s->appendChild(t);
		mp_env->doPrettyPrint(s);

		// Add to the over-arching X509Data

		mp_keyInfoDOMNode->appendChild(s);
		mp_env->doPrettyPrint(mp_keyInfoDOMNode);

	}

	else {

		mp_X509IssuerNameTextNode->setNodeValue(encodedName);
		mp_X509SerialNumberTextNode->setNodeValue(serial);

	}

}

void DSIGKeyInfoX509::setRawRetrievalURI(const XMLCh * uri) {

	if (mp_rawRetrievalURI != NULL)
		XSEC_RELEASE_XMLCH(mp_rawRetrievalURI);

	mp_rawRetrievalURI = XMLString::replicate(uri);

}

void DSIGKeyInfoX509::appendX509Certificate(const XMLCh * base64Certificate) {

	safeBuffer str;
	DOMDocument *doc = mp_env->getParentDocument();
	const XMLCh * prefix = mp_env->getDSIGNSPrefix();

	makeQName(str, prefix, "X509Certificate");

	DOMElement * s = doc->createElementNS(DSIGConstants::s_unicodeStrURIDSIG, str.rawXMLChBuffer());
	DOMNode * b64Txt = doc->createTextNode(base64Certificate);
	s->appendChild(b64Txt);

	mp_keyInfoDOMNode->appendChild(s);
	mp_env->doPrettyPrint(mp_keyInfoDOMNode);

	// Add to the list

	X509Holder * h;
	XSECnew(h, X509Holder);
	m_X509List.push_back(h);
	h->mp_encodedX509 = b64Txt->getNodeValue();
	h->mp_cryptoX509 = XSECPlatformUtils::g_cryptoProvider->X509();
	char * charX509 = XMLString::transcode(h->mp_encodedX509);
	ArrayJanitor<char> j_charX509(charX509);
	h->mp_cryptoX509->loadX509Base64Bin(charX509, (unsigned int) strlen(charX509));
	
}
