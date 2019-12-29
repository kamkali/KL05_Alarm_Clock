#include "leds.h"


const uint32_t MaskLED[] = {1UL << 8, 1UL << 9, 1UL << 10};

void InitLED(void){

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[8] = PORT_PCR_MUX(1UL);
	PORTB->PCR[9] = PORT_PCR_MUX(1UL);
	PORTB->PCR[10] = PORT_PCR_MUX(1UL);
	PTB->PDDR |= MaskLED[0];
  PTB->PDDR |= MaskLED[1];
  PTB->PDDR |= MaskLED[2];
}

void BlinkLED(void){
	uint32_t i = 0;
	for (int i = 0; i < 3; i++){
		PTB->PTOR = MaskLED[i];
		for(int j = 0 ; j < 3000000; j++){};
		PTB->PSOR = MaskLED[i];
		for(int j = 0 ; j < 6000000; j++){};
	}
}
