# Copyright 2002-2004 The Apache Software Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# All these setting come from the configure script
# We use configure to do most of the hard work as it is more
# designed to handle different system complexity than make
#

######################## CONFIGURE DEFINITIONS ##########################


PLATFORM = LINUX

CC  = gcc
CXX = g++
CXXFLAGS =  -O2 -DNDEBUG
CPPFLAGS = -I/root/CineCast/public/openssl/include -I/root/CineCast/public/xerces-c/include -fPIC -I/root/CineCast/public/xerces-c/src/xerces-c-src_2_7_0//src

PREFIX = /usr/local
LDFLAGS = -L/root/CineCast/public/openssl/lib -L/root/CineCast/public/xerces-c/lib
LIBS=-lm  -lcrypto -lxecres-c -lxecres-depdom
OBJSUFFIX = .o
SHLIBSUFFIX = .so

CC1 = g++  -O2 -DNDEBUG -Wall -fPIC -DLINUX
CC4 = gcc  -O2 -DNDEBUG -Wall -fPIC -DLINUX
MAKE_SHARED = g++  -O2 -DNDEBUG -Wl,-soname,$(LIBNAME) -DLINUX -shared -fPIC
LINK = g++  -O2 -DNDEBUG -DLINUX -ldl -fPIC

LN_S = ln -s
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

# Packaging

PACKAGE_NAME = XML-Security-C
PACKAGE_TARNAME = xml-security-c
PACKAGE_VERSION = 1.2.0
PACKAGE_VERSION_MAJOR = 1

# Library Files
LIB_OBJ_FILES =  ../lib/obj/XSECC14n20010315.o ../lib/obj/XSECCannon.o ../lib/obj/TXFMBase.o ../lib/obj/TXFMBase64.o ../lib/obj/TXFMC14n.o ../lib/obj/TXFMChain.o ../lib/obj/TXFMCipher.o ../lib/obj/TXFMConcatChains.o ../lib/obj/TXFMDocObject.o ../lib/obj/TXFMEnvelope.o ../lib/obj/TXFMMD5.o ../lib/obj/TXFMOutputFile.o ../lib/obj/TXFMParser.o ../lib/obj/TXFMSB.o ../lib/obj/TXFMSHA1.o ../lib/obj/TXFMURL.o ../lib/obj/TXFMXPath.o ../lib/obj/TXFMXPathFilter.o ../lib/obj/TXFMXSL.o ../lib/obj/DSIGConstants.o ../lib/obj/DSIGKeyInfoList.o ../lib/obj/DSIGKeyInfoMgmtData.o ../lib/obj/DSIGKeyInfoName.o ../lib/obj/DSIGKeyInfoPGPData.o ../lib/obj/DSIGKeyInfoSPKIData.o ../lib/obj/DSIGKeyInfoValue.o ../lib/obj/DSIGKeyInfoX509.o ../lib/obj/DSIGObject.o ../lib/obj/DSIGReference.o ../lib/obj/DSIGReferenceList.o ../lib/obj/DSIGSignature.o ../lib/obj/DSIGSignedInfo.o ../lib/obj/DSIGTransform.o ../lib/obj/DSIGTransformBase64.o ../lib/obj/DSIGTransformC14n.o ../lib/obj/DSIGTransformEnvelope.o ../lib/obj/DSIGTransformList.o ../lib/obj/DSIGTransformXPath.o ../lib/obj/DSIGTransformXPathFilter.o ../lib/obj/DSIGTransformXSL.o ../lib/obj/DSIGXPathFilterExpr.o ../lib/obj/DSIGXPathHere.o ../lib/obj/XSECCryptoBase64.o ../lib/obj/XSECCryptoException.o ../lib/obj/XSECCryptoUtils.o ../lib/obj/XSECCryptoX509.o ../lib/obj/XSECKeyInfoResolverDefault.o ../lib/obj/XSECAlgorithmMapper.o ../lib/obj/XSECEnv.o ../lib/obj/XSECError.o ../lib/obj/XSECException.o ../lib/obj/XSECProvider.o ../lib/obj/XSECURIResolverXerces.o ../lib/obj/XSECBinTXFMInputStream.o ../lib/obj/XSECDOMUtils.o ../lib/obj/XSECNameSpaceExpander.o ../lib/obj/XSECPlatformUtils.o ../lib/obj/XSECSOAPRequestorSimple.o ../lib/obj/XSECSafeBuffer.o ../lib/obj/XSECSafeBufferFormatter.o ../lib/obj/XSECTXFMInputSource.o ../lib/obj/XSECXPathNodeList.o  ../lib/obj/XKMSConstants.o ../lib/obj/OpenSSLCryptoBase64.o ../lib/obj/OpenSSLCryptoHash.o ../lib/obj/OpenSSLCryptoHashHMAC.o ../lib/obj/OpenSSLCryptoKeyDSA.o ../lib/obj/OpenSSLCryptoKeyHMAC.o ../lib/obj/OpenSSLCryptoKeyRSA.o ../lib/obj/OpenSSLCryptoProvider.o ../lib/obj/OpenSSLCryptoSymmetricKey.o ../lib/obj/OpenSSLCryptoX509.o ../lib/obj/XSCryptCryptoBase64.o ../lib/obj/XSECBinHTTPURIInputStream.o ../lib/obj/XSECSOAPRequestorSimpleUnix.o ../lib/obj/XSECURIResolverGenericUnix.o ../lib/obj/XENCAlgorithmHandlerDefault.o ../lib/obj/XENCCipherDataImpl.o ../lib/obj/XENCCipherImpl.o ../lib/obj/XENCCipherReferenceImpl.o ../lib/obj/XENCCipherValueImpl.o ../lib/obj/XENCEncryptedDataImpl.o ../lib/obj/XENCEncryptedKeyImpl.o ../lib/obj/XENCEncryptedTypeImpl.o ../lib/obj/XENCEncryptionMethodImpl.o ../lib/obj/XKMSAuthenticationImpl.o ../lib/obj/XKMSCompoundRequestImpl.o ../lib/obj/XKMSCompoundResultImpl.o ../lib/obj/XKMSKeyBindingAbstractTypeImpl.o ../lib/obj/XKMSKeyBindingImpl.o ../lib/obj/XKMSLocateRequestImpl.o ../lib/obj/XKMSLocateResultImpl.o ../lib/obj/XKMSMessageAbstractTypeImpl.o ../lib/obj/XKMSMessageFactoryImpl.o ../lib/obj/XKMSNotBoundAuthentication.o ../lib/obj/XKMSPendingRequestImpl.o ../lib/obj/XKMSPrototypeKeyBindingImpl.o ../lib/obj/XKMSQueryKeyBindingImpl.o ../lib/obj/XKMSRegisterRequestImpl.o ../lib/obj/XKMSRegisterResultImpl.o ../lib/obj/XKMSRequestAbstractTypeImpl.o ../lib/obj/XKMSRespondWithImpl.o ../lib/obj/XKMSResponseMechanismImpl.o ../lib/obj/XKMSResultImpl.o ../lib/obj/XKMSResultTypeImpl.o ../lib/obj/XKMSStatusImpl.o ../lib/obj/XKMSStatusRequestImpl.o ../lib/obj/XKMSStatusResultImpl.o ../lib/obj/XKMSUnverifiedKeyBindingImpl.o ../lib/obj/XKMSUseKeyWithImpl.o ../lib/obj/XKMSValidateRequestImpl.o ../lib/obj/XKMSValidateResultImpl.o ../lib/obj/XKMSValidityIntervalImpl.o

