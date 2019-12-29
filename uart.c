#include "uart.h"

#define OSCERCLK 8e6
#define OSR		31


void UART_Init(void)
{
	uint16_t baud_rate = 9600;
	uint8_t divisor = OSCERCLK /((OSR + 1) * baud_rate);

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;		//UART0 dolaczony do zegara
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		//Port A dolaczony do zegara
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);	//Zegar OSCERCLK (8MHz)

	PORTA->PCR[1] = PORT_PCR_MUX(2u);			//PTA1=RX_D
	PORTA->PCR[2] = PORT_PCR_MUX(2u);			//PTA2=TX_D

	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika

	UART0->BDH = 0;
	UART0->BDL = divisor;		//BR=9600
	UART0->BDH &= ~UART0_BDH_SBNS_MASK;	//1 bit stopu

	UART0->C1 |= UART0_C1_M_MASK | UART0_C1_PE_MASK;

	UART0->C4 |= UART0_C4_OSR(OSR);
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;

	UART0->C2 |= UART0_C2_RIE_MASK;	//| UART0_C2_RIE_MASK;
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		//Wlacz nadajnik i o dbiornik

	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
}
