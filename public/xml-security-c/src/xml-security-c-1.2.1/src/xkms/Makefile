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

OBJ_FILES= ../../lib/obj/XKMSConstants.o
INCLUDE_FILES= XKMSAuthentication.hpp XKMSCompoundRequest.hpp XKMSCompoundResult.hpp XKMSConstants.hpp XKMSKeyBinding.hpp XKMSKeyBindingAbstractType.hpp XKMSLocateRequest.hpp XKMSLocateResult.hpp XKMSMessageAbstractType.hpp XKMSMessageFactory.hpp XKMSNotBoundAuthentication.hpp XKMSPendingRequest.hpp XKMSPrototypeKeyBinding.hpp XKMSQueryKeyBinding.hpp XKMSRegisterRequest.hpp XKMSRegisterResult.hpp XKMSRequestAbstractType.hpp XKMSRespondWith.hpp XKMSResponseMechanism.hpp XKMSResult.hpp XKMSResultType.hpp XKMSStatus.hpp XKMSStatusRequest.hpp XKMSStatusResult.hpp XKMSUnverifiedKeyBinding.hpp XKMSUseKeyWith.hpp XKMSValidateRequest.hpp XKMSValidateResult.hpp XKMSValidityInterval.hpp
SRC_BASE=..
XSEC_INCL=-I${SRC_BASE}/../include

OBJDIR=${SRC_BASE}/../lib/obj
INCLUDE_DIR=${SRC_BASE}/../include/xsec/xkms

prefix = /usr/local
includedir=${prefix}/include
INSTALL_DIR=${includedir}/xsec/xkms

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
