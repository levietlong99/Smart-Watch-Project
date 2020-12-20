/*
*
*   GPS_NEO6M.h
*
*/

#ifndef GPS_NEO6M_H_
#define GPS_NEO6M_H_

#define F_CPU 8000000UL
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"
#include "LCD.h"
#define HOME_LAT 21.03898594
#define HOME_LONG 105.7847605

extern char Lat_Raw[20];
extern char Long_Raw[20];
extern char Time_Buff[15];
extern char Date_Buff[10];
extern char Distance_Buff[10];
extern char Long_Buff[20];																	// String ch?a kinh ?? (th?p phân)
extern  char Lat_Buff[20];																	// String ch?a v? ?? (th?p phân)
extern char GPRMC_Buff[150];															// String l?u tr? chu?i GGA (Chu?i tr? l?i c?a GPS NEO 6M)
extern uint8_t GPRMC_Comma[20]; 															// Chu?i l?u tr? d?u ','
extern char GPRMC_Code[3];																// Mã l?u tr?ng thái c?a string c?n ki?m tra
extern volatile uint16_t GPRMC_Index, Comma_Count;									// Bi?n ??m d?u ph?y, và d?u xu?ng dòng
extern bool IsItGPRMCString; 																// Bi?n bool xác ??nh tính xác th?c c?a chu?i nh?n ???c4

// void convert_time_to_VietNam();																		// Hàm chuyển về múi thời gian Việt nam
void convert_to_degrees(char *raw, char *degrees_buffer);														// Hàm chuyển tọa độ đọc được về độ
void get_gpstime();																		// Hàm lấy thời gian
void get_latitude(uint16_t lat_pointer);																		// Hàm lấy vĩ độ
void get_longitude(uint16_t long_pointer);															 // Hàm lấy kinh độ
void convert_to_degrees(char *raw, char *degrees_buffer);													// Hàm chuyển từ độ về thập phân
void get_date(uint16_t date_pointer);																	// Hàm lấy ngày giờ
void convert_to_date();																					// Hàm chuyển ngày giờ về char
void get_distance(float lat1, float lon1, float lat2, float lon2, char *distance);											              // Hàm lấy khoảng cách
float deg_to_rad(float deg);																	// Hàm chuyển từ độ về radian
bool wait_GPS(bool is_first_wating);
void update_GPS();

#endif