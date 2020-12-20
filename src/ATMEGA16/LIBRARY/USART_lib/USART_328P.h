/*
*
*	USART_328P.h
*
*/
#ifndef USART_328P_H_
#define USART_328P_H_

#define F_CPU 8000000UL // Clock Speed
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_Init(unsigned long BAUDRATE);
unsigned char USART_RxChar(unsigned char data);
void USART_TxChar(unsigned char data);
void USART_SendString(char *string);

#endif