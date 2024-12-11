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

//#include "FreeRTOS_Tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// ------------------------------------------------------------------
// User Defined Macros
// ------------------------------------------------------------------


#define clk	8000000

#define MAX_CMD_LENGTH  	50

// ------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------
const char* json_commands[] =
{
		"{\"command\":\"ENA\",\"nodeID\":\"0x81\",\"data\":null}",
		"{\"command\":\"DIS\",\"nodeID\":\"0x80\",\"data\":null}",
		"{\"command\":\"ACT\",\"nodeID\":\"0x50\",\"data\":\"ON\"}",
		"{\"command\":\"STA\",\"nodeID\":\"0x81\",\"data\":null}",
		"{\"command\":\"DUR\",\"nodeID\":\"0x50\",\"data\":\"5\"}"
};


//QueueHandle_t commandQueue;

char volatile CMD = 0;
int i = 0;
int j = 0;



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
	RCC_GPIOB_CLk_EN();
	RCC_AFIO_CLK_EN();
}

QueueHandle_t currCommandQueue;
QueueHandle_t commandsQueue;
QueueHandle_t commandExecuteQueue;

void UART1_RXe_Callback(void);

TaskHandle_t parse;

TaskHandle_t Parse_Command_Handle;
TaskHandle_t Store_Command_Handle;
TaskHandle_t Command_Execute_Handle;

SemaphoreHandle_t RX_TX_Semaphore;



void Parse_Command_Handler(void *vparams);
void Store_Command_Handler(void *vparams);
void Command_Execute_Handler(void *vparams);

// ------------------------------------------------------------------
// Main Function
// ------------------------------------------------------------------


int main(void) {
	// Clock Initialization
	clockInit();

	//GPIOAInit();
	//LCDInit();

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

	// queue creation
	//commandQueue = xQueueCreate(COMMAND_QUEUE_LENGTH, COMMAND_BUFFER_SIZE);

	//task creation
	//	if (xTaskCreate(CommandProcessing_Task, "Json Command Processing Task", 512, NULL, 3, &parse) != pdPASS) {
	//			char *buffer = "Error: Create_CommandProcessing_Task creation failed!\n";
	//			MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
	//		}
	//
	//	if (xTaskCreate(UART1_Recieve_JsonCommand, "UART RX JSON Command", 128, NULL, 1, NULL) != pdPASS) {
	//			char *buffer = "Error: Create_UART_RX_Task creation failed!\n";
	//			MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
	//		}
	currCommandQueue = xQueueCreate(50, sizeof(char));
	commandsQueue = xQueueCreate(5, MAX_CMD_LENGTH);
	commandExecuteQueue = xQueueCreate(5, sizeof(COMMAND_TYPE));


	xTaskCreate(Store_Command_Handler, "Store Command", 256, NULL, 2, &Store_Command_Handle);
	xTaskCreate(Parse_Command_Handler, "Parse Command", 128, NULL, 1, &Parse_Command_Handle);
	xTaskCreate(Command_Execute_Handler, "Execute Command", 256, NULL, 1, &Command_Execute_Handle);
	//RX_TX_Semaphore = xSemaphoreCreateBinary();

	MCAL_UART_SendData(UART1, (uint8_t*)"here\r\n", 6, enable);
	MCAL_UART_SendData(UART2, (uint8_t*)"here\r\n", 6, enable);
	vTaskStartScheduler();


	// Initialize LM35 with ADC1 ch0.
	//HAL_LM35_Init();

	// Initialize LDR with ADC1 ch1.
	//HAL_LDR_Init();

	// Initialize Relay Actuator.
	//HAL_Relay_Init(GPIOA, GPIO_PIN_11);


	while (1) {

	}
}

//void UART1_RX_Monitor_Handler(void *vparams){
//	uint8_t ch = '$';
//	while(1){
//		MCAL_UART_ReceiveData(UART1, (uint8_t*)&ch, enable);
//		if(ch != '$'){
//			//xQueueSend(currCommandQueue, ch, portMAX_DELAY);
//			xSemaphoreGive(RX_TX_Semaphore);
//		}
//		ch = '$';
//		vTaskDelay(25);
//	}
//}

