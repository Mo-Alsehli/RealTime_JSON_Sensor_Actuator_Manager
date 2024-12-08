################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/UART/UART.c 

OBJS += \
./MCAL/UART/UART.o 

C_DEPS += \
./MCAL/UART/UART.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/UART/UART.o: ../MCAL/UART/UART.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Libs/cJson_Functions" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Libs/cJson" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/Relay_5V_Actuator" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LDR_Light_Sensor" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LM35_Temp_Sensor" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LCD" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/ADC" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Inc" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/EXTI" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/GPIO" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/RCC" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/TIM" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/UART" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/UART/UART.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

