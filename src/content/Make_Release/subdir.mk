﻿################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ContentManager.cpp \
../ContentParser.cpp \
../ContentQuery.cpp 

C_SRCS += 

OBJS += \
./ContentManager.o \
./ContentParser.o \
./ContentQuery.o 

CPP_DEPS += \
./ContentManager.d \
./ContentParser.d \
./ContentQuery.d  

C_DEPS += 

INCLUDE += \
 -I../../../public/boost/include \
 -I../../../include \
 -I../../../../include/dcp \
 -I../../../../include/brunt \
 -I../../../../include/xml \
 -I../../../../public/boost_1_33_1 \
 -I/usr/include/QtCore \
 -I/usr/include/QtXml
 
CFLAGS += \
-fPIC \
-O3 \
-Wall
#-Wall \
#-DENABLE_RAID
#-DDEBUG \

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDE) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(CFLAGS) $(INCLUDE) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

