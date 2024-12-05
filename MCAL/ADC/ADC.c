/*
 * ADC.c
 * Author: Mohamed Magdi
 */

#ifndef INC_STM32F103C6_ADC_H_
#define INC_STM32F103C6_ADC_H_

//-----------------------------
//Includes
//-----------------------------
#include <STM32F103x8.h>

#include "GPIO_Driver.h"


//-----------------------------
//User type definitions (structures)
//-----------------------------

typedef struct
{
	uint16_t Channel_num								 ;    //specified through ADC_Channels enum
	uint16_t channel_Sampling_rate						 ;	 //Specify Sampling rate of the channel through ADC_SR enum
	void(* Channel_IRQ_callback)(uint16_t channel_data)  ;   //the address callback function of the channel
}Channels;

typedef struct
{
	uint16_t Number_of_channels  ; //specify the number of channels of ADC

	Channels channels[10]  		 ; //specify the number of channels to read

	uint16_t Continous_Mode	     ; //specify the Adc CONT mode
							      //this parameter can be a value of @ref ADC_CONT_Mode

	uint16_t Interrupt			 ; //Enable or disable interrupt
								  //this parameter can be a value of @ref ADC_Interrupt_Mode

	uint16_t Data_Alignment      ; //specify data alignment mode
								  //this parameter can be a value of @ref ADC_Align_Mode

}ADC_Config_t;

//-----------------------------
//Macros Configuration References
//-----------------------------


//@ref ADC_CONT_Mode
#define ADC_CONT_disabled    	0
#define ADC_CONT_enabled     	(uint32_t)(0x2)


//@ref ADC_Interrupt_Mode
#define ADC_Interrupt_disable  	0
#define ADC_Interrupt_enable  	(uint32_t)(1<<5)

//@ref ADC_Align_Mode
#define ADC_Right_alignment 	0
#define ADC_Left_alignment  	(uint32_t)(1<<11)

//ADC Channels numbers
typedef enum
{
	Ch_A0=1 , Ch_A1 , Ch_A2 , Ch_A3 ,Ch_A4 , Ch_A5 , Ch_A6 , Ch_A7 , Ch_B0 , Ch_B1
}ADC_channels;


//ADC sampling rate
typedef enum
{
	SR_1_cycle , SR_7_cycles , SR_13_cycles , SR_28_cycles ,SR_41_cycles, SR_55_cycles , SR_71_cycles , SR_239_cycles
}ADC_SR;


//ADC Idle mode
typedef enum
{
	ADC_full_close,
	ADC_Less_power
}ADC_Idle;

//ADC Channels sequence
typedef enum
{
	Rank0 , Rank1 , Rank2 , Rank3 ,Rank4 , Rank5 , Rank6 , Rank7 , Rank8 , Rank9
}ADC_Ch_Sequence;

/*
* ===============================================
* APIs Supported by "MCAL SPI DRIVER"
* ===============================================
*/
void MCAL_ADC_init(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config);
void MCAL_ADC_Deinit(ADC_TypeDef* ADCx , ADC_Idle Idle_mode);
void MCAL_ADC_pins_set(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config);
void MCAL_ADC_READ(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config ,uint16_t *data);



#endif /* INC_STM32F103C6_ADC_H_ */

