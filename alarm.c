#include "alarm.h"

uint8_t ring = 1;

void ringTheAlarm(void){
	ring = 1;
	while(ring){
		ledOn(RED);
		buzzerTone(1, 100);
		ledOff();
		buzzerTone(1, 100);
		
		if (ring == 0)
			break;
	}
}

void stopTheAlarm(void){
	ring = 0;
}
