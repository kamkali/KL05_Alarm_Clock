#include "MKL05Z4.h"
#include "uart.h"
#include "leds.h"


void uart_init(){
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;      //UART0 dolaczony do zegara
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      //Port A dolaczony do zegara
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2); //Zegar OSCERCLK ~32kHz
		PORTB->PCR[1] = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);            //PTB1=RX_D
    PORTB->PCR[2] = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);            //PTB2=TX_D

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
		NVIC_SetPriority(UART0_IRQn, 0);
    NVIC_ClearPendingIRQ(UART0_IRQn);
}


void uart_sendStr(char* str){
	uint16_t i=0;
	while(str[i] != 0){
		while( !(UART0->S1&UART0_S1_TDRE_MASK) && !(UART0->S1&UART0_S1_TC_MASK));
		UART0->D = str[i];
		i++;
	} 
}

void uart_sendCh(uint8_t data){
	while(!(UART0->S1 & UART0_S1_TDRE_MASK) && !(UART0->S1&UART0_S1_TC_MASK));
	UART0->D = data;
}


uint8_t uart_getChar(void){
  while (!(UART0->S1 & UART0_S1_RDRF_MASK));
    
  return UART0->D;
}
