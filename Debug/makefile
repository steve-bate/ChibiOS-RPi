################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include testhal/BCM2835/SPI/subdir.mk
-include testhal/BCM2835/RTC/subdir.mk
-include testhal/BCM2835/PWM/subdir.mk
-include testhal/BCM2835/I2C/subdir.mk
-include testhal/BCM2835/GPT/subdir.mk
-include testhal/BCM2835/GPIO/subdir.mk
-include os/various/lwip_bindings/arch/subdir.mk
-include os/various/lwip_bindings/subdir.mk
-include os/various/fatfs_bindings/subdir.mk
-include os/various/devices_lib/accel/subdir.mk
-include os/various/subdir.mk
-include os/ports/common/ARMCMx/subdir.mk
-include os/ports/GCC/ARM/subdir.mk
-include os/kernel/templates/subdir.mk
-include os/kernel/src/subdir.mk
-include os/hal/templates/meta/subdir.mk
-include os/hal/templates/subdir.mk
-include os/hal/src/subdir.mk
-include os/hal/platforms/BCM2835/subdir.mk
-include os/contrib/subdir.mk
-include demos/ARM11-BCM2835-GCC/subdir.mk
-include boards/RASPBERRYPI_MODB/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ChibiOS-RPi

# Tool invocations
ChibiOS-RPi: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -L"/home/run/workspace/ChibiOS-RPi/os/hal/include" -o "ChibiOS-RPi" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ChibiOS-RPi
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
