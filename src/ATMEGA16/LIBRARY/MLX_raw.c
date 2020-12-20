// Atmega48P
// Copyright 2011 Dexter Industries. http://www.dexterindustries.com/howto
// Chip: Atmega48P
// Operations: This chip reads the MLX90614 temperature sensor, calculates out the temperature value, and sends it out
// on the serial line.
//
// Important to note two critical changes that must be made to adapt to other chips:
// In i2cmaster.S:
// – Match the pin and ports at the top. This example matches to PIN B 1, PIN B 2.
// – Match the i2c_delay_T2 to the clock. This example matches to 16 Mhz.
// http://www.avr-asm-tutorial.net/avr_en/beginner/JUMP.html
//
// Sensor Notes: needs at least a 2.2k resistor. Max operation is 100 kHz.
// Based on the I2C Bitbang protocol built by Fleury. http://jump.to/fleury
//
// Created: 4/21/2011
// Author: John
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
#define F_CPU 16000000UL // Clock speed for UART // We’re using an external 16MHz Clock
#define FOSC 16000000UL // Clock Speed for UART // We’re using an external 16MHz Clock
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define SCL_CLOCK 100000L // JDC added.

#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"

char ret1 = 0;
char ret2 = 0;
char ret3 = 0;

long celsius = 0;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// Initializes USART.
// Very well.
void USART_Init( unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

// TRANSMIT
// This code sends an integer over the line.
void USART_Transmit(int data)
{
// If you look in the data sheet, there is not any register named UCSRnB.
// The little ‘n’ means that you substitue that with a number for the exact
// register that you want to use such as UCSR0B or UCSR1B.
/* Wait for empty transmit buffer */
while ( !(UCSR0A & (1<<UDRE0))); // Using register 0

/* Put data into buffer, sends the data */
UDR0 = data; // Using register 0
}

// WAIT: Take a breather.
void Wait()
{
uint8_t i=0;
for(;i<5;i++)
_delay_ms(10);
//_delay_loop_1(0);
}

// BLINK This will serve as a debugging tool
// just to let us know we’re still alive.
void blink_on(){
//Set PC0=High(+5v)
PORTC|=0b00111111;
Wait();
}

void blink_off(){
//Set PC0=Low(GND)
PORTC&=0b11000000;
Wait();
}

void blink(){
blink_on();
blink_off();
}

void i2c_refresh(){

int i2c_slave_address = 0x5A<<1; // Shifted address. 0x5A is the address, shifted is B4.

i2c_start(i2c_slave_address+I2C_WRITE); // Set device address and read mode
i2c_write(0x07); // Read the temp. 0x06 = ambient Temp. 0x07 = TObj1, 0x08 = TObj2.

i2c_rep_start(i2c_slave_address+I2C_READ); // Kick it.
ret1 = i2c_readAck(); // Read the first bit.
ret2 = i2c_readAck(); // Read the second bit.
ret3 = i2c_readNak(); // This is a NACK because it’s the last byte. We don’t care much about this.
i2c_stop();

// Process the data.

double tempFactor = 0.02; // 0.02 degrees per bit
double tempData = 0x0000; // zero out the data
int frac; // data past the decimal point

// This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
tempData = (double)(((ret2 & 0x007F) << 8) + ret1);
tempData = (tempData * tempFactor)-0.01;

celsius = ((tempData – 273.15)*100); // Final value of celsius is multipled by 100 so we don’t have decimals.

}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{

i2c_init(); // Initialize I2C library
USART_Init(MYUBRR); // Initialize USART

while(1)
{
USART_Transmit(0xFF); // Just markers for the serial stream.
USART_Transmit(celcius); // Transmit the value of celsius.
i2c_refresh(); // Refresh the data from the MLX90614
blink(); // Blink to let us know the microcontroller is alive
}
}