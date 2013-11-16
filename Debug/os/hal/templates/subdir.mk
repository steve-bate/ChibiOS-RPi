################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/hal/templates/adc_lld.c \
../os/hal/templates/can_lld.c \
../os/hal/templates/ext_lld.c \
../os/hal/templates/gpt_lld.c \
../os/hal/templates/hal_lld.c \
../os/hal/templates/i2c_lld.c \
../os/hal/templates/icu_lld.c \
../os/hal/templates/mac_lld.c \
../os/hal/templates/pal_lld.c \
../os/hal/templates/pwm_lld.c \
../os/hal/templates/serial_lld.c \
../os/hal/templates/spi_lld.c \
../os/hal/templates/uart_lld.c \
../os/hal/templates/usb_lld.c 

OBJS += \
./os/hal/templates/adc_lld.o \
./os/hal/templates/can_lld.o \
./os/hal/templates/ext_lld.o \
./os/hal/templates/gpt_lld.o \
./os/hal/templates/hal_lld.o \
./os/hal/templates/i2c_lld.o \
./os/hal/templates/icu_lld.o \
./os/hal/templates/mac_lld.o \
./os/hal/templates/pal_lld.o \
./os/hal/templates/pwm_lld.o \
./os/hal/templates/serial_lld.o \
./os/hal/templates/spi_lld.o \
./os/hal/templates/uart_lld.o \
./os/hal/templates/usb_lld.o 

C_DEPS += \
./os/hal/templates/adc_lld.d \
./os/hal/templates/can_lld.d \
./os/hal/templates/ext_lld.d \
./os/hal/templates/gpt_lld.d \
./os/hal/templates/hal_lld.d \
./os/hal/templates/i2c_lld.d \
./os/hal/templates/icu_lld.d \
./os/hal/templates/mac_lld.d \
./os/hal/templates/pal_lld.d \
./os/hal/templates/pwm_lld.d \
./os/hal/templates/serial_lld.d \
./os/hal/templates/spi_lld.d \
./os/hal/templates/uart_lld.d \
./os/hal/templates/usb_lld.d 


# Each subdirectory must supply rules for building sources it contributes
os/hal/templates/%.o: ../os/hal/templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


