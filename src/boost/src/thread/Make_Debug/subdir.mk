################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../barrier.cpp \
../condition.cpp \
../exceptions.cpp \
../mutex.cpp \
../once.cpp \
../recursive_mutex.cpp \
../thread.cpp \
../tss_dll.cpp \
../tss_hooks.cpp \
../tss_null.cpp \
../tss_pe.cpp \
../xtime.cpp 

OBJS += \
./barrier.o \
./condition.o \
./exceptions.o \
./mutex.o \
./once.o \
./recursive_mutex.o \
./thread.o \
./tss_dll.o \
./tss_hooks.o \
./tss_null.o \
./tss_pe.o \
./xtime.o 

CPP_DEPS += \
./barrier.d \
./condition.d \
./exceptions.d \
./mutex.d \
./once.d \
./recursive_mutex.d \
./thread.d \
./tss_dll.d \
./tss_hooks.d \
./tss_null.d \
./tss_pe.d \
./xtime.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/home/baiqian/myproject/newV8/sdk/8634/2.8/smp86xx_toolchain_2.8.0.1/build_mipsel_nofpu/staging_dir/bin/mipsel-linux-g++ -I../../../../boost/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


