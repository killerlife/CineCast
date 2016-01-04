################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../zSocket.cpp \
../NetCommThread.cpp \
../GuiServer.cpp \
../BaseOperation.cpp 

C_SRCS += \
../timeout.c \
../usocket.c 


OBJS += \
./timeout.o \
./usocket.o \
./zSocket.o \
./NetCommThread.o \
./GuiServer.o \
./BaseOperation.o 

CPP_DEPS += \
./zSocket.d \
./NetCommThread.d \
./GuiServer.d \
./BaseOperation.d 

C_DEPS += \
./timeout.d \
./usocket.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -DDEBUG -fPIC -I../../../public/boost/include -I../../../include -I../../../../include/dcp -I../../../../include/brunt -I../../../../include/xml -I../../../../public/boost_1_33_1 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC)  -DDEBUG -fPIC -I../../../public/boost/include -I../../../include -I../../../../include/dcp -I../../../../include/brunt -I../../../../include/xml -I../../../../public/boost_1_33_1 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

