/*
 * ADC.h
 * Author: Mohamed Magdi
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "STM32F103x8.h"

typedef struct {
	uint8_t mode; 					// This parameter must be set based on @ref ADC_define_MODE.
	uint8_t channels; 			 	// Sets the ADC channels in the SQR1,2,3
									// this parameter must be set based on @ref ADC_define_SequenceChannels.
	uint8_t conversions; 			// This parameter must be set based on @ref ADC_NumOfConvs_define.
	uint8_t convMode; 				// This parameter specifies the mode of conversion (single or continous).
					  	  	  	  	// This parameter must be set based on @ref ADC_Conv_define_MODE.
	uint8_t dataAlginement; 		// This parameter must be set based on @ref ADC_DataAlign_define.
	uint8_t IRQ_Enable; 			// This parameter must be set based on @ref ADC_IRQ_define.
	void (*P_IRQ_Callback)(void); 	// Set the C function which will be called once the IRQ Happen.

}ADC_Config_t;

//------------------------------------------
// Macros Configuration References
//------------------------------------------

/*
 * @ref ADC_define_MODE
 */
#define ADC_SCAN_MODE					((uint32_t)(1 << 8))

/*
 * @ref ADC_define_SequenceChannels
 */
#define ADC_1_2_SequenceChannels		((0 << 0) | 1 << 5)

/*
 * @ref ADC_NumOfConvs_define
 */
#define ADC_NumOfConvs_1				(000000000)
#define ADC_NumOfConvs_2				(000000001)

/*
 * @ref ADC_Conv_define_MODE
 */
#define ADC_Conv_CONT_MODE				1
#define ADC_Conv_Single_MODE			0

/*
 * @ref ADC_DataAlign_define
 */
#define ADC_DataAlign_Right				0
#define ADC_DataAlign_Left				1

/*
 * @ref ADC_IRQ_define
 */
#define ADC_IRQ_Enable					1
#define ADC_IRQ_Disable					0

// APIs
void ADC_Init(ADC_Config_t* ADCCfg);
void ADC_DeInit();

#endif /* ADC_ADC_H_ */
