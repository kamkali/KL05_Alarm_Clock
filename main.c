#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "pit.h"
#include "extra.h"
#include "alarm.h"


uint8_t	msg;


void UART0_IRQHandler(void){
	NVIC_ClearPendingIRQ(UART0_IRQn);
	msg = uart_getchar();
	uart_sendCh(msg);
}

int main(void){	
	initLed();
	buzzerInit();
	//pitInit();
	uart_init();
	while(1){
		delay_mc(1000);
		uart_sendStr("Welcome ");
		uart_sendStr(msg);
		uart_sendStr("\n\r");
	}
}
