################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/various/devices_lib/accel/lis302dl.c 

OBJS += \
./os/various/devices_lib/accel/lis302dl.o 

C_DEPS += \
./os/various/devices_lib/accel/lis302dl.d 


# Each subdirectory must supply rules for building sources it contributes
os/various/devices_lib/accel/%.o: ../os/various/devices_lib/accel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


