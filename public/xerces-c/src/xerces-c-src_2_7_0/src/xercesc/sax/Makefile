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

MODULE = sax

SAX_CPP_PUBHEADERS = \
	AttributeList.hpp \
	DTDHandler.hpp \
	DocumentHandler.hpp \
	EntityResolver.hpp \
	ErrorHandler.hpp \
	HandlerBase.hpp \
	InputSource.hpp \
	Locator.hpp \
	Parser.hpp \
	SAXException.hpp \
	SAXParseException.hpp

SAX_CPP_PRIVHEADERS =

SAX_C_FILES =

SAX_CPP_OBJECTS = \
	Dummy.$(TO) \
	InputSource.$(TO) \
	SAXException.$(TO) \
	SAXParseException.$(TO)

all::	includes $(SAX_CPP_OBJECTS)

includes::	pubheaders $(SAX_C_FILES)

pubheaders::
	$Qmkdir -p $(XML_INC_DIR)/$(MODULE)
	@echo "  (CP)  $(XML_INC_DIR)/$(MODULE)"
	$Q$(CP) $(SAX_CPP_PUBHEADERS) $(SAX_C_FILES) $(XML_INC_DIR)/$(MODULE)

# this may generate unnecessary dependencies, but it makes life easier
depend:: includes
	@echo "  (DEP)"
	$Q$(MAKE_DEPEND) $(XML_INCL) *.cpp > $(DEPFILE)

clean::
	@echo "Making clean in $(MODULE) ..."
	$(RM2) $(addprefix $(XML_OBJ_DIR)/,$(SAX_CPP_OBJECTS))

distclean::	clean
	$(RM) Makefile $(DEPFILE)
	@echo "Removing all $(MODULE) header files ..."
	$(RM2) $(addprefix $(XML_INC_DIR)/$(MODULE)/,$(SAX_CPP_PUBHEADERS))

install::
	-mkdir -p $(DESTDIR)$(PREFIX_INCLUDE)/$(MODULE)
	$(CP) $(SAX_CPP_PUBHEADERS) $(SAX_C_FILES) $(DESTDIR)$(PREFIX_INCLUDE)/$(MODULE)
