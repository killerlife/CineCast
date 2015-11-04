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
all: libmips-boost-threadd.so

# Tool invocations
libmips-boost-threadd.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	/home/baiqian/myproject/newV8/sdk/8634/2.8/smp86xx_toolchain_2.8.0.1/build_mipsel_nofpu/staging_dir/bin/mipsel-linux-g++ -shared -o"libmips-boost-threadd.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(CC_DEPS)$(C_DEPS)$(CPP_DEPS)$(LIBRARIES)$(CXX_DEPS)$(C_UPPER_DEPS) libmips-boost-threadd.so
	-@echo ' '

post-build:
	-cp libmips-boost-threadd.so ../../../lib & cp makefile ../Make_Debug & cp *.mk ../Make_Debug
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
