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

#include "GPIO_Driver.h"
#include "EXTI_Driver.h"
#include "TIM.h"
#include "UART.h"
#include "RCC.h"


int main(void)
{
    /* Loop forever */
	for(;;);
}
