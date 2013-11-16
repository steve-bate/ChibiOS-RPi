################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/ports/common/ARMCMx/nvic.c 

OBJS += \
./os/ports/common/ARMCMx/nvic.o 

C_DEPS += \
./os/ports/common/ARMCMx/nvic.d 


# Each subdirectory must supply rules for building sources it contributes
os/ports/common/ARMCMx/%.o: ../os/ports/common/ARMCMx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


