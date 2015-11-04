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

DEPFILE=depends

#all these setting come from the arguments passed in to runConfigure.

PLATFORM = LINUX

CC  = gcc
CXX = g++
CXXFLAGS =  -O2 -DNDEBUG
CPPFLAGS = -I/root/CineCast/public/openssl/include -I/root/CineCast/public/xerces-c/include -fPIC -I/root/CineCast/public/xerces-c/src/xerces-c-src_2_7_0//src

PREFIX = /usr/local
LDFLAGS = -L/root/CineCast/public/openssl/lib -L/root/CineCast/public/xerces-c/lib
OBJSUFFIX = .o
SHLIBSUFFIX = .so

CC1 = g++  -O2 -DNDEBUG -Wall -fPIC -DLINUX
CC4 = gcc  -O2 -DNDEBUG -Wall -fPIC -DLINUX
MAKE_SHARED = g++  -O2 -DNDEBUG -Wl,-soname,$(LIBNAME) -DLINUX -shared -fPIC
LINK = g++  -O2 -DNDEBUG -DLINUX -ldl -fPIC

######################### Commands ###########################

LN_S = ln -s
MKDIR = mkdir -p
CP = cp
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

######################### Directories ###########################

OBJ_FILES= ../../lib/obj/DSIGConstants.o ../../lib/obj/DSIGKeyInfoList.o ../../lib/obj/DSIGKeyInfoMgmtData.o ../../lib/obj/DSIGKeyInfoName.o ../../lib/obj/DSIGKeyInfoPGPData.o ../../lib/obj/DSIGKeyInfoSPKIData.o ../../lib/obj/DSIGKeyInfoValue.o ../../lib/obj/DSIGKeyInfoX509.o ../../lib/obj/DSIGObject.o ../../lib/obj/DSIGReference.o ../../lib/obj/DSIGReferenceList.o ../../lib/obj/DSIGSignature.o ../../lib/obj/DSIGSignedInfo.o ../../lib/obj/DSIGTransform.o ../../lib/obj/DSIGTransformBase64.o ../../lib/obj/DSIGTransformC14n.o ../../lib/obj/DSIGTransformEnvelope.o ../../lib/obj/DSIGTransformList.o ../../lib/obj/DSIGTransformXPath.o ../../lib/obj/DSIGTransformXPathFilter.o ../../lib/obj/DSIGTransformXSL.o ../../lib/obj/DSIGXPathFilterExpr.o ../../lib/obj/DSIGXPathHere.o
INCLUDE_FILES= DSIGConstants.hpp DSIGKeyInfo.hpp DSIGKeyInfoList.hpp DSIGKeyInfoMgmtData.hpp DSIGKeyInfoName.hpp DSIGKeyInfoPGPData.hpp DSIGKeyInfoSPKIData.hpp DSIGKeyInfoValue.hpp DSIGKeyInfoX509.hpp DSIGObject.hpp DSIGReference.hpp DSIGReferenceList.hpp DSIGSignature.hpp DSIGSignedInfo.hpp DSIGTransform.hpp DSIGTransformBase64.hpp DSIGTransformC14n.hpp DSIGTransformEnvelope.hpp DSIGTransformList.hpp DSIGTransformXPath.hpp DSIGTransformXPathFilter.hpp DSIGTransformXSL.hpp DSIGXPathFilterExpr.hpp DSIGXPathHere.hpp
SRC_BASE=..
XSEC_INCL=-I${SRC_BASE}/../include

OBJDIR=${SRC_BASE}/../lib/obj
INCLUDE_DIR=${SRC_BASE}/../include/xsec/dsig

prefix = /usr/local
includedir=${prefix}/include
INSTALL_DIR=${includedir}/xsec/dsig

########################### Action #############################

compile: ${OBJ_FILES}

includes:
	$(MKDIR) $(INCLUDE_DIR)
	$(CP) -p $(INCLUDE_FILES) $(INCLUDE_DIR)

distclean:
	rm -f Makefile.in
	rm -f Makefile.bak
	rm -f Makefile

install:
	$(INSTALL) -d $(DESTDIR)$(INSTALL_DIR)
	$(INSTALL_DATA) $(INCLUDE_FILES) $(DESTDIR)$(INSTALL_DIR)

# Include Library pattern rules

include ${SRC_BASE}/Makefile.incl
