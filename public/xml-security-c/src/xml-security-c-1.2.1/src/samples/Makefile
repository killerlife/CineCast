# Copyright 2002-2005 The Apache Software Foundation.
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

DEPFILE=depends

#all these setting come from the arguments passed in to runConfigure.

PLATFORM = LINUX

CC  = gcc
CXX = g++
CXXFLAGS =  -O2 -DNDEBUG
CPPFLAGS = -I/root/CineCast/public/openssl/include -I/root/CineCast/public/xerces-c/include -fPIC -I/root/CineCast/public/xerces-c/src/xerces-c-src_2_7_0//src

PREFIX = /usr/local
LDFLAGS = -L/root/CineCast/public/openssl/lib -L/root/CineCast/public/xerces-c/lib
LIBS = -lm  -lcrypto
OBJSUFFIX = .o
SHLIBSUFFIX = .so

CC1 = g++  -O2 -DNDEBUG -Wall -fPIC -DLINUX
CC4 = gcc  -O2 -DNDEBUG -Wall -fPIC -DLINUX
MAKE_SHARED = g++  -O2 -DNDEBUG -Wl,-soname,$(LIBNAME) -DLINUX -shared -fPIC
LINK = g++  -O2 -DNDEBUG -DLINUX -ldl -fPIC

LN_S = ln -s

# Packaging

PACKAGE_NAME = XML-Security-C
PACKAGE_TARNAME = xml-security-c
PACKAGE_VERSION = 1.2.0

# Directory specific
SRC_BASE=..
XSEC_INCL=-I${SRC_BASE}/../include

LIBDIR=${SRC_BASE}/../lib
BINDIR=${SRC_BASE}/../bin
OBJDIR=${BINDIR}/obj

LDXSEC=-L${LIBDIR} -l${PACKAGE_TARNAME}

allsamples: ${BINDIR}/simpleHMAC \
          ${BINDIR}/simpleValidate \
          ${BINDIR}/simpleEncrypt \
          ${BINDIR}/simpleDecrypt

#========================== simpleHMAC ==========================

#${OBJDIR}/xtest${OBJSUFFIX}: xtest/xtest.cpp
#	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/simpleHMAC: ${OBJDIR}/simpleHMAC${OBJSUFFIX} ${OBJDIR}/IOStreamOutputter${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#========================= simpleValidate ========================

#${OBJDIR}/xtest${OBJSUFFIX}: xtest/xtest.cpp
#	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/simpleValidate: ${OBJDIR}/simpleValidate${OBJSUFFIX} ${OBJDIR}/IOStreamOutputter${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#========================= simpleEncrypt ========================

${BINDIR}/simpleEncrypt: ${OBJDIR}/simpleEncrypt${OBJSUFFIX} ${OBJDIR}/IOStreamOutputter${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#========================= simpleDecrypt ========================

${BINDIR}/simpleDecrypt: ${OBJDIR}/simpleDecrypt${OBJSUFFIX} ${OBJDIR}/IOStreamOutputter${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#============================= Clean =============================

distclean:
	rm -f Makefile.bak
	rm -f Makefile

# Include Library pattern rules

include ${SRC_BASE}/Makefile.incl
