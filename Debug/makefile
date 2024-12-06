################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include MCAL/UART/subdir.mk
-include MCAL/TIM/subdir.mk
-include MCAL/RCC/subdir.mk
-include MCAL/GPIO/subdir.mk
-include MCAL/EXTI/subdir.mk
-include MCAL/ADC/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SmartEGAT_Task.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SmartEGAT_Task.list \

OBJCOPY_HEX += \
SmartEGAT_Task.hex \

OBJCOPY_BIN += \
SmartEGAT_Task.bin \


# All Target
all: SmartEGAT_Task.elf secondary-outputs

# Tool invocations
SmartEGAT_Task.elf: $(OBJS) $(USER_OBJS) C:\Users\dell\STM32CubeIDE\workspace_1.4.0\SmartEGAT_Task\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "SmartEGAT_Task.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\dell\STM32CubeIDE\workspace_1.4.0\SmartEGAT_Task\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SmartEGAT_Task.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SmartEGAT_Task.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SmartEGAT_Task.list"
	@echo 'Finished building: $@'
	@echo ' '

SmartEGAT_Task.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "SmartEGAT_Task.hex"
	@echo 'Finished building: $@'
	@echo ' '

SmartEGAT_Task.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "SmartEGAT_Task.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
