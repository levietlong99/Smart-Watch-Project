/*
 * MEGA8.c
 *
 * Created: 05/12/2020 11:37:46 SA
 * Author : VietLong
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SIM800L.h"
#include "USART.h"

struct SIM{
	
	char buff[100];
	char response[100];
	int var;	
};

struct SIM sim;
int main(void)
{

	USART_Init(9600);
	_delay_ms(10000);
	int temp = 30;
	int pulse = 50;
	int distance = 1.12;
	
    while (1) 
    {
		SIM800L_THINGSPEAK(temp, pulse, distance, sim.response, sim.buff);
		_delay_ms(4000);
		SIM800L_sleep_mode(false, sim.buff, sim.response);
		_delay_ms(8000);
		SIM800L_sleep_mode(true, sim.buff, sim.response);
		_delay_ms(8000);
		temp = temp + 1;
		pulse = pulse + 1;
		distance = distance + 1;
    }
}

ISR(USART_RXC_vect){
	
	if(strlen(sim.buff) == 0){
		sim.var = 0;
	}
	sim.buff[sim.var] = UDR;
	sim.var++;
}

