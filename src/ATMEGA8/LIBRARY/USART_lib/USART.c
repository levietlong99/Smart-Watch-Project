/*
* 
*   USART.c
* 
*/ 
#include "USART.h"

void USART_Init(unsigned long BAUDRATE)                                         // Hàm khởi tạo kết nối USART
{
    UBRRH = (BAUD_PRESCALE >> 8);                                               // Ghi đè dữ liệu 8 bit cao vào UBRRH
    UBRRL = BAUD_PRESCALE;                                                      // Ghi đè dữ liệu 8  bit  thấp vào UBRRL
    UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);                          // Kích hoạt truyền và nhận USART
    UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);                         // Khung truyền gồm 8 bit dữ liệu và 1 bit stop

}

char USART_RxChar(void)                                                         // Hàm nhận dữ liệu
{
    while (!(UCSRA & (1 << RXC)));                                              // Chờ đến khi  dữ liệu USART được nhận
    return UDR;                                                                 // Trả về dữ liệu
}

void USART_TxChar(char data)                                                    // Hàm truyền 1 ký tự
{
    UDR = data;                                                                 // Ghi dữ liệu cần gửi vào UDR
    while (!(UCSRA & (1<<UDRE)));                                               // Chờ đến khi thanh ghi dữ liệu trống
}

void USART_SendString(char *str)                                                // Hàm gửi string
{
    int i = 0;
    while (str[i] != 0)
    {
        USART_TxChar(str[i]);                                                   // Hàm gửi string cho đến ký tự NULL
        i++;
    }
}