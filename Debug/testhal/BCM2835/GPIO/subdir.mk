################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../testhal/BCM2835/GPIO/main.c 

OBJS += \
./testhal/BCM2835/GPIO/main.o 

C_DEPS += \
./testhal/BCM2835/GPIO/main.d 


# Each subdirectory must supply rules for building sources it contributes
testhal/BCM2835/GPIO/%.o: ../testhal/BCM2835/GPIO/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


