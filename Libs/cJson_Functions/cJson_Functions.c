/*
 * cJson_Functions.c
 * Author: Mohamed Magdi
 */

#include "cJson_Functions.h"


void parse_json_object(const char *json_string, char *jsonBuffer) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        sprintf(jsonBuffer, "name: %s\n", name->valuestring);
    }

    cJSON_Delete(json);
}


void Parse_Command(const char *json_msg, COMMAND_TYPE* currCommand) {
    cJSON *root = cJSON_Parse(json_msg);
    if (!root) {
        printf("Error: JSON Parse Failed\r\n");
        return;
    }

    cJSON *command = cJSON_GetObjectItemCaseSensitive(root, "command");
    cJSON *nodeID = cJSON_GetObjectItemCaseSensitive(root, "nodeID");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");

    if(cJSON_IsString(command)  && cJSON_IsString(nodeID)){
        //MCAL_UART_SendData(UART1, (uint8_t*)command, 3, enable);

    	strcpy(currCommand->cmd, command->valuestring);
    	strcpy(currCommand->nodeID, nodeID->valuestring);
    	if(cJSON_IsNull(data) || !data){
    		strcpy(currCommand->data, "null");
    	}else{
    		strcpy(currCommand->data, data->valuestring);
    	}
    }else{
		char *buffer = "\r\nError: DATA INVALID!\r\n";
		MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
		strcpy(currCommand->cmd, "");
		strcpy(currCommand->nodeID, "");
		strcpy(currCommand->data, "");
    }

    cJSON_Delete(root);

//    if (command && nodeID) {
//        if (strcmp(command->valuestring, "ENA") == 0) {
//            Enable_Node(nodeID->valueint);
//        } else if (strcmp(command->valuestring, "DIS") == 0) {
//            Disable_Node(nodeID->valueint);
//        } else if (strcmp(command->valuestring, "ACT") == 0) {
//            if (data) {
//                Act_On_Node(nodeID->valueint, data->valuestring);
//            }
//        } else if (strcmp(command->valuestring, "STA") == 0) {
//            Get_Node_Status(nodeID->valueint);
//        } else if (strcmp(command->valuestring, "DUR") == 0) {
//            if (data) {
//                Set_Node_Duration(nodeID->valueint, atoi(data->valuestring));
//            }
//        }
//    }

    // Free memory
}

int Validate_JSON_Format(const char *json_msg) {
    cJSON *root = cJSON_Parse(json_msg);
    if (!root) {
        char* buffer = "\r\nError: Invalid JSON format!\r\n";
		MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);
        return 0;
    }
    cJSON_Delete(root);
    return 1;
}



void parse_array_of_objects(const char *json_string) {
    cJSON *json = cJSON_Parse(json_string);
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");

    cJSON *user = NULL;
    cJSON_ArrayForEach(user, users) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(user, "name");
        cJSON *age = cJSON_GetObjectItemCaseSensitive(user, "age");

        if (cJSON_IsString(name) && cJSON_IsNumber(age)) {
        	char buffer[100];
            sprintf(buffer, "Name: %s, Age: %d \r\n", name->valuestring, age->valueint);
            MCAL_UART_SendData(UART1, (uint8_t*)buffer, strlen(buffer), enable);

        }
    }

    cJSON_Delete(json);
}

void Load_Test_Cases(const char* json_content) {
    cJSON *testCases = cJSON_Parse(json_content);
    if (!testCases) {
        printf("Error: JSON Parse Failed\r\n");
        return;
    }

    cJSON *testCase;
    cJSON_ArrayForEach(testCase, testCases) {
        char *jsonString = cJSON_Print(testCase);
        MCAL_UART_SendData(UART1, (uint8_t*) jsonString, strlen(jsonString), enable);
        free(jsonString);
    }

    cJSON_Delete(testCases);
}
