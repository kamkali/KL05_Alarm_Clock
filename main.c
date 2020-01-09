#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "pit.h"
#include "extra.h"
#include "alarm.h"

#define SYSCLK	8e3
#define BAUD		9600

#define PIT_CHANNEL_1	1

void UART0_IRQHandler(void){
	static uint8_t	elo;
	elo = UART0->D;
	UART0->D = elo;
}


int main(void){	
	initLed();
	//buzzerInit();
	//pitInit();
	uart_init();
	
	while(1){


		/*
		uint8_t set_pit_t = 1;
		ledBlink(BLUE, 2);
		if (set_pit_t){
			setPit(PIT_CHANNEL_1, 99999999);
			set_pit_t = 0;
		}
		*/
		//buzzerTone(3, 200);
		//ledBlink(GREEN, 4);
		//ledBlink(BLUE, 4);


	}
}
