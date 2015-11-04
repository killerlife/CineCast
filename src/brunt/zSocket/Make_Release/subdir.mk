################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../auxiliar.c \
../io.c \
../timeout.c \
../usocket.c \
../wsocket.c \
../zSocket.cpp

OBJS += \
./auxiliar.o \
./io.o \
./timeout.o \
./usocket.o \
./wsocket.o \
./zSocket.o

CPP_DEPS += \
./auxiliar.d \
./io.d \
./timeout.d \
./usocket.d \
./wsocket.d \
./zSocket.d

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -I../../../../include -I../../../../public/boost/include -I../../../../include/mxf -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -I../../../../include -I../../../../public/boost/include -I../../../../include/mxf -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

