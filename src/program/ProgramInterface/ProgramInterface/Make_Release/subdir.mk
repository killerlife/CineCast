################################################################################
# Automatically-generated file. Do not edit!
################################################################################
# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../v15ProgramInterface.cpp\
../v15ProgramManager.cpp \
../v15ImportInterface.cpp


OBJS += \
./v15ProgramInterface.o\
./v15ProgramManager.o \
./v15ImportInterface.o


CPP_DEPS += \
./v15ProgramInterface.d\
./v15ProgramManager.d \
./v15ImportInterface.d


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../../include/program -I../../../../../include -I../../log/src/logCtrl2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


