/*
*
*   SIM800L.c
*
*/
#include "SIM800L.h"

void SIM800L_send_sms(char *sms, char *response, char *buff){                   // Hàm gửi tin nhắn từ module SIM

    char sms_buffer[50];
    while(1){
        
        USART_SendString("AT\r");                                               // Kiểm tra trạng thái Module SIM
        int8_t status = SIM800L_response(response, buff);

        if(status == 1 && strcmp(response, "OK") == 0){                         // Phân tích phản hồi trả về từ module SIM
            break;                                                              // Nếu không đúng, tiến hành lại
        }
        continue;

    }
    _delay_ms(1000);                                                            // Các quãng nghỉ cho module SIM
    
    while(1){
        
        USART_SendString("AT+CMGF=1\r");                                        // Chuyển sang chế độ nhập text cho module SIM
        int8_t status = SIM800L_response(response, buff);

        if(status == 1 && strcmp(response, "OK") == 0){
            break;
        }
        continue;
    }
    
    _delay_ms(1000);
    while(1){
        
        sprintf(sms_buffer, "AT+CMGS=\"%s\"\r", PHONE_NUMBER);
        USART_SendString(sms_buffer);                                           // Gửi số điện thoại cần gửi tin nhắn
        int8_t status = SIM800L_response(response, buff);

        if(status == 1 && strcmp(response, ">") == 0){                          // Bắt đầu gửi sms khi char(26) đã được nhận
            
            USART_SendString(sms);
            USART_TxChar(26);
            break;
        }
        continue;
    }
}

int8_t SIM800L_response(char *response, char *buff){                            // Hàm chờ trả lời từ Module SIM
    
    memset(buff, 0, strlen(buff));                                              // Tiến hành xóa các buff phản hồi trước khi đợi phản hồi mới
    memset(response, 0, strlen(response));
    unsigned int timeout = 0;
    char CRLF_buff[2];
    int Response_Length = 0;
    while(1)
    {
        if(timeout >= 7000) return -1;                                         // Nếu thời gian chờ lớn hơn 5000, tự động reset lại
        Response_Length = strlen(buff);
        if(Response_Length)
        {
            _delay_ms(2);
            timeout++;
            if(Response_Length == strlen(buff))                                 // Kích thước buff không đổi sau 2s (USART hoàn thành)
            {
                for(int run = 0; run < Response_Length; run++)                  // Kiểm tra liên tục các ký tự CR - LF khi module SIM phản hồi
                {
                    CRLF_buff[0] = CRLF_buff[1];
                    CRLF_buff[1] = buff[run];
                    if(strncmp(CRLF_buff, "\r\n", 2) == 0)
                    {
                        _delay_ms(1000);
                        SIM800L_response_display(buff + run,\
                        response);                                         // Tiến hành gọi hàm phân tích khi đã phát hiện \r\n
                        return 1;                                           // Trả về thành công
                    }
                }
            }
        }
        _delay_ms(1);
        timeout++;
    }
}

void SIM800L_response_display(char *pCRLF, char *response){                     // Hàm hiển thị và phân tích phản hồi từ Module SIM

    //int lcd_pointer = 0;
    int run = 1;
    //LCD_Command(0xc0);                                                        // Di chuyển con trỏ xuống dòng bên dưới
    while(*(pCRLF + run) != '\r')                                               // Hiển thị response cho đến khi gặp carriage turn - char(13)
    {
        *(response + run - 1) = *(pCRLF + run);                                 // Tách response từ module SIM
        //LCD_Char(*(pCRLF + run));                                             // Hiển thị response của module SIM
        if(*(response) == '>'){                                                 // Trong trường hợp gửi tin nhắn hoặc đường dẫn
            break;
        }
        run++;
    }
}

