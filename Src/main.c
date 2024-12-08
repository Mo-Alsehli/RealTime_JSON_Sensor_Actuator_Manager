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
#include <cJson.h>
#include <stdlib.h>

#include "GPIO_Driver.h"
#include "EXTI_Driver.h"
#include "UART.h"
#include "RCC.h"
#include "ADC.h"
#include "LCD.h"
#include "LM35_Temp_Sensor.h"
#include "LDR_Light_Sensor.h"
#include "Relay_5V_Actuator.h"
#include "cJson_Functions.h"




// ------------------------------------------------------------------
// User Defined Macros
// ------------------------------------------------------------------


#define clk	8000000

// ------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------
char jsonBuffer[1024];

const char* json_commands[] =
{
        "{\"command\":\"ENA\",\"nodeID\":\"0x81\",\"data\":null}",
        "{\"command\":\"DIS\",\"nodeID\":\"0x80\",\"data\":null}",
        "{\"command\":\"ACT\",\"nodeID\":\"0x50\",\"data\":\"ON\"}",
        "{\"command\":\"STA\",\"nodeID\":\"0x81\",\"data\":null}",
        "{\"command\":\"DUR\",\"nodeID\":\"0x50\",\"data\":\"5\"}"
};


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

#define MAX_CMD_LENGTH			50

char rxBuffer[MAX_CMD_LENGTH];

int i = 0;

char newCommandFlag = 0;

void UART1_Recieve_JsonCommand(){
	char ch;

	MCAL_UART_ReceiveData(UART1, (uint8_t*)&ch, disable);
	MCAL_UART_SendData(UART1, (uint8_t*)&ch, 1, enable);

	if(ch == '\b' || ch == 127){
			if(i > 0){
				i--;
				rxBuffer[i] = '\0';
			}
	}else if(ch == '}'){
		if(i < (MAX_CMD_LENGTH - 1)){
			rxBuffer[i++] = ch;
			rxBuffer[i] = '\0';
			if(!(Validate_JSON_Format(rxBuffer))){
				newCommandFlag = 0;
				memset(rxBuffer, 0, MAX_CMD_LENGTH);
			}else{
				newCommandFlag = 1;
			}
			i = 0;
		}else {
			i = 0;
			newCommandFlag = 0;
			char *buffer = "\r\nError: Command too long!\r\n";
			MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
			memset(rxBuffer, 0, MAX_CMD_LENGTH);
		}
	}else{
		if(i < (MAX_CMD_LENGTH - 1)){
			rxBuffer[i++] = ch;
		}else{
			i = 0;
			newCommandFlag = 0;
			char *buffer = "\r\nError: Command too long!\r\n";
			MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
			memset(rxBuffer, 0, MAX_CMD_LENGTH);
		}

	}
}

void UART_RX_Callback(){
	UART1_Recieve_JsonCommand();
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
        .IRQ_Enable = UART_IRQ_Eanble_RXNEIE,
        .HwFlowCtl = UART_HwFlowCtl_NONE,
        .Parity = UART_Parity_NONE,
        .Mode = UART_Mode_TX_RX,
        .Payload = UART_PayloadLength_8B,
        .StopBits = UART_StopBits_One,
        .P_IRQ_Callback = UART_RX_Callback
    };
    MCAL_UART_Init(UART1, &uartCfg);
    MCAL_UART_GPIO_SetPins(UART1);

    GPIO_PinConfig_t gpioCfg;
    gpioCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
    gpioCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_2M;
    gpioCfg.GPIO_PinNumber = GPIO_PIN_11;
    MCAL_GPIO_Init(GPIOA, &gpioCfg);


    // Initialize LM35 with ADC1 ch0.
    HAL_LM35_Init();

    // Initialize LDR with ADC1 ch1.
    HAL_LDR_Init();

    // Initialize Relay Actuator.
    HAL_Relay_Init(GPIOA, GPIO_PIN_11);

    // Parsing Json Commands
    COMMAND_TYPE command;
    char buffer[50];

    while (1) {
    	//HAL_LM35_UART_SendDAta();

        //HAL_LDR_UART_SendData();
    	//delay(500, U_ms, clk);
        //HAL_Control_Relay_Light();
    	if(newCommandFlag){
    		Parse_Command((const char*)rxBuffer, &command);
    		sprintf(buffer, "\r\nCommand: %s, nodeID: %s, data: %s\r\n", command.cmd, command.nodeID, command.data);
    		MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
    		newCommandFlag = 0;
			memset(rxBuffer, 0, MAX_CMD_LENGTH);
			memset(buffer, 0, strlen(buffer));
    	}

    }
}



