#ifndef __PIT_H__
#define __PIT_H__

#include "MKL05Z4.h"

void pitInit(void);

void setPit(uint8_t channel, uint32_t time);

void PIT_IRQHandler(void);

#endif /* __PIT_H__ */
