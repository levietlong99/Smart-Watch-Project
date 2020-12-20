#ifndef TIMER1_H_
#define TIMER1_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LCD.h"

volatile uint8_t timer_count;
uint16_t timer_start;
extern volatile bool is_get_temp;
extern volatile bool is_get_pulse;
extern volatile bool is_send_data;
extern volatile bool is_get_loc;
extern volatile char time_display[20];

volatile int hours;
volatile int minutes;
volatile int seconds;

void set_timer1();
void check_timer();
void update_time(char *timer_raw);

#endif