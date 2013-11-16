################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/hal/templates/meta/driver.c \
../os/hal/templates/meta/driver_lld.c 

OBJS += \
./os/hal/templates/meta/driver.o \
./os/hal/templates/meta/driver_lld.o 

C_DEPS += \
./os/hal/templates/meta/driver.d \
./os/hal/templates/meta/driver_lld.d 


# Each subdirectory must supply rules for building sources it contributes
os/hal/templates/meta/%.o: ../os/hal/templates/meta/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