void Store_Command_Handler(void *vparams){
	uint8_t ch = 'E';
	uint8_t commandIndex = 0;
	uint8_t cmdBuffer[MAX_CMD_LENGTH];

	while(1){
		if(xQueueReceive(currCommandQueue, &ch, (TickType_t)5) != pdFALSE){
			//cmdBuffer[commandIndex++] = ch;
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
				if(i < (MAX_CMD_LENGTH - 1)){
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

void Command_Execute_Handler(void *vparams){
	COMMAND_TYPE command;
	char buffer[50];
	uint8_t relayLastStatus = 0;
	float tempLastStatus = 0;
	float lightLastStatus = 0;
	while(1){
		if(xQueueReceive(commandExecuteQueue, &command, (TickType_t) 5)){
			if(strcmp(command.cmd, "ENA") == 0){
				if(strcmp(command.nodeID, "0x50") == 0){ // Node ID For Relay Actuator.
					HAL_Relay_Init(GPIOA, GPIO_PIN_11);
				}else if(strcmp(command.nodeID, "0x80") == 0){ // Node ID For Tempareture Sensor.
					HAL_LM35_Init();
				}else if(strcmp(command.nodeID, "0x81") == 0){ // Node ID For Lights Sensor.
					HAL_LDR_Init();
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
					sprintf(buffer, "Relay Status: %d\r\n", relayLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x80") == 0){
					tempLastStatus = HAL_LM35_Read();
					sprintf(buffer, "Tempreture Status: %.2f\r\n", tempLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}else if(strcmp(command.nodeID, "0x81") == 0){
					lightLastStatus = HAL_LDR_Read_Intensity();
					sprintf(buffer, "Intensity Last Status: %.2f %%\r\n", lightLastStatus);
					for(int i = 0; i < strlen(buffer); i++)
						UART_Send_SingelChar(UART2, buffer[i], enable);
				}
				memset(buffer, 0, 50);
			}
		}
		vTaskDelay(15);
	}
}


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







//void UART1_Recieve_JsonCommand(void *pvParams){
//	while(1){
//		//i++;
//		//Simple_UART_Send(UART1, 'N');
//		MCAL_UART_ReceiveData(UART1, (uint8_t*)&ch, enable);
//		MCAL_UART_SendData(UART1, (uint8_t*)&ch, 1, enable);
//		//
//		//		if(ch == '\b' || ch == 127){
//		//			if(i > 0){
//		//				i--;
//		//				rxBuffer[i] = '\0';
//		//			}
//		//		}else if(ch == '}'){
//		//			if(i < (MAX_CMD_LENGTH - 1)){
//		//				rxBuffer[i++] = ch;
//		//				rxBuffer[i] = '\0';
//		//				if(!(Validate_JSON_Format(rxBuffer))){
//		//					memset(rxBuffer, 0, MAX_CMD_LENGTH);
//		//				}else{
//		//					 //check if queue is full and add current command to it.
//		//					if (xQueueSend(commandQueue, rxBuffer, portMAX_DELAY) != pdPASS) {
//		//						char *buffer = "\r\nError: Commands Queue Is Full!!\r\n";
//		//						MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
//		//					}
//		//					memset(rxBuffer, 0, MAX_CMD_LENGTH); // Reset Command Buffer.
//		//				}
//		//				i = 0; // Reset Command Buffer Index.
//		//			}else {
//		//				i = 0;
//		//				char *buffer = "\r\nError: Command too long!\r\n";
//		//				MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
//		//				memset(rxBuffer, 0, MAX_CMD_LENGTH);
//		//			}
//		//		}else{
//		//			if(i < (MAX_CMD_LENGTH - 1)){
//		//				rxBuffer[i++] = ch;
//		//			}else{
//		//				i = 0;
//		//				char *buffer = "\r\nError: Command too long!\r\n";
//		//				MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
//		//				memset(rxBuffer, 0, MAX_CMD_LENGTH);
//		//			}
//		//		}
//		//MCAL_UART_SendData(UART1, (uint8_t*)"M", 1, enable);
//		vTaskDelay(pdMS_TO_TICKS(20));
//	}
//}


void CommandProcessing_Task(void *pvParams){
	while(1){
		j++;
		//MCAL_UART_SendData(UART1, (uint8_t*)"N", 1, enable);
		//		if(xQueueReceive(commandQueue, commandBuffer, portMAX_DELAY) == pdPASS){
		//			Parse_Command((const char*)commandBuffer, &command);
		//			sprintf(buffer, "\r\nCommand: %s, nodeID: %s, data: %s\r\n", command.cmd, command.nodeID, command.data);
		//			MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
		//			memset(buffer, 0, strlen(buffer));
		//			memset(commandBuffer, 0, MAX_CMD_LENGTH);
		//		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

