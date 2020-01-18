#ifndef __uart_H__
#define __uart_H__

#include "MKL05Z4.h"

void uart_init(void);
void uart_sendCh(uint8_t data);
void uart_sendStr(char* str);

uint8_t uart_getChar (void);

#endif /* __uart_H__ */
