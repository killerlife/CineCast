CC=gcc
CPLUS=g++

SRC=test.cpp
HED=
OBJ=test.o

CFLG = -L../../lib -L/usr/lib64

CFLAGS += \

CLIB = \
-lx86-externCall \
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
	cp $(TARGET) /usr/CineCast/bin/test
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
