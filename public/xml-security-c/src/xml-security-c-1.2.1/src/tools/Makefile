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
PACKAGE_VERSION_MAJOR = 1

# Directory specific
SRC_BASE=..
XSEC_INCL=-I${SRC_BASE}/../include

LIBDIR=${SRC_BASE}/../lib
BINDIR=${SRC_BASE}/../bin
OBJDIR=${BINDIR}/obj

LDXSEC=-L${LIBDIR} -l${PACKAGE_TARNAME}

alltools: ${BINDIR}/xtest \
          ${BINDIR}/c14n \
          ${BINDIR}/checksig \
          ${BINDIR}/templatesign \
		  ${BINDIR}/txfmout \
		  ${BINDIR}/siginf \
		  ${BINDIR}/cipher \
		  ${BINDIR}/xklient

compile: ${OBJ_FILES}

#============================= xtest =============================

${OBJDIR}/xtest${OBJSUFFIX}: xtest/xtest.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/xtest: ${OBJDIR}/xtest${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@

#============================== c14n =============================

${OBJDIR}/c14n${OBJSUFFIX}: c14n/c14n.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/c14n: ${OBJDIR}/c14n${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@


#============================ checksig ===========================

${OBJDIR}/checksig${OBJSUFFIX}: checksig/checksig.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${OBJDIR}/AnonymousResolver${OBJSUFFIX}: checksig/AnonymousResolver.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${OBJDIR}/InteropResolver${OBJSUFFIX}: checksig/InteropResolver.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/checksig: ${OBJDIR}/checksig${OBJSUFFIX} ${OBJDIR}/AnonymousResolver${OBJSUFFIX} ${OBJDIR}/InteropResolver${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#========================== templatesign =========================

${OBJDIR}/templatesign${OBJSUFFIX}: templatesign/templatesign.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/templatesign: ${OBJDIR}/templatesign${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@

#============================= txfmout ============================

${OBJDIR}/txfmout${OBJSUFFIX}: txfmout/txfmout.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/txfmout: ${OBJDIR}/txfmout${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@

#============================= siginf =============================

${OBJDIR}/siginf${OBJSUFFIX}: siginf/siginf.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/siginf: ${OBJDIR}/siginf${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@

#============================= cipher =============================

${OBJDIR}/cipher${OBJSUFFIX}: cipher/cipher.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${OBJDIR}/XencInteropResolver${OBJSUFFIX}: cipher/XencInteropResolver.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/cipher: ${OBJDIR}/cipher${OBJSUFFIX} ${OBJDIR}/XencInteropResolver${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $^ $(LDXSEC) $(LIBS) -o $@

#============================= xklient =============================

${OBJDIR}/xklient${OBJSUFFIX}: xklient/xklient.cpp
	${CC1} -c ${CPPFLAGS} ${XSEC_INCL} -o ${@} $<

${BINDIR}/xklient: ${OBJDIR}/xklient${OBJSUFFIX}
	$(LINK) $(LDFLAGS) $< $(LDXSEC) $(LIBS) -o $@

#============================= Clean =============================

distclean:
	rm -f Makefile.bak
	rm -f Makefile

# Include Library pattern rules

include ${SRC_BASE}/Makefile.incl
