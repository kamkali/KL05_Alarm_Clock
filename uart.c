#include "MKL05Z4.h"
#include "uart.h"
#include "leds.h"


void uart_init(){
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;      //UART0 dolaczony do zegara
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      //Port A dolaczony do zegara
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2); //Zegar OSCERCLK (8MHz)
		PORTB->PCR[1] = PORT_PCR_MUX(2u);            //PTA1=RX_D
    PORTB->PCR[2] = PORT_PCR_MUX(2u);            //PTA2=TX_D

    UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );      //Blokada nadajnika i o dbiornika
	
    UART0->BDH = UART0_BDH_SBR(0x0);
    UART0->BDL = UART0_BDL_SBR(0x1);        //BR=2000
		UART0->BDH &= ~UART0_BDH_SBNS_MASK;
		UART0->C1 &= ~UART0_C1_M_MASK;
		UART0->C1 &= ~UART0_C1_PE_MASK;
	
    UART0->C4 |= UART0_C4_OSR(0x3);
	
    UART0->C5 |= UART0_C5_BOTHEDGE_MASK;
	
    UART0->C2 |= UART0_C2_RIE_MASK;
		//UART0->C2 |= UART0_C2_TIE_MASK; //DODANE DODATKOWO
    UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);        //Wlacz nadajnik i o dbiornik
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_ClearPendingIRQ(UART0_IRQn);
}

void uart_send(uint8_t data){
	UART0->D = data;
}
