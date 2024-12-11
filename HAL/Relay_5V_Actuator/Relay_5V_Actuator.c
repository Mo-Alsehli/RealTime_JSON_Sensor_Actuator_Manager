/*
 * Realy_5V_Actuator.c
 * Author: Mohamed Magdi
 */

#include "Relay_5V_Actuator.h"



void HAL_Relay_Init(GPIO_TYPE_DEF* gpio, uint16_t GPIO_PIN_NUM){
	GPIO_PinConfig_t gpioCfg;
	gpioCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	gpioCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_2M;
	gpioCfg.GPIO_PinNumber = GPIO_PIN_NUM;
	MCAL_GPIO_Init(gpio, &gpioCfg);
}


void HAL_Control_Relay_Light(){
	float intensity = HAL_LDR_Read_Intensity();

	if(intensity > 40){
		// High means to open the relay as it's NC by default.
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_HIGH);
	}else{
		// Low means to close the relay (switch lamp on).
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_LOW);
	}
}


void HAL_Control_Relay_Temperature(){
	float temperature = HAL_LM35_Read();

	if(temperature > 25){
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_HIGH);
	}else{
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_LOW);
	}
}

void HAL_Relay_ACT(GPIO_TYPE_DEF* gpio, uint16_t GPIO_PIN_NUM, uint8_t data){
	if(data == 1){
		MCAL_GPIO_WritePin(gpio, GPIO_PIN_NUM, GPIO_PIN_HIGH);
	}else{
		MCAL_GPIO_WritePin(gpio, GPIO_PIN_NUM, GPIO_PIN_LOW);
	}
}