# MACROS defining commands
MKDIR=mkdir -p

######################## Build Directories ##########################

# SRC Directories

CANON_DIR = canon
TRANSFORMERS_DIR = transformers
UTILS_DIR = utils
UNIXUTILS_DIR = utils/unixutils
ENC_DIR = enc
FRAMEWORK_DIR = framework
DSIG_DIR = dsig
OPENSSL_DIR = enc/OpenSSL
XSCRYPT_DIR = enc/XSCrypt
XENC_DIR = xenc
XENCIMPL_DIR = xenc/impl
XKMS_DIR = xkms
XKMSIMPL_DIR = xkms/impl

# Utility SRC directories

TOOLS_DIR = tools
SAMPLES_DIR = samples

######################## Install Directories ##########################

prefix = /usr/local
exec_prefix = ${prefix}
includedir = ${prefix}/include
libdir = ${exec_prefix}/lib
bindir = ${exec_prefix}/bin
infodir = ${prefix}/info
mandir = ${prefix}/man

######################## Library Definition ##########################


# Define the Library

LIB_DIR = ../lib
LIB_OBJECTS_DIR = $(LIB_DIR)/obj

# Actual names are defined by configure

LIBNAME = libxml-security-c.so.12.0
LIBSHORTNAME = libxml-security-c.so
LIBMAJORNAME = libxml-security-c.so.12

# Our build target

THISLIB = ${LIB_DIR}/${LIBNAME}

BINTARGETDIR = ../bin

######################## RULES ##########################


# Now the actual compilation

all: lib alltools allsamples

lib: ${THISLIB}

${THISLIB}: $(LIB_OBJ_FILES)
	$(MAKE_SHARED) $(LDFLAGS) $(LIB_OBJ_FILES) $(LIBS) -o $@
	(cd $(LIB_DIR) ; rm -f $(LIBMAJORNAME) ; $(LN_S) $(LIBNAME) $(LIBMAJORNAME))
	(cd $(LIB_DIR) ; rm -f $(LIBSHORTNAME) ; $(LN_S) $(LIBNAME) $(LIBSHORTNAME))

$(LIB_OBJ_FILES): compile

