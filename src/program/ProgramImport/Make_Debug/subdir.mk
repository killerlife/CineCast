################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GetSpace.cpp \
../NewV8ProgramImport.cpp \
../importThread.cpp 

OBJS += \
./GetSpace.o \
./NewV8ProgramImport.o \
./importThread.o 

CPP_DEPS += \
./GetSpace.d \
./NewV8ProgramImport.d \
./importThread.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -I../../../../include -I../../../../../thread/include -I../../../../../boost/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


