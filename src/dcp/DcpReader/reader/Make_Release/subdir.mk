################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../assetMap.cpp \
../DcpReader.cpp \
../TaCompositionReader.cpp \
../TaTrackFileReader.cpp

OBJS += \
./assetMap.o \
./DcpReader.o \
./TaCompositionReader.o \
./TaTrackFileReader.o

CPP_DEPS += \
./assetMap.d \
./DcpReader.d \
./TaCompositionReader.d \
./TaTrackFileReader.d


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../../public/boost/include -I../../../../../include/dcp/DcpReader -I../../../../../include/dcp -I../../../../../include/mxf -I../../../../../include/xml  -I../../../../../public/boost_1_33_1  -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


