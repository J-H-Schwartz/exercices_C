################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/compare.c \
../src/main.c 

OBJS += \
./src/compare.o \
./src/main.o 

C_DEPS += \
./src/compare.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
src/main.o: ../src/main.c
	gcc -I/home/jonathan/Desktop/Repositories/exercices_C/66_compare_chaine/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/main.d" -o "$@" "$<"

