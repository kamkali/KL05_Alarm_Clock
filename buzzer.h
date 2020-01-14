#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "MKL05Z4.h"

#define BUZZER_PIN 13		//portB 13


void buzzerInit(void);

void buzzerTone(uint8_t count, uint32_t length);


#endif /* __BUZZER_H__ */
