################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CompositionPlayList.cpp \
../Composition_AssetList.cpp \
../Composition_AuxSoundTrackFileAsset.cpp \
../Composition_ContentVersion.cpp \
../Composition_GenericAsset.cpp \
../Composition_Marker.cpp \
../Composition_MarkerAsset.cpp \
../Composition_MarkerList.cpp \
../Composition_PictureTrackFileAsset.cpp \
../Composition_Rating.cpp \
../Composition_RatingList.cpp \
../Composition_Reel.cpp \
../Composition_ReelList.cpp \
../Composition_SoundTrackFileAsset.cpp \
../Composition_TrackFileAsset.cpp \
../DCIProjectElement.cpp \
../DCPMaker.cpp \
../DCPWrapLib.cpp \
../IssuerElement.cpp \
../MakeComposition.cpp \
../Make_Asset.cpp \
../Make_Reel.cpp \
../Make_Track.cpp \
../MyUUIDElement.cpp \
../Pack_Asset.cpp \
../Pack_AssetList.cpp \
../PackingList.cpp 

OBJS += \
./CompositionPlayList.o \
./Composition_AssetList.o \
./Composition_AuxSoundTrackFileAsset.o \
./Composition_ContentVersion.o \
./Composition_GenericAsset.o \
./Composition_Marker.o \
./Composition_MarkerAsset.o \
./Composition_MarkerList.o \
./Composition_PictureTrackFileAsset.o \
./Composition_Rating.o \
./Composition_RatingList.o \
./Composition_Reel.o \
./Composition_ReelList.o \
./Composition_SoundTrackFileAsset.o \
./Composition_TrackFileAsset.o \
./DCIProjectElement.o \
./DCPMaker.o \
./DCPWrapLib.o \
./IssuerElement.o \
./MakeComposition.o \
./Make_Asset.o \
./Make_Reel.o \
./Make_Track.o \
./MyUUIDElement.o \
./Pack_Asset.o \
./Pack_AssetList.o \
./PackingList.o 

CPP_DEPS += \
./CompositionPlayList.d \
./Composition_AssetList.d \
./Composition_AuxSoundTrackFileAsset.d \
./Composition_ContentVersion.d \
./Composition_GenericAsset.d \
./Composition_Marker.d \
./Composition_MarkerAsset.d \
./Composition_MarkerList.d \
./Composition_PictureTrackFileAsset.d \
./Composition_Rating.d \
./Composition_RatingList.d \
./Composition_Reel.d \
./Composition_ReelList.d \
./Composition_SoundTrackFileAsset.d \
./Composition_TrackFileAsset.d \
./DCIProjectElement.d \
./DCPMaker.d \
./DCPWrapLib.d \
./IssuerElement.d \
./MakeComposition.d \
./Make_Asset.d \
./Make_Reel.d \
./Make_Track.d \
./MyUUIDElement.d \
./Pack_Asset.d \
./Pack_AssetList.d \
./PackingList.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -fPIC -I../../../../include/xml -I../../../../include/xml/DCPWrapLib -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


