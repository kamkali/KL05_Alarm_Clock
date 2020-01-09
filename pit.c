#include "pit.h"
#include "alarm.h"

#define PIT_IRQ 22

int state = 0;

void pitInit(void)
{
	SIM->SCGC6 |=  SIM_SCGC6_PIT_MASK;      /* Enable Clock to PIT */ 
	PIT->MCR = 0x00;
	
	NVIC_ClearPendingIRQ(PIT_IRQ); 	/* Clear any pending interrupt */
	NVIC_EnableIRQ(PIT_IRQ);				/* Notify Nested Vector Interrupt Controller */
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;

	
}

void setPit(uint8_t channel, uint32_t time_val){
	PIT->CHANNEL[channel].TFLG = PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[channel].LDVAL = time_val;
	
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
}


void PIT_IRQHandler(void)
{
	if (state == 0){
		state = 1;
		ringTheAlarm();
	}else {
		//state = 0;
		stopTheAlarm();
	}
	PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;	//clear the irq flag
}