compile: prepare
	$(MAKE) -C ${CANON_DIR} compile
	$(MAKE) -C ${TRANSFORMERS_DIR} compile
	$(MAKE) -C ${DSIG_DIR} compile
	$(MAKE) -C ${ENC_DIR} compile
	$(MAKE) -C ${OPENSSL_DIR} compile
	$(MAKE) -C ${XSCRYPT_DIR} compile
	$(MAKE) -C ${FRAMEWORK_DIR} compile
	$(MAKE) -C ${UTILS_DIR} compile
	$(MAKE) -C ${UNIXUTILS_DIR} compile
	$(MAKE) -C ${XENC_DIR} compile
	$(MAKE) -C ${XENCIMPL_DIR} compile
	$(MAKE) -C ${XKMS_DIR} compile
	$(MAKE) -C ${XKMSIMPL_DIR} compile

depends:
	$(MAKE) -C ${CANON_DIR} -s depends
	$(MAKE) -C ${TRANSFORMERS_DIR} -s depends
	$(MAKE) -C ${DSIG_DIR} -s depends
	$(MAKE) -C ${ENC_DIR} -s depends
	$(MAKE) -C ${OPENSSL_DIR} -s depends
	$(MAKE) -C ${XSCRYPT_DIR} -s depends
	$(MAKE) -C ${FRAMEWORK_DIR} -s depends
	$(MAKE) -C ${UTILS_DIR} -s depends
	$(MAKE) -C ${UNIXUTILS_DIR} -s depends
	$(MAKE) -C ${XKMS_DIR} -s depends
	$(MAKE) -C ${XKMSIMPL_DIR} -s depends

prepare:
	$(MKDIR) ../lib
	$(MKDIR) ../lib/obj
	$(MKDIR) ../bin
	$(MKDIR) ../bin/obj
	$(MAKE) -C ${CANON_DIR} includes
	$(MAKE) -C ${TRANSFORMERS_DIR} includes
	$(MAKE) -C ${DSIG_DIR} includes
	$(MAKE) -C ${ENC_DIR} includes
	$(MAKE) -C ${OPENSSL_DIR} includes
	$(MAKE) -C ${XSCRYPT_DIR} includes
	$(MAKE) -C ${FRAMEWORK_DIR} includes
	$(MAKE) -C ${UTILS_DIR} includes
	$(MAKE) -C ${UNIXUTILS_DIR} includes
	$(MAKE) -C ${XENC_DIR} includes
	$(MAKE) -C ${XKMS_DIR} includes

alltools: lib
	$(MAKE) -C ${TOOLS_DIR} alltools

allsamples: lib
	$(MAKE) -C ${SAMPLES_DIR} allsamples


####################### INSTALL ##########################

install: lib
	$(MAKE) -C ${CANON_DIR} install
	$(MAKE) -C ${TRANSFORMERS_DIR} install
	$(MAKE) -C ${DSIG_DIR} install
	$(MAKE) -C ${ENC_DIR} install
	$(MAKE) -C ${OPENSSL_DIR} install
	$(MAKE) -C ${XSCRYPT_DIR} install
	$(MAKE) -C ${FRAMEWORK_DIR} install
	$(MAKE) -C ${UTILS_DIR} install
	$(MAKE) -C ${UNIXUTILS_DIR} install
	$(MAKE) -C ${XENC_DIR} install
	$(MAKE) -C ${XKMS_DIR} install
	$(INSTALL) ${THISLIB} $(DESTDIR)${libdir}
	$(RM) $(DESTDIR)${libdir}/${LIBMAJORNAME} && $(LN_S) ${LIBNAME} $(DESTDIR)${libdir}/${LIBMAJORNAME}
	$(RM) $(DESTDIR)${libdir}/${LIBSHORTNAME} && $(LN_S) ${LIBNAME} $(DESTDIR)${libdir}/${LIBSHORTNAME}

######################## CLEAN  ##########################


clean:
	rm -rf ../lib/*
	rm -rf ../bin/*
	rm -rf ../include/*

distclean: clean
	$(MAKE) -C ${CANON_DIR} distclean
	$(MAKE) -C ${TRANSFORMERS_DIR} distclean
	$(MAKE) -C ${DSIG_DIR} distclean
	$(MAKE) -C ${ENC_DIR} distclean
	$(MAKE) -C ${OPENSSL_DIR} distclean
	$(MAKE) -C ${XSCRYPT_DIR} distclean
	$(MAKE) -C ${FRAMEWORK_DIR} distclean
	$(MAKE) -C ${UTILS_DIR} distclean
	$(MAKE) -C ${UNIXUTILS_DIR} distclean
	$(MAKE) -C ${TOOLS_DIR} distclean
	$(MAKE) -C ${XENC_DIR} distclean
	$(MAKE) -C ${XENCIMPL_DIR} distclean
	$(MAKE) -C ${XKMS_DIR} distclean
	$(MAKE) -C ${XKMSIMPL_DIR} distclean
	rm -f config.cache
	rm -f config.status
	rm -f config.log
	rm -rf autom4te.cache
	rm -f framework/XSECConfig.hpp
	rm -f Makefile
	rm -f Makefile.bak

