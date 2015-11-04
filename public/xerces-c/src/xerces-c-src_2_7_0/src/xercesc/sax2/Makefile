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
#
# $Id: Makefile.in 176366 2005-04-05 09:21:57Z gareth $
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

include ../Makefile.incl

MODULE = sax2

SAX2_CPP_PUBHEADERS = \
	Attributes.hpp \
	ContentHandler.hpp \
	DefaultHandler.hpp \
	LexicalHandler.hpp \
	DeclHandler.hpp \
	SAX2XMLReader.hpp \
	SAX2XMLFilter.hpp \
	XMLReaderFactory.hpp

SAX2_CPP_PRIVHEADERS =

SAX2_C_FILES =

SAX2_CPP_OBJECTS = \
	sax2Dummy.$(TO)

all::	includes $(SAX2_CPP_OBJECTS)

includes::	pubheaders $(SAX2_C_FILES)

pubheaders::
	$Qmkdir -p $(XML_INC_DIR)/$(MODULE)
	@echo "  (CP)  $(XML_INC_DIR)/$(MODULE)"
	$(CP) $(SAX2_CPP_PUBHEADERS) $(SAX2_C_FILES) $(XML_INC_DIR)/$(MODULE)

# this may generate unnecessary dependencies, but it makes life easier
depend:: includes
	@echo "  (DEP)"
	$Q$(MAKE_DEPEND) $(XML_INCL) *.cpp > $(DEPFILE)

clean::
	@echo "Making clean in $(MODULE) ..."
	$(RM2) $(addprefix $(XML_OBJ_DIR)/,$(SAX2_CPP_OBJECTS))

distclean::	clean
	$(RM) Makefile $(DEPFILE)
	@echo "Removing all $(MODULE) header files ..."
	$(RM2) $(addprefix $(XML_INC_DIR)/$(MODULE)/,$(SAX2_CPP_PUBHEADERS))

install::
	-mkdir -p $(DESTDIR)$(PREFIX_INCLUDE)/$(MODULE)
	$(CP) $(SAX2_CPP_PUBHEADERS) $(SAX2_C_FILES) $(DESTDIR)$(PREFIX_INCLUDE)/$(MODULE)
