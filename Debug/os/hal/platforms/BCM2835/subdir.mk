################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/hal/platforms/BCM2835/bcm2835.c \
../os/hal/platforms/BCM2835/bcm2835_.c \
../os/hal/platforms/BCM2835/ext_lld.c \
../os/hal/platforms/BCM2835/gpt_lld.c \
../os/hal/platforms/BCM2835/hal_lld.c \
../os/hal/platforms/BCM2835/i2c_lld.c \
../os/hal/platforms/BCM2835/pal_lld.c \
../os/hal/platforms/BCM2835/pwm_lld.c \
../os/hal/platforms/BCM2835/serial_lld.c \
../os/hal/platforms/BCM2835/spi_lld.c 

OBJS += \
./os/hal/platforms/BCM2835/bcm2835.o \
./os/hal/platforms/BCM2835/bcm2835_.o \
./os/hal/platforms/BCM2835/ext_lld.o \
./os/hal/platforms/BCM2835/gpt_lld.o \
./os/hal/platforms/BCM2835/hal_lld.o \
./os/hal/platforms/BCM2835/i2c_lld.o \
./os/hal/platforms/BCM2835/pal_lld.o \
./os/hal/platforms/BCM2835/pwm_lld.o \
./os/hal/platforms/BCM2835/serial_lld.o \
./os/hal/platforms/BCM2835/spi_lld.o 

C_DEPS += \
./os/hal/platforms/BCM2835/bcm2835.d \
./os/hal/platforms/BCM2835/bcm2835_.d \
./os/hal/platforms/BCM2835/ext_lld.d \
./os/hal/platforms/BCM2835/gpt_lld.d \
./os/hal/platforms/BCM2835/hal_lld.d \
./os/hal/platforms/BCM2835/i2c_lld.d \
./os/hal/platforms/BCM2835/pal_lld.d \
./os/hal/platforms/BCM2835/pwm_lld.d \
./os/hal/platforms/BCM2835/serial_lld.d \
./os/hal/platforms/BCM2835/spi_lld.d 


# Each subdirectory must supply rules for building sources it contributes
os/hal/platforms/BCM2835/%.o: ../os/hal/platforms/BCM2835/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


