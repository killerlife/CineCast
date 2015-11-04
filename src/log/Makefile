CC=gcc

SRC=Log.cpp LogFileOperation.cpp
OBJ=Log.o LogFileOperation.o

BIND=/usr/local/bin/
INC=-I../../include/config -I../../include -I../../include/log
CFLG=-shared -fPIC

TARGET=libx86-Log.so.1.0

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLG) $(OBJ) -o $(TARGET) $(CLIB) 

$(OBJ): $(HED)

install: all
	-cp $(TARGET) ../../lib
	-cp $(TARGET) ../../bin/linux/release

uninstall:
	rm ../../lib/$(TARGET)
	rm ../../bin/linux/release/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) *~

%.o: %.cpp
	$(CC) $(INC) -shared -fPIC -c $< -o $@