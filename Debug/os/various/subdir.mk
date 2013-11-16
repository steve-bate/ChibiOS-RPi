################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/various/chprintf.c \
../os/various/chrtclib.c \
../os/various/evtimer.c \
../os/various/memstreams.c \
../os/various/shell.c \
../os/various/syscalls.c \
../os/various/usb_msc.c 

OBJS += \
./os/various/chprintf.o \
./os/various/chrtclib.o \
./os/various/evtimer.o \
./os/various/memstreams.o \
./os/various/shell.o \
./os/various/syscalls.o \
./os/various/usb_msc.o 

C_DEPS += \
./os/various/chprintf.d \
./os/various/chrtclib.d \
./os/various/evtimer.d \
./os/various/memstreams.d \
./os/various/shell.d \
./os/various/syscalls.d \
./os/various/usb_msc.d 


# Each subdirectory must supply rules for building sources it contributes
os/various/%.o: ../os/various/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


