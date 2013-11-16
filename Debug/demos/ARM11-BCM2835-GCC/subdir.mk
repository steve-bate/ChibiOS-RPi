################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demos/ARM11-BCM2835-GCC/main.c 

OBJS += \
./demos/ARM11-BCM2835-GCC/main.o 

C_DEPS += \
./demos/ARM11-BCM2835-GCC/main.d 


# Each subdirectory must supply rules for building sources it contributes
demos/ARM11-BCM2835-GCC/%.o: ../demos/ARM11-BCM2835-GCC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


