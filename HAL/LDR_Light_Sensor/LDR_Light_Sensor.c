/*
 * LDR_Light_Sensor.c
 * Author: Mohamed Magdi
 */

#include "LDR_Light_Sensor.h"

// Local Functions

float Calculate_LightIntensityPercentage(uint16_t adcValue) {
    // Constants
    const float Vcc = 3.3;       // Supply voltage
    const float R_fixed = 10000; // Fixed resistor value (10kΩ)
    const float R_min = 100;     // Minimum LDR resistance (100Ω)
    const float R_max = 200000;  // Maximum LDR resistance (200kΩ)

    // Calculate Vout from ADC value
    float Vout = (adcValue * Vcc) / 4096.0;

    // Calculate LDR resistance
    float R_LDR = R_fixed * ((Vcc / Vout) - 1);

    // Apply a logarithmic scale
    float log_R_LDR = log(R_LDR);
    float log_R_min = log(R_min);
    float log_R_max = log(R_max);
    float intensityPercentage = ((log_R_LDR - log_R_min) / (log_R_max - log_R_min)) * 100;
    // Clamp percentage to [0, 100] to avoid invalid values
    if (intensityPercentage > 100) {
        intensityPercentage = 100;
    } else if (intensityPercentage < 0) {
        intensityPercentage = 0;
    }

    return intensityPercentage;
}



// APIs
void HAL_LDR_Init(){
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
		ADC_Init(ADC1, &ADCCfg, GPIO_PIN_1);

		// Perform a Dummy Read.
		HAL_LDR_Read_Intensity();
}


float HAL_LDR_Read_Intensity(){
	uint16_t adcValue = ADC_Read_SingleChannel(ADC1, LDR_ADC_CH);
	// Calculate Intensity as a Percentage.
	float intensity;
	if(adcValue != 0){
		intensity = Calculate_LightIntensityPercentage(adcValue);
	}else {
		intensity = 0;
	}

	return intensity;
}

void HAL_LDR_UART_SendData(){
	float intensity = HAL_LDR_Read_Intensity();
	char buffer[50];
	sprintf(buffer, "Intensity: %.1f %% \r\n", intensity);
	MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
}


void HAL_LDR_DeInit(){
	// Deinitialize the ADC channel for LDR (Channel 1)
	GPIOA->CRL &= ~(0xF << (1 * 4));
	// Disable LDR Channel in the Sequence Register
	ADC1->SQR3 &= ~(0x1F);
}




