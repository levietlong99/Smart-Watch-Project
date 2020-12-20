/*
*
*   USART_328P.c
*
*/
#include "USART_328P.h"
void USART_Init(unsigned long BAUDRATE){

    UBRR0H = (unsigned char)(BAUD_PRESCALE>>8);
    UBRR0L = (unsigned char)BAUD_PRESCALE;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0);
    UCSR0C = (1<<UCSZ00) | (1 << UCSZ01);
}

unsigned char USART_RxChar(unsigned char data){

    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void USART_TxChar(unsigned char data){

    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_SendString(char *string){

    int run = 0;
    while(string[run] != 0){
        USART_TxChar(string[run]);
        run++;
    }
}