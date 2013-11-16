################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/kernel/src/chcond.c \
../os/kernel/src/chdebug.c \
../os/kernel/src/chdynamic.c \
../os/kernel/src/chevents.c \
../os/kernel/src/chheap.c \
../os/kernel/src/chlists.c \
../os/kernel/src/chmboxes.c \
../os/kernel/src/chmemcore.c \
../os/kernel/src/chmempools.c \
../os/kernel/src/chmsg.c \
../os/kernel/src/chmtx.c \
../os/kernel/src/chqueues.c \
../os/kernel/src/chregistry.c \
../os/kernel/src/chschd.c \
../os/kernel/src/chsem.c \
../os/kernel/src/chsys.c \
../os/kernel/src/chthreads.c \
../os/kernel/src/chvt.c 

OBJS += \
./os/kernel/src/chcond.o \
./os/kernel/src/chdebug.o \
./os/kernel/src/chdynamic.o \
./os/kernel/src/chevents.o \
./os/kernel/src/chheap.o \
./os/kernel/src/chlists.o \
./os/kernel/src/chmboxes.o \
./os/kernel/src/chmemcore.o \
./os/kernel/src/chmempools.o \
./os/kernel/src/chmsg.o \
./os/kernel/src/chmtx.o \
./os/kernel/src/chqueues.o \
./os/kernel/src/chregistry.o \
./os/kernel/src/chschd.o \
./os/kernel/src/chsem.o \
./os/kernel/src/chsys.o \
./os/kernel/src/chthreads.o \
./os/kernel/src/chvt.o 

C_DEPS += \
./os/kernel/src/chcond.d \
./os/kernel/src/chdebug.d \
./os/kernel/src/chdynamic.d \
./os/kernel/src/chevents.d \
./os/kernel/src/chheap.d \
./os/kernel/src/chlists.d \
./os/kernel/src/chmboxes.d \
./os/kernel/src/chmemcore.d \
./os/kernel/src/chmempools.d \
./os/kernel/src/chmsg.d \
./os/kernel/src/chmtx.d \
./os/kernel/src/chqueues.d \
./os/kernel/src/chregistry.d \
./os/kernel/src/chschd.d \
./os/kernel/src/chsem.d \
./os/kernel/src/chsys.d \
./os/kernel/src/chthreads.d \
./os/kernel/src/chvt.d 


# Each subdirectory must supply rules for building sources it contributes
os/kernel/src/%.o: ../os/kernel/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/run/workspace/ChibiOS-RPi/os/hal/include" -I"/home/run/workspace/ChibiOS-RPi/os/kernel/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


