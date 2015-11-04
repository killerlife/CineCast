# Generated automatically from Makefile.in by configure.
#
# Copyright 2002,2004 The Apache Software Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
# $Log$
# Revision 1.5  2004/09/08 13:56:38  peiyongz
# Apache License Version 2.0
#
# Revision 1.4  2004/07/09 16:19:33  peiyongz
# Build on HP-Itanium, aCC A.05.52
#
# Revision 1.3  2003/02/23 05:44:12  jberry
# Ripple through changes of BeOSDefs.h name change
#
# Revision 1.2  2002/12/31 19:30:56  tng
# [Bug 15590] BeOSDefs.hpp has wrong case in CVS.
#
# Revision 1.1  2002/12/02 20:42:30  tng
# [Bug 12490] Patches required to build Xerces-C++ on BeOS R5.  Patch from Andrew Bachmann.
#
#

PLATFORM = LINUX
CC  = gcc
CXX = c++
CXXVER = 
GCC = yes
GXX = yes
CXXFLAGS =   -w -O -DNDEBUG -DPROJ_XMLPARSER  -DPROJ_XMLUTIL  -DPROJ_PARSERS  -DPROJ_SAX4C  -DPROJ_SAX2  -DPROJ_DOM -DPROJ_DEPRECATED_DOM -DPROJ_VALIDATORS -DXML_USE_NATIVE_TRANSCODER -DXML_USE_INMEM_MESSAGELOADER -DXML_USE_PTHREADS -DXML_USE_NETACCESSOR_SOCKET  
CFLAGS =   -w -O -DNDEBUG -DPROJ_XMLPARSER  -DPROJ_XMLUTIL  -DPROJ_PARSERS  -DPROJ_SAX4C  -DPROJ_SAX2  -DPROJ_DOM -DPROJ_DEPRECATED_DOM -DPROJ_VALIDATORS -DXML_USE_NATIVE_TRANSCODER -DXML_USE_INMEM_MESSAGELOADER -DXML_USE_PTHREADS -DXML_USE_NETACCESSOR_SOCKET   
PREFIX = /usr/local
PREFIX_INCLUDE = ${PREFIX}/include/xercesc
LDFLAGS =    
LIBS =   -lpthread 
OSVER = 
USELIBWWW = 
MESSAGELOADER = INMEM
TRANSCODER = NATIVE
THREADS = pthread

MODULE = util

include ../../../Makefile.incl

SUBMODULE = Platforms/BeOS
CXXFLAGS += -D_GNU_SOURCE -D__USE_GNU
CPP_PUBHEADERS = BeOSDefs.hpp
CPP_OBJECTS = BeOSPlatformUtils.$(TO)

include ../../Makefile.util.submodule
