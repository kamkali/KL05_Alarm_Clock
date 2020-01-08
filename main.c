#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"

#define SYSCLK	8e3
#define BAUD		9600


int main(void){	
	initLed();
	buzzerInit();
	
	
	while(1){
		ledBlink(RED, 4);
		//buzzerTone(3, 200);
		ledBlink(GREEN, 4);
		ledBlink(BLUE, 4);


	}
}
