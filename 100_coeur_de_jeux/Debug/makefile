################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: 100_coeur_de_jeux

# Tool invocations
100_coeur_de_jeux: $(OBJS) $(USER_OBJS)
	gcc  -o "100_coeur_de_jeux" $(OBJS) $(USER_OBJS) $(LIBS)

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) 100_coeur_de_jeux
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
