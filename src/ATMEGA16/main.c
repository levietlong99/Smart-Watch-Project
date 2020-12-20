#define F_CPU 8000000UL
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// #include "I2C.h"
#include "LCD.h"
#include "USART.h"
#include "GPS_NEO6M.h"
#include "TIMER1.h"
#define randomm(min, max) min + rand() % (max + 1 - min)
char temp[10];
char pulse[10];
volatile bool is_gps_ready = false;
int main(){
	
    LCD_Init();
	USART_Init(9600);
    _delay_ms(1000);
    sei();
	wait_GPS(true);
	cli();
	update_time(Time_Buff);

	set_timer1();
	sei();

	while(1){
		
		if(is_get_pulse){              // Lấy giá trị nhịp tim ở đây
			
			is_get_pulse = false;
		}
		else if(is_get_temp){
										// Lấy giá trị nhiệt độ ở đây
		}
		else if(is_get_loc){           // Xác định vị trí và đồng bộ ngày giờ tại đây
			
			if(wait_GPS(false)){
				
				is_gps_ready = true;
				update_time(Time_Buff);
				LCD_String_xy(1, 8, Lat_Buff);
				LCD_String_xy(0, 8, Long_Buff);
			}
			else{
				is_gps_ready = false;
			}
			is_get_loc = false;
		}
		else if(is_send_data){			// Gửi dữ liệu cho atmega8 thông qua I2C tại đây
			
			if(is_gps_ready == true){
				
				LCD_String_xy(1, 0, Date_Buff);
				is_gps_ready = false;
			}
			else{
				
				LCD_String_xy(1, 0, "GPS fail");
			}
			is_send_data = false;
		}
	}
}

ISR(USART_RXC_vect){
	
	update_GPS();
}

ISR(TIMER1_OVF_vect){
	
	check_timer();
}