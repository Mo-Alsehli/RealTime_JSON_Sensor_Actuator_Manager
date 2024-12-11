################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/Relay_5V_Actuator/Relay_5V_Actuator.c 

OBJS += \
./HAL/Relay_5V_Actuator/Relay_5V_Actuator.o 

C_DEPS += \
./HAL/Relay_5V_Actuator/Relay_5V_Actuator.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/Relay_5V_Actuator/Relay_5V_Actuator.o: ../HAL/Relay_5V_Actuator/Relay_5V_Actuator.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Middlewares/freeRTOS/include" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Src/FreeRTOS_Tasks_Definitions" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Middlewares/freeRTOS/portable/GCC/ARM_CM3" -I../Inc -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Libs/cJson_Functions" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Libs/cJson" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/Relay_5V_Actuator" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LDR_Light_Sensor" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LM35_Temp_Sensor" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/HAL/LCD" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/ADC" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/Inc" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/EXTI" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/GPIO" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/RCC" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/TIM" -I"C:/Users/dell/STM32CubeIDE/workspace_1.4.0/SmartEGAT_Task/MCAL/UART" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/Relay_5V_Actuator/Relay_5V_Actuator.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

