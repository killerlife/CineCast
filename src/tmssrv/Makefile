CC=gcc
CPLUS=g++

SRC=tmsd.cpp
HED=
OBJ=tmsd.o

CFLG = -L../../lib -L/usr/lib64

CLIB = \
-lQtCore \
-lQtXml \
-lx86-tmsSrv \
-lx86-Log \
-lx86-contentMgr \
-lx86-netComm \
-lx86-dvbOperater \
-lx86-Config \
-lx86-iniFile \
-lssl \
-lcrypto \
-lboost_filesystem \
-lboost_system \
-lboost_thread \
-lboost_signals \
-lx86-activeThreadManager

BIND=/usr/local/bin/
INCLUDE=-I../s2/linux/include -I../../include -I../../include/brunt

TARGET=tmsd

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
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
