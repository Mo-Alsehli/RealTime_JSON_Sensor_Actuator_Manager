/*
 * cJson_Functions.h
 * Author: Mohamed Magdi
 */

#ifndef CJSON_FUNCTIONS_CJSON_FUNCTIONS_H_
#define CJSON_FUNCTIONS_CJSON_FUNCTIONS_H_

#include "STM32F103x8.h"
#include "cJson.h"
#include "UART.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


typedef struct {
	char cmd[5];
	char nodeID[5];
	char data[5];
}COMMAND_TYPE;


// APIs

void parse_json_object(const char *json_string, char *jsonBuffer);
void Parse_Command(const char *json_msg, COMMAND_TYPE* currCommand);
void parse_array_of_objects(const char *json_string);
void Load_Test_Cases(const char* json_content);
int Validate_JSON_Format(const char *json_msg);

#endif /* CJSON_FUNCTIONS_CJSON_FUNCTIONS_H_ */
