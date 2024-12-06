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
#include "LCD.h"

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

volatile char data[100];

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

    // ADC Configuration
    ADC_Config_t ADCCfg = {
        .mode = 0,                          // No scan mode
        .channels = ADC_1_SequenceChannels, // Channel 0
        .convMode = ADC_Conv_CONT_MODE,   // Single conversion mode
        .conversions = ADC_NumOfConvs_1,
        .dataAlginement = ADC_DataAlign_Right,
        .IRQ_Enable = ADC_IRQ_Disable,      // Interrupts disabled
        .P_IRQ_Callback = NULL
    };
    ADC_Init(&ADCCfg);

    char buffer[50];
    uint16_t adcValue = 0;
    float temperature = 0;

    while (1) {
        // Read ADC value from channel 0
        adcValue = ADC_Read(0);

        // Convert ADC value to temperature
        float voltage = (adcValue * 3.3) / 4096.0; // Assuming a 12-bit ADC and 3.3V reference
        temperature = voltage * 100.0;            // For an LM35 sensor
        //int var = 0;
        // Print the temperature to UART
        sprintf(buffer, "Temp: %.2f C\r\n", temperature);
        MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
        //sendChar('M');
        //sendStr((uint8_t*)buffer);
        // Add a delay
        //delay(1000, U_ms, clk);
        wait(500);
        //displayClear();
    }
}



