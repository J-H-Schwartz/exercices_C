################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app_thread.c \
../src/debug.c \
../src/display_thread.c \
../src/game_p4.c \
../src/leds_control.c \
../src/main.c \
../src/numbers_module.c \
../src/queue.c \
../src/read_thread.c \
../src/timer_threads.c 

OBJS += \
./src/app_thread.o \
./src/debug.o \
./src/display_thread.o \
./src/game_p4.o \
./src/leds_control.o \
./src/main.o \
./src/numbers_module.o \
./src/queue.o \
./src/read_thread.o \
./src/timer_threads.o 

C_DEPS += \
./src/app_thread.d \
./src/debug.d \
./src/display_thread.d \
./src/game_p4.d \
./src/leds_control.d \
./src/main.d \
./src/numbers_module.d \
./src/queue.d \
./src/read_thread.d \
./src/timer_threads.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

