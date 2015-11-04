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
	/home/baiqian/myproject/newV8/sdk/8634/2.8/smp86xx_toolchain_2.8.0.1/build_mipsel_nofpu/staging_dir/bin/mipsel-linux-g++ -I../../../boost/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


