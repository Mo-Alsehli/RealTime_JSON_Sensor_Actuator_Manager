/*
 * FreeRTOS_Tasks.h
 *
 *  Created on: Dec 10, 2024
 *      Author: dell
 */

#ifndef FREERTOS_TASKS_DEFINITIONS_FREERTOS_TASKS_H_
#define FREERTOS_TASKS_DEFINITIONS_FREERTOS_TASKS_H_

#include "STM32F103x8.h"
#include "UART.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cJson_Functions.h"
#include "queue.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND_QUEUE_LENGTH 	5
#define COMMAND_BUFFER_SIZE 	50
#define MAX_CMD_LENGTH			50




void UART1_Recieve_JsonCommand();
void Create_UART_RX_Task(void);
void CommandProcessing_Task();
void Create_CommandProcessing_Task();

#endif /* FREERTOS_TASKS_DEFINITIONS_FREERTOS_TASKS_H_ */
