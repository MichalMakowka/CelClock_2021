################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CelClock_2021.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CelClock_2021.list \

OBJCOPY_BIN += \
CelClock_2021.bin \


# All Target
all: CelClock_2021.elf secondary-outputs

# Tool invocations
CelClock_2021.elf: $(OBJS) $(USER_OBJS) C:\Users\hp\STM32CubeIDE\workspace_1.3.0\CelClock_2021\STM32G071CBTX_FLASH.ld
	arm-none-eabi-gcc -o "CelClock_2021.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\hp\STM32CubeIDE\workspace_1.3.0\CelClock_2021\STM32G071CBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CelClock_2021.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CelClock_2021.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CelClock_2021.list"
	@echo 'Finished building: $@'
	@echo ' '

CelClock_2021.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CelClock_2021.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
