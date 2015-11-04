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
	-$(CC) -I../../../../../public/boost/include -fPIC -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


