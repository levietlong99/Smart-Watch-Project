/*
*
* 	USART.h
*
*/
#ifndef USART_H_
#define USART_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>

#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)						/* Macro tính baudrate từ F_CPU */

void USART_Init(unsigned long BAUDRATE);
char USART_RxChar();
void USART_TxChar(char data);
void USART_SendString(char *str);
void USART_Stop(bool is_it_stop);

#endif