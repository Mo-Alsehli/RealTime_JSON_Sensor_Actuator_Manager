/*
 * ADC.c
 * Author: Mohamed Magdi
 */

#include "ADC.h"

// Global Variables.
void (*G_ADC_Callback)(void);

/*
 * Function Description: This function is used to initialize ADC1
 * 						 With two channels 0 and 1.
 *
 */
void ADC_Init(ADC_Config_t* ADCCfg){
	// Enable Clocks for ADC and GPIO.
	RCC_GPIOA_CLk_EN();
	RCC_ADC1_CLK_EN();

	// Configure GPIO Pins PA0 and PA1 for analog Input.
	// Reset the first 8-bits in the CRL Register.
	GPIOA->CRL &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));

	/******** ADC Configuration ***********/
	// Enable ADC.
	ADC1->CR2 |= (1 << 0);

	// Input Channels.
	ADC1->SQR3 = ADCCfg->channels;

	// ADC Mode.
	ADC1->CR1 |= ADCCfg->mode;

	G_ADC_Callback = ADCCfg->P_IRQ_Callback;

	// ADC Converstion Mode (CONT - Single).
	if(ADCCfg->convMode == ADC_Conv_CONT_MODE){
		ADC1->CR2 |= (1 << 1);
	}else if(ADCCfg->convMode == ADC_Conv_Single_MODE){
		ADC1->CR2 &= ~(1 << 1);
	}else{}

	// Data Alignment
	if(ADCCfg->dataAlginement == ADC_DataAlign_Right){
		ADC1->CR2 &= ~(1 << 11);
	}else if(ADCCfg->dataAlginement == ADC_DataAlign_Left){
		ADC1->CR2 |= (1 << 11);
	}else{}

	// ADC Converstion Complete Interrupt Enable (EOCIE).
	if(ADCCfg->IRQ_Enable == ADC_IRQ_Enable){
		NVIC_IRQ18_ADC_Enable;
		ADC1->CR1 |= (1 << 5);
	}else if(ADCCfg->IRQ_Enable == ADC_IRQ_Disable){
		ADC1->CR1 &= ~(1 << 5);
	}

	// Start Conversion.
	// Small Delay(tSTAB).
	for(int i = 0; i <= 1000; i++);
	// Set ADON again to start conversion.
	ADC1->CR2 |= (1 << 0);
	// Set SWSTART Bit to start trigger by SW (best for operation).
	ADC1->CR2 |= (0b111 << 17);
}

void ADC_DeInit(){
	RCC_ADC1_CLK_DIS();
	NVIC_IRQ18_ADC_Disable;
	ADC1->CR2 &= (1 << 0);
}

void ADC1_2_IRQHandler(){
	if(G_ADC_Callback){
		G_ADC_Callback();
	}
	ADC1->SR &= ~(1 << 1);
}
