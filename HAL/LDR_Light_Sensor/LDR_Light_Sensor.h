/*
 * LDR_Light_Sensor.h
 * Author: Mohamed Magdi
 */

#ifndef LDR_LIGHT_SENSOR_LDR_LIGHT_SENSOR_H_
#define LDR_LIGHT_SENSOR_LDR_LIGHT_SENSOR_H_

#include "STM32F103x8.h"
#include "GPIO_Driver.h"
#include "UART.h"
#include "ADC.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// User Defined Macros.

#define LDR_ADC_CH			1		// ADC Channel 1


// APIs
void HAL_LDR_Init();
float HAL_LDR_Read_Intensity();
void HAL_LDR_UART_SendData();
void HAL_LDR_DeInit();




#endif /* LDR_LIGHT_SENSOR_LDR_LIGHT_SENSOR_H_ */
