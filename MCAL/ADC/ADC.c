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
void ADC_Init(ADC_Config_t* ADCCfg) {
    // Enable clocks for ADC and GPIO
    RCC_GPIOA_CLk_EN();
    RCC_ADC1_CLK_EN();

    // Configure GPIO Pins PA0 and PA1 for analog input (reset to analog mode)
    GPIOA->CRL &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));

    // Enable ADC by setting ADON bit
    ADC1->CR2 |= (1 << 0);

    // Configure ADC channels (set the sequence in SQR3)
    ADC1->SQR3 = ADCCfg->channels;

    // Set continuous or single conversion mode
    if (ADCCfg->convMode == ADC_Conv_CONT_MODE) {
        ADC1->CR2 |= (1 << 1); // Enable continuous mode
    } else if (ADCCfg->convMode == ADC_Conv_Single_MODE) {
        ADC1->CR2 &= ~(1 << 1); // Disable continuous mode (single conversion)
    }

    // Configure data alignment (right or left)
    if (ADCCfg->dataAlginement == ADC_DataAlign_Right) {
        ADC1->CR2 &= ~(1 << 11); // Right alignment
    } else if (ADCCfg->dataAlginement == ADC_DataAlign_Left) {
        ADC1->CR2 |= (1 << 11); // Left alignment
    }

    // Enable or disable End of Conversion (EOC) interrupt
    if (ADCCfg->IRQ_Enable == ADC_IRQ_Enable) {
        NVIC_IRQ18_ADC_Enable; // Enable ADC interrupt in NVIC
        ADC1->CR1 |= (1 << 5); // Enable EOC interrupt
    } else if (ADCCfg->IRQ_Enable == ADC_IRQ_Disable) {
        ADC1->CR1 &= ~(1 << 5); // Disable EOC interrupt
    }

    // Set ADC callback if interrupt mode is enabled
    G_ADC_Callback = ADCCfg->P_IRQ_Callback;

    // Wait for ADC stabilization (tSTAB)
    for (int i = 0; i <= 1000; i++);

    // Set ADON again to start the ADC
    ADC1->CR2 |= (1 << 0);
}


uint16_t ADC_Read(uint8_t channel) {
    // Select the ADC channel
    ADC1->SQR3 = channel;

    // Start the conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for the End of Conversion (EOC) flag
    while (!(ADC1->SR & ADC_SR_EOC));

    // Read the ADC value
    uint16_t adcValue = ADC1->DR;

    // Clear the EOC flag (optional, reading ADC1->DR usually clears it)
    ADC1->SR &= ~ADC_SR_EOC;
    ADC1->SR &= ~(1 << 4);

    return adcValue;
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
