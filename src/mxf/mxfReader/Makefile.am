EXTRA_DIST = $(rsrc_DATA)

rsrcdir = $(datadir)/mxfReader
pkgincludedir = $(includedir)/mxfReader

lib_LIBRARIES = libmxfReader.a
libmxfReader_a_SOURCES = AESCrypto.cpp Rijndael.cpp mxfReader.cpp

INCLUDES = -I$(top_builddir)
pkginclude_HEADERS = \
			AESCrypto.h \
			BufferPool.h \
			IMxfReader.h \
			Rijndael.h \
			mxfReader.h 

rsrc_DATA = \
			$(top_builddir)/types.xml \
			$(top_builddir)/xmldict.xml \
			$(top_builddir)/dict.xml \
			$(top_builddir)/DMS_Crypto.xml
