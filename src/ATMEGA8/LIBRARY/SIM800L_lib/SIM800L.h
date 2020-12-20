/*
*
*   SIM800L.h
*
*/
#ifndef SIM800L_H_
#define SIM800L_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include "LCD.h"
#include "USART.h"


#define PHONE_NUMBER "+84382486452"                                             // Số điện thoại để gửi tin nhắn
#define APN_VINAPHONE "\"m3-world\",\"mms\",\"mms\""                            // APN, UserName và PassWord cho sóng Vinaphone                                                          // Tọa độ nhà riêng
#define TCP_UDP "\"TCP\",\"api.thingspeak.com\",\"80\""
#define API "GET https://api.thingspeak.com/update?api_key=TJ698Q9B6IH4EDPH"    // API của chanel cần gửi dữ liệu

void SIM800L_send_sms(char *sms, char *response, char *buff);                   // Hàm gửi tin nhắn
int8_t SIM800L_response(char *response, char *buff);                            // Hàm đợi phản hồi từ Module SIM
void SIM800L_response_display(char *pCRLF, char *response);                     // Hàm phân tích và hiển thị  phản hồi
void SIM800L_THINGSPEAK(int temputure, int pulse, int distance, char *response,\
                        char *buff);                                            // Hàm gửi dữ liệu lên THINGSPEAK                                                                 
void SIM800L_sleep_mode(bool exit_sleep_mode, char *buff, char *response);      // Hàm đưa module về trạng thái tiết kiệm điện false/off-true/
#endif
