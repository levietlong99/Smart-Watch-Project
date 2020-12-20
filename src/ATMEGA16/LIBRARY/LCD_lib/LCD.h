/*
*
* 	LCD.h
*
*/

#ifndef LCD_H_
#define LCD_H_


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_Dir  DDRA           												/* Thanh điều hướng output cho LCD */
#define LCD_Port PORTA          												/* Thanh xuất dữ liệu cho LCD */
#define RS PA0              													/* Chân Register select trên LCD */
#define EN PA1              													/* Chân enable trên LCD */

void LCD_Command( unsigned char cmnd );											// Hàm hiển thị LCD
void LCD_Char(char data);
void LCD_Init (void);
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();

#endif