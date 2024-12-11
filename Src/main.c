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

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

// ------------------------------------------------------------------
// User Defined Macros
// ------------------------------------------------------------------


#define clk	8000000

#define MAX_CMD_LENGTH  	50

// ------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------
char volatile CMD = 0;

float tempLastStatus = 0;
float lightLastStatus = 0;
uint16_t duration;

char buffer[60];

QueueHandle_t currCommandQueue;
QueueHandle_t commandsQueue;
QueueHandle_t commandExecuteQueue;


TaskHandle_t Parse_Command_Handle;
TaskHandle_t Store_Command_Handle;
TaskHandle_t Command_Execute_Handle;

SemaphoreHandle_t RX_TX_Semaphore;


TimerHandle_t TempSensorStatusPeriodic_Timer;
TimerHandle_t LightSensorStatusPeriodic_Timer;

// ------------------------------------------------------------------
// Global functions
// ------------------------------------------------------------------


void clockInit(){
	RCC_GPIOA_CLk_EN();
	RCC_GPIOB_CLk_EN();
	RCC_AFIO_CLK_EN();
}

// FreeRTOS OS Tasks.
void Parse_Command_Handler(void *vparams);
void Store_Command_Handler(void *vparams);
void Command_Execute_Handler(void *vparams);


// Periodic Timers Callback.
void TempSensorStatusPeriodicTimer_Handler(TimerHandle_t xTimer){
	tempLastStatus = HAL_LM35_Read();
	sprintf(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x88\", \"data\": %.2f ^C}\r\n", tempLastStatus);
	for(int i = 0; i < strlen(buffer); i++)
		UART_Send_SingelChar(UART2, buffer[i], enable);
	memset(buffer, 0, 60);
}

void LightSensorStatusPeriodicTimer_Handler(TimerHandle_t xTimer){
	lightLastStatus = HAL_LDR_Read_Intensity();
	sprintf(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x88\", \"data\": %.2f ^C}\r\n", lightLastStatus);
	for(int i = 0; i < strlen(buffer); i++)
		UART_Send_SingelChar(UART2, buffer[i], enable);
	memset(buffer, 0, 60);

}


void welcomeMessage(){
	strcpy(buffer, "Welcome To SmartEGAT Task :) \r\nPlease Enter a Command: \r\n");
	for(int i = 0; i < strlen(buffer); i++){
		UART_Send_SingelChar(UART1, buffer[i], enable);
		UART_Send_SingelChar(UART2, buffer[i], enable);
	}
	memset(buffer, 0, 60);
}

// ------------------------------------------------------------------
// Main Function
// ------------------------------------------------------------------

int main(void) {
	// Clock Initialization
	clockInit();

	// UART Configuration
	UART_Config_t uartCfg ;
	uartCfg.BaudRate = UART_BaudRate_9600;
	uartCfg.IRQ_Enable = UART_IRQ_Eanble_RXNEIE;
	uartCfg.HwFlowCtl = UART_HwFlowCtl_NONE;
	uartCfg.Parity = UART_Parity_NONE;
	uartCfg.Mode = UART_Mode_TX_RX;
	uartCfg.Payload = UART_PayloadLength_8B;
	uartCfg.StopBits = UART_StopBits_One;
	uartCfg.P_IRQ_Callback = NULL;
	MCAL_UART_Init(UART1, &uartCfg);
	MCAL_UART_GPIO_SetPins(UART1);
	uartCfg.IRQ_Enable = UART_IRQ_Eanble_NONE;
	MCAL_UART_Init(UART2, &uartCfg);
	MCAL_UART_GPIO_SetPins(UART2);

	// Queues Creation.
	currCommandQueue = xQueueCreate(50, sizeof(char));
	commandsQueue = xQueueCreate(5, MAX_CMD_LENGTH);
	commandExecuteQueue = xQueueCreate(5, sizeof(COMMAND_TYPE));

	// Tasks Creation.
	xTaskCreate(Store_Command_Handler, "Store Command", 256, NULL, 2, &Store_Command_Handle);
	xTaskCreate(Parse_Command_Handler, "Parse Command", 128, NULL, 1, &Parse_Command_Handle);
	xTaskCreate(Command_Execute_Handler, "Execute Command", 512, NULL, 2, &Command_Execute_Handle);

	// Timers Creation
	TempSensorStatusPeriodic_Timer = xTimerCreate("Tempareture Status TIM", (1000 / portTICK_PERIOD_MS), pdTRUE, (void*)0, TempSensorStatusPeriodicTimer_Handler);
	LightSensorStatusPeriodic_Timer = xTimerCreate("Light Status TIM", (1000 / portTICK_PERIOD_MS), pdTRUE, (void*)1, LightSensorStatusPeriodicTimer_Handler);

	//RX_TX_Semaphore = xSemaphoreCreateBinary();

	welcomeMessage();

	// Starting FreeRTOS Schedular.
	vTaskStartScheduler();

	while (1) {
		// shouldn't reach here.
	}
}




