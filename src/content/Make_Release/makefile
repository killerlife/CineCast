################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: libx86-contentMgr.so

# Tool invocations
libx86-contentMgr.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -L../../../lib/linux/release $(LIBS) -shared -o"libx86-contentMgr.so" $(OBJS) $(USER_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(CC_DEPS)$(C_DEPS)$(CPP_DEPS)$(LIBRARIES)$(CXX_DEPS)$(C_UPPER_DEPS) libx86-contentMgr.so
	-@echo ' '

pre-build:
#	-cp ../*.h ../../../../include
	-@echo ' '

post-build:
	-cp libx86-contentMgr.so ../../../lib
	-cp libx86-contentMgr.so ../../../bin/linux/release
	-cp libx86-contentMgr.so /usr/CineCast/lib
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
