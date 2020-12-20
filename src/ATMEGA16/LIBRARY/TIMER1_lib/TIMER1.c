
#include "TIMER1.h"

uint16_t timer_start = 0;
volatile bool is_get_temp = false;
volatile bool is_get_pulse = false;
volatile bool is_send_data = false;
volatile bool is_get_loc = false;

volatile uint8_t timer_count = 0;
volatile int hours = 0;
volatile int minutes = 0;
volatile int seconds = 0;
volatile char time_display[20];

void set_timer1(){

	
	TCCR1B = (1 << CS12);
	TIMSK |= (1 << TOIE1);
	TCNT1 = 34286;
}

void update_time(char *timer_raw){

	cli();
	char temp1[3], temp2[3], temp3[3];
	memset(temp1, 0, 3);
	memset(temp2, 0, 3);
	memset(temp3, 0, 3);
	temp1[0] =  timer_raw[0];
	temp1[1] =  timer_raw[1];
	temp2[0] =  timer_raw[2];
	temp2[1] =  timer_raw[3];
	temp3[0] =  timer_raw[4];
	temp3[1] =  timer_raw[5];
	hours = atoi(temp1);
	minutes = atoi(temp2);
	seconds = atoi(temp3);
	hours = hours + 7;
	if(hours >= 24){

		hours = hours - 24;
	}
	sei();
}
void check_timer(){

	cli();
	TCNT1 = 34286;
	if(timer_count == 60){						// G?i d? li?u c?m bi?n và ??nh v? thu th?p ???c cho atmega8
		is_send_data = true;
		timer_count = 0;
	}
	else if(timer_count % 5 == 0){				// Láy giá tr? c?m bi?n nhi?t ?? (??ng th?i update màn hình)
		
		is_get_temp = true;
	}
	else if((timer_count - 1) % 5 == 0){
		
		is_get_pulse = true;
	}
	else if(timer_count == 57){					// L?y giá tr? ??nh v? trong 2 giây (??ng th?i ??ng b? gi?)

		is_get_loc = true;
	}
	
	if(seconds == 59){

		seconds = 0;
		if(minutes == 59){

			minutes = 0;
			if(hours == 23){

				hours = 0;
			}
			else{

				hours++;
			}
		}
		else{

			minutes++;
		}
	}
	else{

		seconds++;
	}
	sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
	timer_count++;
	LCD_String_xy(0, 0, time_display);
	sei();
}