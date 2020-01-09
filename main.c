#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "pit.h"
#include "extra.h"
#include "alarm.h"


volatile uint8_t	msg = ' ';


void UART0_IRQHandler(void){
	NVIC_ClearPendingIRQ(UART0_IRQn);
	msg = UART0->D;
	uart_sendCh(msg);
	
	/*
	if (UART0->S1&UART0_S1_RDRF_MASK)
	{
		msg = UART0->D;
		if (!(UART0->S1&UART0_S1_TDRE_MASK) && !(UART0->S1&UART0_S1_TC_MASK)) {
			UART0->D = msg;
		}
	}
	*/
}

int main(void){	
	initLed();
	buzzerInit();
	//pitInit();
	uart_init();
	
	while(1){
		
		delay_mc(1000);
		uart_sendCh(msg + 48);	// constantly sends ASCII 13 (CR - Carrige return)

		/*
		uart_sendStr("Welcome ");
		uart_sendStr("\n\r");
		*/
	}
}
