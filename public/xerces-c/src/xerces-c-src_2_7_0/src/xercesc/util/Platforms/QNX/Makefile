# Generated automatically from Makefile.in by configure.
#
# Copyright 2003,2004 The Apache Software Foundation.
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
# Revision 1.3  2004/09/08 13:56:42  peiyongz
# Apache License Version 2.0
#
# Revision 1.2  2004/07/09 16:24:50  peiyongz
# Build on HP-Itanium, aCC A.05.52
#
# Revision 1.1  2003/03/13 22:11:16  tng
# [Bug 17858] Support for QNX/Neutrino.  Patch from Chris McKillop.
#
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

SUBMODULE = Platforms/QNX
CPP_PUBHEADERS = QNXDefs.hpp
CPP_OBJECTS = QNXPlatformUtils.$(TO)

include ../../Makefile.util.submodule
