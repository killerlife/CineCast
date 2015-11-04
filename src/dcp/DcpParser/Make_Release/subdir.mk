################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AssistHash.cpp \
../AssistReel.cpp \
../AssistComposition.cpp \
../DcpParser.cpp \
../assetMap.cpp 

OBJS += \
./AssistHash.o \
./AssistReel.o \
./AssistComposition.o \
./DcpParser.o \
./assetMap.o 

CPP_DEPS += \
./AssistHash.d \
./AssistReel.d \
./AssistComposition.d \
./DcpParser.d \
./assetMap.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../include/dcp/DcpParser -I../../../../include/xml -I../../../../include -I../../../../include/mxf -I../../../../public/boost/include  -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


