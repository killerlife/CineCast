################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DcpProgramParse.cpp \
../IniProgramParse.cpp \
../programParse.cpp 

OBJS += \
./DcpProgramParse.o \
./IniProgramParse.o \
./programParse.o 

CPP_DEPS += \
./DcpProgramParse.d \
./IniProgramParse.d \
./programParse.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I../../../../../ini/include -I../../../../../dcp/include -I../../../../include -I../../../../../xml/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