/*
 * This function is responsible for:
 * 1. Recive Command From UART ISR.
 * 2. Validate command (from deletions).
 * 3. Validate Json Command is a valid json format.
 * 4. Send Command Via Queue to be preccessed and executed.
 */
void Store_Command_Handler(void *vparams){
	uint8_t ch = 'E';
	uint8_t commandIndex = 0;
	uint8_t cmdBuffer[MAX_CMD_LENGTH];

	while(1){
		if(xQueueReceive(currCommandQueue, &ch, (TickType_t)5) != pdFALSE){
			// Command Validation.
			if(ch == '\b' || ch == 127){
				if(commandIndex > 0){
					commandIndex--;
					cmdBuffer[commandIndex] = '\0';
				}
			}else if(ch == '}'){
				if(commandIndex < (MAX_CMD_LENGTH - 1)){
					cmdBuffer[commandIndex++] = ch;
					cmdBuffer[commandIndex] = '\0';
					if((Validate_JSON_Format(cmdBuffer))){
						xQueueSend(commandsQueue, cmdBuffer, (TickType_t) 5);
					}
					UART_Send_SingelChar(UART1, '\r', enable);
					UART_Send_SingelChar(UART1, '\n', enable);
					memset(cmdBuffer, 0, MAX_CMD_LENGTH); // Reset Command Buffer.
					commandIndex = 0;
				}else {
					char *buffer = "\r\nError: Command too long!\r\n";
					for(int i = 0; i < strlen(buffer); i++){
						UART_Send_SingelChar(UART1, buffer[i], enable);
					}
					memset(cmdBuffer, 0, MAX_CMD_LENGTH);
					commandIndex = 0;
				}
			}else{
				if(commandIndex < (MAX_CMD_LENGTH - 1)){
					cmdBuffer[commandIndex++] = ch;
				}else{
					char *buffer = "\r\nError: Command too long!\r\n";
					for(int i = 0; i < strlen(buffer); i++){
						UART_Send_SingelChar(UART1, buffer[i], enable);
					}
					memset(cmdBuffer, 0, MAX_CMD_LENGTH);
					commandIndex = 0;
				}
			}
		}
		ch = 0;
		vTaskDelay(20);
	}}


/*
 * This function is responsible for:
 * 1. Recieve command from Queue.
 * 2. Decrypt the Json command and extract data.
 * 3. Send command struct via Queue.
 */
void Parse_Command_Handler(void *vparams){
	uint8_t cmd[50];
	COMMAND_TYPE command;
	char buffer[50];
	while(1){
		if(xQueueReceive(commandsQueue, cmd, (TickType_t) 5) != pdFALSE){
			Parse_Command((const char*)cmd, &command);
			xQueueSend(commandExecuteQueue, &command, (TickType_t) 5);
			memset(buffer, 0, 50);
			memset(cmd, 0, 50);
		}
		vTaskDelay(15);
	}
}

/*
 * This function is responsible for:
 * 1. Recieve command struct.
 * 2. Validate Execute commands.
 */

