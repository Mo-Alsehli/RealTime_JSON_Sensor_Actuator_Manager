/*
 * LM35_Temp_Sensor.h
 * Author: Mohamed Magdi
 */

#ifndef LM35_TEMP_SENSOR_LM35_TEMP_SENSOR_H_
#define LM35_TEMP_SENSOR_LM35_TEMP_SENSOR_H_

#include "STM32F103x8.h"
#include <stdio.h>
#include <string.h>
#include "UART.h"
#include "ADC.h"
#include "GPIO_Driver.h"

// User Defined Macros.
#define LM35_ADC_CH					0			// Channel 0





// APIs
void HAL_LM35_Init();
float HAL_LM35_Read();
void HAL_LM35_UART_SendDAta();
void HAL_LM35_DeInit();




#endif /* LM35_TEMP_SENSOR_LM35_TEMP_SENSOR_H_ */
