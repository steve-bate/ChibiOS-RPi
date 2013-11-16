################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/hal/src/adc.c \
../os/hal/src/can.c \
../os/hal/src/ext.c \
../os/hal/src/gpt.c \
../os/hal/src/hal.c \
../os/hal/src/i2c.c \
../os/hal/src/i2s.c \
../os/hal/src/icu.c \
../os/hal/src/mac.c \
../os/hal/src/mmc_spi.c \
../os/hal/src/mmcsd.c \
../os/hal/src/pal.c \
../os/hal/src/pwm.c \
../os/hal/src/rtc.c \
../os/hal/src/sdc.c \
../os/hal/src/serial.c \
../os/hal/src/serial_usb.c \
../os/hal/src/spi.c \
../os/hal/src/tm.c \
../os/hal/src/uart.c \
../os/hal/src/usb.c 

OBJS += \
./os/hal/src/adc.o \
./os/hal/src/can.o \
./os/hal/src/ext.o \
./os/hal/src/gpt.o \
./os/hal/src/hal.o \
./os/hal/src/i2c.o \
./os/hal/src/i2s.o \
./os/hal/src/icu.o \
./os/hal/src/mac.o \
./os/hal/src/mmc_spi.o \
./os/hal/src/mmcsd.o \
./os/hal/src/pal.o \
./os/hal/src/pwm.o \
./os/hal/src/rtc.o \
./os/hal/src/sdc.o \
./os/hal/src/serial.o \
./os/hal/src/serial_usb.o \
./os/hal/src/spi.o \
./os/hal/src/tm.o \
./os/hal/src/uart.o \
./os/hal/src/usb.o 

C_DEPS += \
./os/hal/src/adc.d \
./os/hal/src/can.d \
./os/hal/src/ext.d \
./os/hal/src/gpt.d \
./os/hal/src/hal.d \
./os/hal/src/i2c.d \
./os/hal/src/i2s.d \
./os/hal/src/icu.d \
./os/hal/src/mac.d \
./os/hal/src/mmc_spi.d \
./os/hal/src/mmcsd.d \
./os/hal/src/pal.d \
./os/hal/src/pwm.d \
./os/hal/src/rtc.d \
./os/hal/src/sdc.d \
./os/hal/src/serial.d \
./os/hal/src/serial_usb.d \
./os/hal/src/spi.d \
./os/hal/src/tm.d \
./os/hal/src/uart.d \
./os/hal/src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
os/hal/src/%.o: ../os/hal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


