################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/debug.c \
../src/game_p4.c \
../src/leds_control.c \
../src/main.c \
../src/numbers_module.c \
../src/queue.c 

OBJS += \
./src/debug.o \
./src/game_p4.o \
./src/leds_control.o \
./src/main.o \
./src/numbers_module.o \
./src/queue.o 

C_DEPS += \
./src/debug.d \
./src/game_p4.d \
./src/leds_control.d \
./src/main.d \
./src/numbers_module.d \
./src/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

