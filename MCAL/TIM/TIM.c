/*
 * TIM.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Mohamed Magdi
 */

#include "TIM.h"

// Global Variables

uint8_t delayFlag = 1;
uint16_t overflowTimsTIM4 = 0;
uint16_t timerTicksTIM4 = 0;
uint16_t overflowTimsTIM3 = 0;
uint16_t timerTicksTIM3 = 0;
uint16_t overflowTimsTIM1 = 0;
uint16_t timerTicksTIM1 = 0;
volatile uint64_t xMicros = 0;

// Static Functions

static uint32_t counts_time(uint32_t* timx, uint32_t clk){
	uint32_t  x = 0;
	if(timx == TIM4){
		timerTicksTIM4 = TIM4->CNT;
		x = ((timerTicksTIM4 + overflowTimsTIM4 * 64000)) / (clk/1000000); // (time in micro-seconds)
		overflowTimsTIM4 = 0;
		timerTicksTIM4 = 0;
	}else if(timx == TIM3){
		timerTicksTIM3 = TIM3->CNT;
		x = ((timerTicksTIM3 + overflowTimsTIM3 * 64000)) / (clk/1000000); // (time in micro-seconds)
		overflowTimsTIM3 = 0;
		timerTicksTIM3 = 0;
	}
	return x;
}




void delay(uint16_t time, uint8_t unit, uint32_t clk) {
    delayFlag = 1;

    // Enable Clock for Timer 2
    RCC_TIM2_CLk_EN();

    // Disable Timer 2
    TIM2->CR1 &= ~(1 << CEN);

    // Calculate Prescaler and ARR
    uint32_t timerFrequency = clk / (unit == 0 ? 1000 : 1000000); // Frequency in Hz or MHz
    uint32_t ticks = timerFrequency * time;

    TIM2->PSC = (ticks / 65536) - 1; // Set prescaler
    TIM2->ARR = (ticks / (TIM2->PSC + 1)) - 1; // Set auto-reload register

    // Ensure values are within valid range
    if (TIM2->ARR > 0xFFFF) TIM2->ARR = 0xFFFF;
    if (TIM2->PSC > 0xFFFF) TIM2->PSC = 0xFFFF;

    // Enable update event and interrupt
    TIM2->DIER |= (1 << UIE);
    TIM2->EGR |= (1 << 0); // Reinitialize counter
    TIM2->CR1 |= (1 << URS); // Update request source: only overflow/underflow
    TIM2->CR1 |= (1 << CEN); // Enable Timer

    NVIC_IRQ28_TIM2_Enable;

    // Wait for delayFlag to be cleared in ISR
    while (delayFlag);
}


// This function is used with TIM3 and TIM4
uint32_t timeCalc(TIMx_TYPE_DEF* timx, uint32_t clk, uint8_t timerState){
	if(timerState == TIM_START){

		// Enable Clock For Selected Timer
		if(timx == TIM4){
			RCC_TIM4_CLk_EN();
		}else if(timx == TIM3){
			RCC_TIM3_CLk_EN();
		}


		timx -> CR1 &= ~(1<<CEN);
		timx -> CR1 |= (1 << URS);
		timx -> DIER |= (1 << UIE);

		/*
		 * delay = (pre * ticks) / freq.
		 * 8 * 10e-3 = (1 * ticks) / 8*10e6
		 */
		// 1 * 10e-3 = (1 * ticks) / 8*10e6
		timx->ARR = 64000; // Peak value to get delay of 8ms at freq = 8000000hz
		/*
		 * Bit 0  UG: Update generation
		 * This bit can be set by software, it is automatically cleared by hardware.
		 * 0: No action
		 * 1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
		 * counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
		 * the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
		 * value (TIMx_ARR) if DIR=1 (downcounting).
		 */
		timx->PSC = 0;
		timx->EGR |= (1<<0);
		// Enable Timer
		timx->CR1 |= (1 << 0);

		if(timx == TIM3){
			NVIC_IRQ29_TIM3_Enable;
		}else if(timx == TIM4){
			NVIC_IRQ30_TIM4_Enable;
		}

		return 1;
	}else {
		timx -> CR1 &= ~(1 << CEN);
		uint32_t x = counts_time(timx, clk);
		return x;
	}
}


void TIM1CalcMicrosInit(void){
	// Enable Clock For Timer 1
	RCC_TIM1_CLk_EN();

	TIM1 -> CR1 &= ~(1<<CEN);
	TIM1 -> CR1 |= (1 << URS);
	TIM1 -> DIER |= (1 << UIE);

	TIM1->ARR = 64000; // Peak value to get delay of 8ms at freq = 8000000hz

	TIM1->PSC = 0;
	TIM1->EGR |= (1<<0);
	// Enable Timer
	TIM1->CR1 |= (1 << 0);

	NVIC_IRQ25_TIM1_Enable;
}


uint64_t TIM1CalcMicros(uint32_t clk){
	timerTicksTIM1 = TIM1->CNT;
	xMicros = ((timerTicksTIM1 + overflowTimsTIM1 * 64000)) / (clk/1000000); // (time in micro-seconds)
	return xMicros;
}





// Timer ISR() Handler

// IRQ for delay Function with Timer2
void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 0)) { // Check update interrupt flag
        TIM2->SR &= ~(1 << 0); // Clear interrupt flag
        delayFlag = 0;           // Signal delay completion
    }
}



void TIM1_UP_IRQHandler() {
        // Clear the update interrupt flag
        TIM1->SR &= ~(1 << 0);
    	overflowTimsTIM1++;
}


void TIM3_IRQHandler(){
	TIM3->SR &= ~(1 << 0);
	overflowTimsTIM3++;
}
// IRQ for timeCalc function with Timer4.
void TIM4_IRQHandler(){
	TIM4->SR &= ~(1 << 0);
	overflowTimsTIM4++;
}










