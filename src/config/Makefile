CC=gcc

SRC=config.cpp
OBJ=config.o

BIND=/usr/local/bin/
INC=-I../../include/config -I../../include
CFLG=-shared -fPIC

TARGET=libx86-Config.so

all: $(TARGET) install

$(TARGET): $(OBJ)
	$(CC) $(CFLG) $(OBJ) -o $(TARGET) $(CLIB) 

$(OBJ): $(HED)

install:
	-cp $(TARGET) ../../lib
	-cp $(TARGET) ../../bin/linux/release
	-cp $(TARGET) /usr/CineCast/lib

uninstall:
	rm ../../lib/$(TARGET)
	rm ../../bin/linux/release/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) *~

%.o: %.cpp
	$(CC) $(INC) -shared -fPIC -c $< -o $@