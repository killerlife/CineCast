################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DXMLCore.cpp \
../XMLManage.cpp \
../XMLNode.cpp 

OBJS += \
./DXMLCore.o \
./XMLManage.o \
./XMLNode.o 

CPP_DEPS += \
./DXMLCore.d \
./XMLManage.d \
./XMLNode.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I../../../../../sdk/xerces-c/include -I../../../include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


