################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../convenience.cpp \
../exception.cpp \
../operations_posix_windows.cpp \
../path_posix_windows.cpp 

OBJS += \
./convenience.o \
./exception.o \
./operations_posix_windows.o \
./path_posix_windows.o 

CPP_DEPS += \
./convenience.d \
./exception.d \
./operations_posix_windows.d \
./path_posix_windows.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	-$(CC) -I../../../../../public/boost/include -fPIC -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


