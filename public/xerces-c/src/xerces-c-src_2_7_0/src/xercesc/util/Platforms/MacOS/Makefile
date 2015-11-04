# Generated automatically from Makefile.in by configure.
#
# Copyright 1999-2000,2004 The Apache Software Foundation.
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
# $Log
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
SUBMODULE = Platforms/MacOS
CXXFLAGS += -D_GNU_SOURCE -D__USE_GNU

include ../../../Makefile.incl

CPP_PUBHEADERS = \
	MacOSDefs.hpp \
	MacOSPlatformUtils.hpp \
	MacAbstractFile.hpp \
	MacCarbonFile.hpp \
	MacPosixFile.hpp

CPP_OBJECTS = \
	MacOSPlatformUtils.$(TO) \
	MacCarbonFile.$(TO) \
	MacPosixFile.$(TO)

include ../../Makefile.util.submodule