void SIM800L_THINGSPEAK(int temputure, int pulse, int distance, char *response,\
char *buff){                                            // Hàm gửi dữ liệu lên THINGSPEAK

    char TP_buff[100];
    bool SEND_SUCCESS = false;
    int8_t status = 0;
    int relapse = 0;
    //     static int fail_var;
    //     char debug[50];

    START_AGAIN:                                                                // Quay lại khi gửi không thành công
    
    while(1){
        
        _delay_ms(4000);
        USART_SendString("AT+CIPSHUT\r");                                   // Tắt kết nối UDP/ICP
        status = SIM800L_response(response, buff);
        if(status == 1 && strcmp(response, "SHUT OK") == 0){                // So sánh phản hồi để lấy chuẩn (datasheet)
            break;
        }
    }
    
    if(SEND_SUCCESS == true){                                               // Nếu gửi thành công thoát khỏi hàm
        return;
    }
    if(relapse == 10){
        SIM800L_sleep_mode(false, buff, response);                          // Reset module 
        SIM800L_sleep_mode(true, buff, response);
        goto START_AGAIN;
    }
    relapse++;
    
    sprintf(TP_buff, "AT+CSTT=%s\r", APN_VINAPHONE);                        // Kết nối với APN của vinaphone
    _delay_ms(1000);
    USART_SendString(TP_buff);
    status = SIM800L_response(response, buff);                              // Lấy phản hồi tức thì ngay khi vừa gửi AT - command
    if(status != 1 || strcmp(response, "OK") != 0){
        
        SEND_SUCCESS = false;
        goto START_AGAIN;
    }
    
    _delay_ms(1000);
    USART_SendString("AT+CIICR\r");                                         // Mở kết nối mạng không dây wireless
    status = SIM800L_response(response, buff);
    if(status != 1 || strcmp(response, "OK") != 0){
        SEND_SUCCESS = false;
        goto START_AGAIN;
    }
    
    _delay_ms(1000);
    USART_SendString("AT+CIFSR\r");                                         // Lấy địa chỉ IP đang kết nối
    status = SIM800L_response(response, buff);
    if(status != 1 || strcmp(response, "ERROR") == 0){
        SEND_SUCCESS = false;
        goto START_AGAIN;
    }

    _delay_ms(1000);
    sprintf(TP_buff, "AT+CIPSTART=%s\r", TCP_UDP);                          // Bắt đầu kết nối đến URL định trước
    USART_SendString(TP_buff);
    status = SIM800L_response(response, buff);
    if(status != 1 || strcmp(response, "OK") != 0){
        
        SEND_SUCCESS = false;
        goto START_AGAIN;
    }
    _delay_ms(3000);
    sprintf(TP_buff, "%s&field2=%d&field4=%d&field1=%d\r\n\r\n",\
    API, temputure, pulse, distance);                               // Gửi dữ liệu đến 3 field của  THINGSPEAK
    
    USART_SendString("AT+CIPSEND\r");                                       // Truyền tín hiệu chờ để gửi dữ liệu
    status = SIM800L_response(response, buff);
    if(status == 1 && strcmp(response, ">") == 0){                          // Ký tự ">" là phản hồi của module khi lấy string
        
        _delay_ms(1000);
        USART_SendString(TP_buff);
        _delay_ms(2000);
        USART_TxChar(26);                                                   // Gửi ký tự Ctrl - Z để thông báo kết thúc truyền
        SEND_SUCCESS = true;
    }
    else{
        
        SEND_SUCCESS = false;
    }
    goto START_AGAIN;                                                       // Quay trở lại vòng lặp
}

void SIM800L_sleep_mode(bool exit_sleep_mode, char *buff, char *response){      // Hàm đưa module SIM vào trạng thái ngủ nhằm tiết kiệm năng lượng

    if(exit_sleep_mode == false){

        USART_SendString("AT+CFUN=0\r");
        _delay_ms(6000);
    }
    if(exit_sleep_mode == true){

        USART_SendString("AT+CFUN=1\r");
        _delay_ms(1000);
    }
}