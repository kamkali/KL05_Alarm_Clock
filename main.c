#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"


int main(void){
		InitLED();
		UART_Init();
	
		while(1){
				BlinkLED();
		}
}
