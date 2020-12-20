/*
*
*	I2C.h
*
*/
#ifndef I2C_H_
#define I2C_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>

#define ATMEGA8 0x20 															// Địa chỉ atmega8 - slave
#define SCL_CLK 320000L 														// Tần số SCL muốn sử dụng cho giao tiếp TWI
#define WRITE 0																	// Chế độ đọc và ghi của MASTER
#define READ 1

#define START_SENT 0x08															// Một vài byte trạng thái của kết nối I2C
#define RSTART_SENT 0x10
#define SLA_W_SENT_ACK_REC 0x18
#define DATA_SENT_ACK_REC 0x28

#define REC_ADD_ACK 0x60														// Các byte trạng tái của slave
#define REC_DATA_ACK 0x80
#define RS_CONDITION 0xA0

#define WAITE_TWINT while(!(TWCR & (1 << TWINT)))								// Macro sử đụng dể chờ bit TWINT set
#define BIT_RATE(TWSR) (F_CPU/SCL_CLK - 16) / (2 * pow(4,\						// Macro tính bitrate cho giao tiếp
		(TWSR &= ((1 << TWPS0) | (1 << TWPS1)))))

void I2C_Init(); 																// Khởi tạo truyền thông TWI
void I2C_Start_Condition(); 													// Gửi tín hiệu khởi động đường truyền
void I2C_Restart_Condition(); 													// Gửi tín hiệu kích hoạt lại đường truyền
void I2C_Stop_Condition(); 														// Hàm gửi tín hiệu  dừng
void I2C_Address_Package(uint8_t address); 										// Gửi gói địa chỉ
void I2C_Data_Package(char data, uint8_t address); 								// Gửi gói dữ liệu
void I2C_Send_String(char *string, uint8_t address); 							// Gửi string đến SLAVE
char *I2C_Master_Receive();
char *I2C_Slave_Receive5();

extern char rec_string[50];
uint8_t semicolon = 0;

#endif