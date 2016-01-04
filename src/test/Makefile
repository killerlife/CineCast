CC=gcc
CPLUS=g++

SRC=test3.cpp
HED=
OBJ=test3.o

CFLG = -L../../lib 

CLIB = -lx86-iniFile -lx86-Log -lx86-mxflib -lx86-mxfEssenceReader -lxerces-c -lx86-dcpXmlCore -lx86-GeneralXMLLib -lx86-dcpXmlWrap -lx86-dcpParser -lx86-programQuery -lx86-programManager -lx86-programImport -lx86-Config -lx86-iniFile -lx86-programInterface -lssl -lcrypto -lboost_filesystem -lboost_system -lboost_thread -lboost_signals -lx86-activeThreadManager

BIND=/usr/local/bin/
INCLUDE=-I../s2/linux/include -I../../include

TARGET=log

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
