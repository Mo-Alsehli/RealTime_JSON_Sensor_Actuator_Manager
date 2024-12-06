/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mohamed Magdi
 * @brief          : Main program For SmartEGAT Task
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// ------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "GPIO_Driver.h"
#include "EXTI_Driver.h"
#include "TIM.h"
#include "UART.h"
#include "RCC.h"
#include "ADC.h"

// ------------------------------------------------------------------
// User Defined Macros
// ------------------------------------------------------------------


#define clk	8000000

// ------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------

int gVar = 20;
volatile uint16_t sensorValues[2]; // sensorValues[0]-> Tempreture Sensor.
								   // sensorValues[1]-> Light Sensor.
volatile uint8_t currentChannel = 0;

volatile uint8_t data[30];

float tempreture;

// ------------------------------------------------------------------
// Global functions
// ------------------------------------------------------------------


void ADC_Callback(){
//	sensorValues[currentChannel] = ADC1->DR;
//	currentChannel = (currentChannel + 1) % 2;
//	gVar += 20;
	sensorValues[0] = ADC1->DR;
	float voltage = (sensorValues[0] * 5.0) / 4096.0;
	tempreture = (voltage - 5.0) * 100.0;
}

void clockInit(){
	RCC_GPIOA_CLk_EN();
	RCC_AFIO_CLK_EN();
}

// ------------------------------------------------------------------
// Main Function
// ------------------------------------------------------------------


int main(void)
{
	// Clock Initialization
	clockInit();
	// UART Configuration.
	UART_Config_t uartCfg;
	uartCfg.BaudRate = UART_BaudRate_9600;
	uartCfg.IRQ_Enable = UART_IRQ_Eanble_NONE;
	uartCfg.HwFlowCtl = UART_HwFlowCtl_NONE;
	uartCfg.Parity = UART_Parity_NONE;
	uartCfg.Mode = UART_Mode_TX_RX;
	uartCfg.Payload = UART_PayloadLength_8B;
	uartCfg.StopBits = UART_StopBits_One;
	uartCfg.P_IRQ_Callback = NULL;
	MCAL_UART_Init(UART1, &uartCfg);
	MCAL_UART_GPIO_SetPins(UART1);

	uint8_t data[] = "Mohamed ";
	MCAL_UART_SendData(UART1, data, 8, enable);

	// ADC Configuration.
	ADC_Config_t ADCCfg;
	ADCCfg.mode = ADC_SCAN_MODE;
	ADCCfg.channels = ADC_1_2_SequenceChannels;
	ADCCfg.convMode = ADC_Conv_CONT_MODE;
	ADCCfg.conversions = ADC_NumOfConvs_2;
	ADCCfg.dataAlginement = ADC_DataAlign_Right;
	ADCCfg.IRQ_Enable = ADC_IRQ_Disable;
	ADCCfg.P_IRQ_Callback = ADC_Callback;
	ADC_Init(&ADCCfg);


	int shift = 8;
	uint8_t data1[] = "Magdi\r\n";
	MCAL_UART_SendData(UART1, data1, 6, enable);
    /* Loop forever */
	while(1){
//		if(shift == 8){
//			shift = 0;
//		}else{
//			shift = 8;
//		}
//		uint8_t ch = ((uint16_t)((sensorValues[0] << shift) + 48));
		uint8_t tempRead = ADC_Start_Reading();
		sprintf(data, "Temp: %d \r\n", tempRead);
		MCAL_UART_SendData(UART1, data, strlen(data), enable);
		delay(1000, U_ms, clk);
	}
}


