#include "buzzer.h"
#include "extra.h"

#define BIT_SHIFT(x)	(1UL << (x))

void buzzerInit(void){
	SIM->SOPT5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB->PCR[13] = PORT_PCR_MUX(1);
 	
	PTB->PDDR |= BIT_SHIFT(BUZZER_PIN);
	
	PTB->PCOR = BIT_SHIFT(BUZZER_PIN);
	
}

void buzzerTone(uint8_t count, uint32_t length){
	for(int i = 0; i < count; i++){
		PTB->PSOR = BIT_SHIFT(BUZZER_PIN);
		delay_mc(length);
		PTB->PCOR = BIT_SHIFT(BUZZER_PIN);
		delay_mc(length);
	}
	
}
