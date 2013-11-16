################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/various/fatfs_bindings/fatfs_diskio.c \
../os/various/fatfs_bindings/fatfs_syscall.c 

OBJS += \
./os/various/fatfs_bindings/fatfs_diskio.o \
./os/various/fatfs_bindings/fatfs_syscall.o 

C_DEPS += \
./os/various/fatfs_bindings/fatfs_diskio.d \
./os/various/fatfs_bindings/fatfs_syscall.d 


# Each subdirectory must supply rules for building sources it contributes
os/various/fatfs_bindings/%.o: ../os/various/fatfs_bindings/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


