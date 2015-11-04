################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Element.cpp \
../GeneralXMLLib.cpp \
../UUIDElement.cpp 

OBJS += \
./Element.o \
./GeneralXMLLib.o \
./UUIDElement.o 

CPP_DEPS += \
./Element.d \
./GeneralXMLLib.d \
./UUIDElement.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I../../../ -I../../../include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


