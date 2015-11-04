################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../activeThread.cpp \
../activeThreadManager.cpp \
../asyncObjectThread.cpp \
../main.cpp \
../sleep.cpp 

OBJS += \
./activeThread.o \
./activeThreadManager.o \
./asyncObjectThread.o \
./main.o \
./sleep.o 

CPP_DEPS += \
./activeThread.d \
./activeThreadManager.d \
./asyncObjectThread.d \
./main.d \
./sleep.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../public/boost/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


