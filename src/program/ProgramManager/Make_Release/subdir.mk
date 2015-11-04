################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ProgramManager.cpp 

OBJS += \
./ProgramManager.o 

CPP_DEPS += \
./ProgramManager.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../include/program -I../../../../include/thread/activeThread -I../../../../include/dcp -I../../../../include/xml -I../../../../public/boost/include -I../../../../include -I../../../../include/boost -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


