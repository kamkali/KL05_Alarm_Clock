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



//void uart0_init (int sysclk, int baud);
// void UART_Init(void);
void uartInitialize(void);
// Piotr
void uart_init(void);
void uart_send(uint8_t data);

uint8_t UART_getchar (void);
void UART_putchar (uint8_t ch);
int UART_getchar_present (void);

/********************************************************************/

#endif /* __uart_H__ */
