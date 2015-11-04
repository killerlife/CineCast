################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AS_DCP.cpp \
../AS_DCP_AES.cpp \
../AS_DCP_JP2K.cpp \
../AS_DCP_MPEG2.cpp \
../AS_DCP_MXF.cpp \
../AS_DCP_PCM.cpp \
../AS_DCP_TimedText.cpp \
../Dict.cpp \
../h__Reader.cpp \
../h__Writer.cpp \
../Index.cpp \
../JP2K.cpp \
../JP2K_Codestream_Parser.cpp \
../JP2K_Sequence_Parser.cpp \
../KLV.cpp \
../KM_fileio.cpp \
../KM_log.cpp \
../KM_prng.cpp \
../KM_util.cpp \
../KM_xml.cpp \
../MDD.cpp \
../Metadata.cpp \
../MPEG.cpp \
../MPEG2_Parser.cpp \
../MXF.cpp \
../MXFTypes.cpp \
../PCMParserList.cpp \
../PCM_Parser.cpp \
../TimedText_Parser.cpp \
../Wav.cpp \

OBJS += \
../AS_DCP.o \
../AS_DCP_AES.o \
../AS_DCP_JP2K.o \
../AS_DCP_MPEG2.o \
../AS_DCP_MXF.o \
../AS_DCP_PCM.o \
../AS_DCP_TimedText.o \
../Dict.o \
../h__Reader.o \
../h__Writer.o \
../Index.o \
../JP2K.o \
../JP2K_Codestream_Parser.o \
../JP2K_Sequence_Parser.o \
../KLV.o \
../KM_fileio.o \
../KM_log.o \
../KM_prng.o \
../KM_util.o \
../KM_xml.o \
../MDD.o \
../Metadata.o \
../MPEG.o \
../MPEG2_Parser.o \
../MXF.o \
../MXFTypes.o \
../PCMParserList.o \
../PCM_Parser.o \
../TimedText_Parser.o \
../Wav.o \

CPP_DEPS += \
../AS_DCP.d \
../AS_DCP_AES.d \
../AS_DCP_JP2K.d \
../AS_DCP_MPEG2.d \
../AS_DCP_MXF.d \
../AS_DCP_PCM.d \
../AS_DCP_TimedText.d \
../Dict.d \
../h__Reader.d \
../h__Writer.d \
../Index.d \
../JP2K.d \
../JP2K_Codestream_Parser.d \
../JP2K_Sequence_Parser.d \
../KLV.d \
../KM_fileio.d \
../KM_log.d \
../KM_prng.d \
../KM_util.d \
../KM_xml.d \
../MDD.d \
../Metadata.d \
../MPEG.d \
../MPEG2_Parser.d \
../MXF.d \
../MXFTypes.d \
../PCMParserList.d \
../PCM_Parser.d \
../TimedText_Parser.d \
../Wav.d \


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -I../ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


