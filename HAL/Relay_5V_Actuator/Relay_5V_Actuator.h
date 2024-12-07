/*
 * Realy_5V_Actuator.h
 * Author: Mohamed Magdi
 */

#ifndef RELAY_5V_ACTUATOR_RELAY_5V_ACTUATOR_H_
#define RELAY_5V_ACTUATOR_RELAY_5V_ACTUATOR_H_


#include "STM32F103x8.h"
#include "GPIO_Driver.h"
#include "LDR_Light_Sensor.h"
#include "LM35_Temp_Sensor.h"


void HAL_Relay_Init(GPIO_TYPE_DEF* gpio, uint16_t GPIO_PIN_NUM);
void HAL_Control_Relay_Light();
void HAL_Control_Relay_Temperature();
void HAL_Relay_DeInit(GPIO_TYPE_DEF* gpio, uint16_t GPIO_PIN_NUM);



#endif /* RELAY_5V_ACTUATOR_RELAY_5V_ACTUATOR_H_ */
