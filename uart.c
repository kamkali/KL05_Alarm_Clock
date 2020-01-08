/*
 * File:        uart.c
 * Purpose:     Provide common uart routines for serial IO
 *
 * Notes:       
 *              
 */

#include "MKL05Z4.h"
#include "uart.h"
#include "leds.h"


#define OSCERCLK 8e6
#define OSR		31


// void UART_Init(void)
// {
// 	uint16_t baud_rate = 9600;
// 	uint8_t divisor = OSCERCLK /((OSR + 1) * baud_rate);

// 	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;		//UART0 dolaczony do zegara
// 	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		//Port B dolaczony do zegara
// 	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);	//Zegar OSCERCLK (8MHz)

// 	PORTB->PCR[1] = PORT_PCR_MUX(2u);			//PTA1=RX_D
// 	PORTB->PCR[2] = PORT_PCR_MUX(2u);			//PTA2=TX_D

// 	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika

// 	//UART0->BDH = 0;
// 	//UART0->BDL = divisor;		//BR=9600
// 	UART0->BDH = UART0_BDH_SBR(divisor>>8);
// 	UART0->BDL = UART0_BDH_SBR(divisor);
	
// 	UART0->BDH &= ~UART0_BDH_SBNS_MASK;	//1 bit stopu

// 	//UART0->C1 |= UART0_C1_M_MASK | UART0_C1_PE_MASK;

// 	UART0->C4 |= UART0_C4_OSR(OSR);
// 	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;

// 	UART0->C2 |= UART0_C2_RIE_MASK;	//| UART0_C2_RIE_MASK;
// 	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		//Wlacz nadajnik i o dbiornik

// 	NVIC_EnableIRQ(UART0_IRQn);
// 	NVIC_ClearPendingIRQ(UART0_IRQn);
// }


uint8_t UART_getchar (void)
{
      while (!(UART0->S1 & UART0_S1_RDRF_MASK));
    
      return UART0->D;
}


void UART_putchar (uint8_t ch)
{
      while(!(UART0->S1 & UART0_S1_TDRE_MASK)){			
			}
    
      UART0->D = ch;
    
 }

 
int UART_getchar_present (void)
{
    return (UART0->S1 & UART0_S1_RDRF_MASK);
}
/********************************************************************/


 #define BUS_CLOCK 24e6
 #define XTAL 8e6
 
 uint32_t baud_rate = 9600;
 
 void uartInitialize(void){
 uint32_t divisor;

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;		//UART0 dolaczony do zegara
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		//Port A dolaczony do zegara
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);	//Zegar OSCERCLK (8MHz)
	PORTB->PCR[1] = PORT_PCR_MUX(2u);			//PTA1=RX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2u);			//PTA2=TX_D
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika
	divisor = XTAL/(baud_rate*32);
	UART0->BDH = UART0_BDH_SBR(divisor>>8);
	UART0->BDL = UART0_BDL_SBR(divisor);
	UART0->BDH &= ~UART0_BDH_SBNS_MASK; // 1 bit stopu zamiast dwóch
	UART0->C4 |= UART0_C4_OSR(31);
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;
	UART0->C2 |= UART0_C2_RIE_MASK;//| UART_C2_TIE_MASK;
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		//Wlacz nadajnik i o dbiornik
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
 }
 