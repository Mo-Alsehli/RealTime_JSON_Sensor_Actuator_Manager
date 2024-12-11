/*
 * LM35_Temp_Sensor.c
 * Author: Mohamed Magdi
 */

#include "LM35_Temp_Sensor.h"

// APIs
void HAL_LM35_Init(){
	// ADC Configuration
	ADC_Config_t ADCCfg = {
			.mode = 0,                          // No scan mode
			.channels = 0,
			.convMode = ADC_Conv_Single_MODE,   // Single conversion mode
			.conversions = ADC_NumOfConvs_1,
			.dataAlginement = ADC_DataAlign_Right,
			.IRQ_Enable = ADC_IRQ_Disable,      // Interrupts disabled
			.P_IRQ_Callback = NULL
	};
	ADC_Init(ADC1, &ADCCfg, GPIO_PIN_0);

	// Perform a Dummy Read.
	HAL_LM35_Read();
}


float HAL_LM35_Read(){
	uint16_t adcValue = 0;

	adcValue = ADC_Read_SingleChannel(ADC1, LM35_ADC_CH);
	float voltage = (adcValue * 3.3) / 4096.0;
	float temperature = voltage * 100.0;

	return temperature;
}

void HAL_LM35_UART_SendDAta(){
	float temperature = HAL_LM35_Read();
	char buffer[50];
	sprintf(buffer, "Temperature: %.2f C \r\n", temperature);

    MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
}


void HAL_LM35_DeInit(){
	// Deinitialize the ADC channel for LM35 (Channel 0)
	    GPIOA->CRL &= ~(0xF << (0 * 4));
	    ADC1->SQR3 &= ~(0x1F);
}


