################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../zSocket.cpp \
../NetCommThread.cpp \
../GuiServer.cpp \
../SimulatorServer.cpp \
../cfctms.cpp \
../aureolam.cpp \
../BaseOperation.cpp 
#../Log.cpp \
#../LogFileOperation.cpp

C_SRCS += \
../timeout.c \
../usocket.c 

OBJS += \
./timeout.o \
./usocket.o \
./zSocket.o \
./NetCommThread.o \
./GuiServer.o \
./SimulatorServer.o \
./cfctms.o \
./aureolam.o \
./BaseOperation.o 
#./Log.o \
#./LogFileOperation.o

CPP_DEPS += \
./zSocket.d \
./NetCommThread.d \
./GuiServer.d \
./SimulatorServer.d \
./cfctms.d \
./aureolam.d \
./BaseOperation.d 
#./Log.d \
#./LogFileOperation.d


C_DEPS += \
./timeout.d \
./usocket.d 

INC += \
-I../../../public/boost/include \
-I../../../include \
-I../../../include/log \
-I../../../include/config \
-I../../../../include/dcp \
-I../../../../include/brunt \
-I../../../include/xml \
-I../../../../public/boost_1_33_1 

CFLAGS += \
-fPIC  \
-DDEBUG \
-DSIMULATOR \
-Wall \
-DENABLE_RAID

ifeq ($(NOC_WANDA), TRUE)
CFLAGS += -DWANDA
endif

ifeq ($(NOC_AUREOLAM), TRUE)
CFLAGS += -DAUREOLAM
endif

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC)  $(CFLAGS) $(INC) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC)  $(CFLAGS) $(INC) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

