################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AESCrypto.cpp \
../Rijndael.cpp \
../mxfReader.cpp 

OBJS += \
./AESCrypto.o \
./Rijndael.o \
./mxfReader.o 

CPP_DEPS += \
./AESCrypto.d \
./Rijndael.d \
./mxfReader.d 

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -I../../../../include -I../../../../public/boost/include -I../../../../include/mxf -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


