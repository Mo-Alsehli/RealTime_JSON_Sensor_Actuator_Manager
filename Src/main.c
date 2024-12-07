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
#include "UART.h"
#include "RCC.h"
#include "ADC.h"
#include "LCD.h"
#include "LM35_Temp_Sensor.h"
#include "LDR_Light_Sensor.h"


// ------------------------------------------------------------------
// User Defined Macros
// ------------------------------------------------------------------


#define clk	8000000

// ------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------

// ------------------------------------------------------------------
// Global functions
// ------------------------------------------------------------------

/*
float temperature;
void ADC_Callback(){
	uint16_t adcValue = ADC1->DR;
	float voltage = (adcValue * 5.0) / 4096.0;
	temperature = (voltage - 5.0) * 100.0;
}
*/

void clockInit(){
	RCC_GPIOA_CLk_EN();
	RCC_AFIO_CLK_EN();
}

void GPIOAInit(){
	GPIO_PinConfig_t gpioCfg;
	    gpioCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	    gpioCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_2M;
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_0;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_1;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_2;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_3;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_4;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_5;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_6;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_7;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_8;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_9;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
	    gpioCfg.GPIO_PinNumber = GPIO_PIN_10;
	    MCAL_GPIO_Init(GPIOA, &gpioCfg);
}

// ------------------------------------------------------------------
// Main Function
// ------------------------------------------------------------------


int main(void) {
    // Clock Initialization
    clockInit();
    //GPIOAInit();
    //LCDInit();

    // UART Configuration
    UART_Config_t uartCfg = {
        .BaudRate = UART_BaudRate_9600,
        .IRQ_Enable = UART_IRQ_Eanble_NONE,
        .HwFlowCtl = UART_HwFlowCtl_NONE,
        .Parity = UART_Parity_NONE,
        .Mode = UART_Mode_TX_RX,
        .Payload = UART_PayloadLength_8B,
        .StopBits = UART_StopBits_One,
        .P_IRQ_Callback = NULL
    };
    MCAL_UART_Init(UART1, &uartCfg);
    MCAL_UART_GPIO_SetPins(UART1);


    // Initialize LM35 with ADC1 ch0
    HAL_LM35_Init();

    // Initialize LDR with ADC1 ch1
    HAL_LDR_Init();

    while (1) {
    	//HAL_LM35_UART_SendDAta();
        wait(100);
        HAL_LDR_UART_SendData();
        wait(200);
    	//delay(500, U_ms, clk);
    }
}



