/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire uart routines for polled serial IO
 *
 * Notes:
 */

#ifndef __uart_H__
#define __uart_H__

#include "MKL05Z4.h"


/********************************************************************/

void uartInitialize(void);
void uart_init(void);
void uart_sendCh(uint8_t data);
void uart_sendStr(uint8_t* str);

uint8_t uart_getchar (void);

/********************************************************************/

#endif /* __uart_H__ */
