################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AssistHash.cpp \
../AssistReel.cpp \
../AssitComposition.cpp \
../DcpParse.cpp \
../assetMap.cpp 

OBJS += \
./AssistHash.o \
./AssistReel.o \
./AssitComposition.o \
./DcpParse.o \
./assetMap.o 

CPP_DEPS += \
./AssistHash.d \
./AssistReel.d \
./AssitComposition.d \
./DcpParse.d \
./assetMap.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I../../../../../boost/include -I../../../../../xml/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