void Command_Execute_Handler(void *vparams){
	COMMAND_TYPE command;
	char buffer[55];
	uint8_t relayLastStatus = 0;

	while(1){
		if(xQueueReceive(commandExecuteQueue, &command, (TickType_t) 5)){
			if(strcmp(command.cmd, "ENA") == 0){
				if(strcmp(command.nodeID, "0x50") == 0){ // Node ID For Relay Actuator.
					HAL_Relay_Init(GPIOA, GPIO_PIN_11);
					strcpy(buffer, "{\"nodeType\":\"NA\", \"nodeID\": \"0x50\", \"data\":\"DONE\"}\r\n");
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x80") == 0){ // Node ID For Tempareture Sensor.
					HAL_LM35_Init();
					strcpy(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x80\", \"data\":\"DONE\"}\r\n");
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x81") == 0){ // Node ID For Lights Sensor.
					HAL_LDR_Init();
					strcpy(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x81\", \"data\":\"DONE\"}\r\n");
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else{
					strcpy(buffer, "Unknown NodeID!!\r\n");
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART1, buffer[i], enable);
				}
			}else if(strcmp(command.cmd, "ACT") == 0){
				if(strcmp(command.nodeID, "0x50") == 0){ // Node ID For Relay Actuator.
					if(strcmp(command.data, "0") == 0){
						HAL_Relay_ACT(GPIOA, GPIO_PIN_11, GPIO_PIN_LOW);
					}else if(strcmp(command.data, "1") == 0){
						HAL_Relay_ACT(GPIOA, GPIO_PIN_11, GPIO_PIN_HIGH);
					}else{
						strcpy(buffer, "Invalid Actuator Data!!\r\n");
						for(int i = 0; i < strlen(buffer); i++)
							UART_Send_SingelChar(UART1, buffer[i], enable);
					}
				}else{
					strcpy(buffer, "Invalid Actuator NodeID!!\r\n");
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART1, buffer[i], enable);
				}
			}else if(strcmp(command.cmd, "STA") == 0){
				if(strcmp(command.nodeID, "0x50") == 0){
					relayLastStatus = MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
					sprintf(buffer, "{\"nodeType\":\"NA\", \"nodeID\": \"0x50\", \"data\": %d}\r\n", relayLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x80") == 0){
					tempLastStatus = HAL_LM35_Read();
					sprintf(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x88\", \"data\": %.2f ^C}\r\n", tempLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x81") == 0){
					lightLastStatus = HAL_LDR_Read_Intensity();
					sprintf(buffer, "{\"nodeType\":\"NS\", \"nodeID\": \"0x81\", \"data\": %.2f}\r\n", lightLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}
				memset(buffer, 0, 50);
			}else if(strcmp(command.cmd, "DUR") == 0){
				if(strcmp(command.nodeID, "0x80") == 0){
					xTimerStart(TempSensorStatusPeriodic_Timer, portMAX_DELAY);
					if(command.nodeID != 0){
						sscanf(command.data, "%hd", &duration);
						duration = duration*200;
						xTimerChangePeriod(TempSensorStatusPeriodic_Timer, (duration/portTICK_PERIOD_MS), (TickType_t)20);
					}
				}else if(strcmp(command.nodeID, "0x81") == 0){
					xTimerStart(LightSensorStatusPeriodic_Timer, portMAX_DELAY);
					if(command.nodeID != 0){
						sscanf(command.data, "%hd", &duration);
						duration = duration*200;
						xTimerChangePeriod(LightSensorStatusPeriodic_Timer, (duration/portTICK_PERIOD_MS), (TickType_t)20);
					}
				}
			}else if(strcmp(command.cmd, "DIS") == 0){
				if(strcmp(command.nodeID, "0x81") == 0){
					xTimerStop(LightSensorStatusPeriodic_Timer, portMAX_DELAY);
				}else if(strcmp(command.nodeID, "0x80") == 0){
					xTimerStop(TempSensorStatusPeriodic_Timer, portMAX_DELAY);
				}
			}else {
				strcpy(buffer, "ERROR: Invalid Command!!\r\n");
				for(int i = 0; i < strlen(buffer); i++)
					UART_Send_SingelChar(UART1, buffer[i], enable);
				memset(buffer, 0, 50);
			}
		}
		vTaskDelay(15);
	}
}

/*
 * ISR For UART RX.
 */
void USART1_IRQHandler() {
	if ((UART1->SR & (1 << 5)) >> 5) {  // Check if RXNE interrupt is set
		//MCAL_UART_ReceiveData(UART1, (uint8_t*)&CMD, disable);  // Read data
		CMD = UART1->DR;
		UART_Send_SingelChar(UART1, CMD, enable);  // Echo back or process the command
		xQueueSend(currCommandQueue, &CMD, (TickType_t) 5);
		// Clear the RXNE flag (by reading DR or explicitly if needed by hardware)
		UART1->SR &= ~(1 << 5);
	}
}


