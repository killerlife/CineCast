################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../crypto.cpp \
../cryptoex.cpp \
../datachunk.cpp \
../debug.cpp \
../deftypes.cpp \
../esp_dvdif.cpp \
../esp_jp2k.cpp \
../esp_mpeg2ts.cpp \
../esp_mpeg2ves.cpp \
../esp_wavepcm.cpp \
../essence.cpp \
../helper.cpp \
../index.cpp \
../klvobject.cpp \
../mdobject.cpp \
../mdtraits.cpp \
../mdtype.cpp \
../metadata.cpp \
../mxffile.cpp \
../partition.cpp \
../primer.cpp \
../rip.cpp \
../sopsax.cpp \
../xmlparser.cpp 

OBJS += \
./crypto.o \
./cryptoex.o \
./datachunk.o \
./debug.o \
./deftypes.o \
./esp_dvdif.o \
./esp_jp2k.o \
./esp_mpeg2ts.o \
./esp_mpeg2ves.o \
./esp_wavepcm.o \
./essence.o \
./helper.o \
./index.o \
./klvobject.o \
./mdobject.o \
./mdtraits.o \
./mdtype.o \
./metadata.o \
./mxffile.o \
./partition.o \
./primer.o \
./rip.o \
./sopsax.o \
./xmlparser.o 

CPP_DEPS += \
./crypto.d \
./cryptoex.d \
./datachunk.d \
./debug.d \
./deftypes.d \
./esp_dvdif.d \
./esp_jp2k.d \
./esp_mpeg2ts.d \
./esp_mpeg2ves.d \
./esp_wavepcm.d \
./essence.d \
./helper.d \
./index.d \
./klvobject.d \
./mdobject.d \
./mdtraits.d \
./mdtype.d \
./metadata.d \
./mxffile.d \
./partition.d \
./primer.d \
./rip.d \
./sopsax.d \
./xmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -I../../../../include/mxf -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


