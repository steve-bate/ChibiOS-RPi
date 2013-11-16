################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../testhal/BCM2835/GPT/main.c \
../testhal/BCM2835/GPT/rtc_lld.c 

OBJS += \
./testhal/BCM2835/GPT/main.o \
./testhal/BCM2835/GPT/rtc_lld.o 

C_DEPS += \
./testhal/BCM2835/GPT/main.d \
./testhal/BCM2835/GPT/rtc_lld.d 


# Each subdirectory must supply rules for building sources it contributes
testhal/BCM2835/GPT/%.o: ../testhal/BCM2835/GPT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


