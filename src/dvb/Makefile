CC=gcc
CPLUS=g++

SRC=test.cpp
HED=
OBJ=test.o

CFLG = -L../../lib -L/usr/lib64

CFLAGS += \

CLIB = \
-lQtCore \
-lQtXml \
-lx86-contentMgr \
-lx86-mxflib \
-lx86-mxfEssenceReader \
-lxerces-c \
-lx86-dcpXmlCore \
-lx86-GeneralXMLLib \
-lx86-dcpXmlWrap \
-lx86-dcpParser \
-lx86-programQuery \
-lx86-programManager \
-lx86-programImport \
-lx86-programInterface \
-lx86-netComm \
-lx86-dvbOperater \
-lx86-Config \
-lx86-iniFile \
-lx86-Log \
-ldvbv5 \
-lssl \
-lcrypto \
-lboost_filesystem \
-lboost_system \
-lboost_thread \
-lboost_signals \
-lx86-activeThreadManager

BIND=/usr/local/bin/
INCLUDE=-I../s2/linux/include -I../../include

TARGET=test

all: $(TARGET)
	$(MAKE) --no-print-directory post-build 
	
$(TARGET): $(OBJ)
	$(CPLUS) $(INCLUDE) $(CFLG) $(OBJ) -o $(TARGET) $(CLIB) 

$(OBJ): $(HED)

install: all
	cp $(TARGET) $(BIND)

uninstall:
	rm $(BIND)$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) *~
	
post-build:
	cp $(TARGET) ../../bin/linux/release
	cp $(TARGET) /usr/CineCast/bin/cinecastd
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
