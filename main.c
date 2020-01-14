#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "pit.h"
#include "extra.h"
#include "alarm.h"
#include "rtc.h"


char msg = ' ';

void UART0_IRQHandler(void){
	NVIC_ClearPendingIRQ(UART0_IRQn);
	msg = uart_getChar();
	uart_sendCh(msg);
	
	//TODO: rest of logic
}



int main(void){	
	initLed();
	buzzerInit();
	//pitInit();
	uart_init();
	rtc_init();
	
	uart_sendStr("Hello!");
	
	rtc_write(4);
	
	uint32_t rtc_time = 0;
	while(1){
		
		delay_mc(1000);
		rtc_time = rtc_read();
		uart_sendCh((char)rtc_time);
		if (msg != ' ')
			uart_sendStr(&msg);
	}
}
