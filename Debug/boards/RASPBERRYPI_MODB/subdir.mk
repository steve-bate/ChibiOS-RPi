################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../boards/RASPBERRYPI_MODB/board.c 

OBJS += \
./boards/RASPBERRYPI_MODB/board.o 

C_DEPS += \
./boards/RASPBERRYPI_MODB/board.d 


# Each subdirectory must supply rules for building sources it contributes
boards/RASPBERRYPI_MODB/%.o: ../boards/RASPBERRYPI_MODB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


