################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SatelliteRecv.cpp \
../demux.cpp \
../FilmDataProcess.cpp \
../PMTDataProcess.cpp \
../PATDataProcess.cpp \
../NotifyDataProcess.cpp \
../StartDataProcess.cpp \
../FinishDataProcess.cpp \
../CancelDataProcess.cpp \
../DataProcessThread.cpp

C_SRCS += \
../do_tuner.c

OBJS += \
./SatelliteRecv.o \
./demux.o \
./do_tuner.o \
./FilmDataProcess.o \
./PMTDataProcess.o \
./PATDataProcess.o \
./NotifyDataProcess.o \
./StartDataProcess.o \
./FinishDataProcess.o \
./CancelDataProcess.o \
./DataProcessThread.o

CPP_DEPS += \
./SatelliteRecv.d \
./demux.d \
./FilmDataProcess.d \
./PMTDataProcess.d \
./PATDataProcess.d \
./NotifyDataProcess.d \
./StartDataProcess.d \
./FinishDataProcess.d \
./CancelDataProcess.d \
./DataProcessThread.d

C_DEPS += \
./do_tuner.d


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(CFLAGS) -DUSE_POLL -DDEBUG -D__USE_FILE_OFFSET64 -D_FILE_OFFSET_BITS=64  -fPIC -I../../../public/boost/include -I../../../include -I../../../../include/dcp -I../../../../include/brunt -I../../../../include/xml -I../../../../public/boost_1_33_1 -O3  -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	gcc $(CFLAGS) -DUSE_POLL -DDEBUG -fPIC -I../../../public/boost/include -I../../../include -I../../../../include/dcp -I../../../../include/brunt -I../../../../include/xml -I../../../../public/boost_1_33_1 -O3  -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
