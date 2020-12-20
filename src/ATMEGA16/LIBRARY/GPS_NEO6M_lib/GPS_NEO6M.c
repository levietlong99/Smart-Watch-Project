/*
*
*   GPS_NEO6M.c
*
*/

#include "GPS_NEO6M.h"

char Lat_Raw[20];
char Long_Raw[20];
char Time_Buff[20];
char Date_Buff[20];
char Distance_Buff[20];
char Long_Buff[20];                                                                 // String ch?a kinh ?? (th?p phân)
char Lat_Buff[20];                                                                  // String ch?a v? ?? (th?p phân)
char GPRMC_Buff[150];                                                           // String l?u tr? chu?i GGA (Chu?i tr? l?i c?a GPS NEO 6M)
uint8_t GPRMC_Comma[20];                                                            // Chu?i l?u tr? d?u ','
char GPRMC_Code[3];                                                             // Mã l?u tr?ng thái c?a string c?n ki?m tra
volatile uint16_t GPRMC_Index = 0;                                  // Bi?n ??m d?u ph?y, và d?u xu?ng dòng
volatile uint16_t Comma_Count = 0;
bool IsItGPRMCString = false;                           // Bi?n bool xác ??nh tính xác th?c c?a chu?i nh?n ???c

void get_gpstime(){                        // Các hàm bên dưới đều tắt ngắt để tránh bị gián đoạn

    cli();                                                                      // Tắt ngắt ngoài trong quá trình dò để tránh bị gián đoạn
    uint8_t time_index = 0;                                                     // Khởi tạo biến chạy cho Time_Buff
    uint8_t index = 0;

    while(GPRMC_Buff[index] != ','  && index <= 6){                               // Lấy từng ký tự cho vào chuỗi chứa thời gian cho đến khi gặp dấu phẩy
        
        Time_Buff[time_index] = GPRMC_Buff[index];
        time_index++;
        index++;
    }
    sei();                                                                      // Tiếp tục kích hoạt ngắt
}

void get_latitude(uint16_t lat_pointer){                        // Hàm lấy giá trị vĩ độ

    cli();

    uint8_t lat_index = 0;
    uint8_t index = lat_pointer + 1;                                            // Vị trí tiếp theo trỏ tới kế thừa từ vị trí tham chiếu
    
    while(GPRMC_Buff[index] != ','){                                          // Trích xuất ký tự  từ GPRMC_Buff sang Lat_Raw đến khi gặp ','

        Lat_Raw[lat_index] = GPRMC_Buff[index];
        lat_index++;
        index++;
    }
    
    Lat_Raw[lat_index++] = GPRMC_Buff[index++];                       // Lấy 2 ký tự nhận diện
    Lat_Raw[lat_index]= GPRMC_Buff[index];                            // Tăng index đến string tiếp theo

    convert_to_degrees(Lat_Raw, Lat_Buff);                        // Thực hiện chuyển vĩ  độ sang giá trị thập phân
    sei();
}

void get_longitude(uint16_t long_pointer){                                              // Hàm lấy giá trị kinh độ

    cli();
    
    uint8_t long_index = 0;                                                     // Khởi tạo biến chạy cho chuỗi kinh độ và tín hiệu GPRMC
    uint8_t index = long_pointer + 1;
    
    while(GPRMC_Buff[index] != ','){                                          // Lưu giá trị vào chuỗi kinh độ cho tới khi gặp dấu ','

        Long_Raw[long_index]= GPRMC_Buff[index];
        long_index++;
        index++;
    }
    
    Long_Raw[long_index++] = GPRMC_Buff[index++];
    Long_Raw[long_index]   = GPRMC_Buff[index];

    convert_to_degrees(Long_Raw, Long_Buff);                       // Chuyển kinh độ sang giá trị thập phân
    sei();
}

void get_date(uint16_t date_pointer){

    cli();
    uint8_t date_index = 0;                                                     // Khởi tạo con trỏ cho chuỗi ngày tháng và tín hiệu GPRMC
    uint8_t index = date_pointer + 1;

    while(GPRMC_Buff[index] != ','){                                          // Lưu giá trị vào chuỗi kinh độ cho tới khi gặp dấu ',' tiếp theo
        Date_Buff[date_index]= GPRMC_Buff[index];
        date_index++;
        index++;
    }
    convert_to_date(Date_Buff);
    sei();
}


void convert_to_degrees(char *raw, char *degrees_buffer){                       // Hàm chuyển giá trị kinh độ, vĩ độ từ mã NMEA (hàng hải)
                                                                                //https://www.siretta.com/2019/01/gps-latitude-longitude-conversion-guide/
    double value;
    float decimal_value;
    float temp;
    int32_t degrees;
    float position;
    
    value = atof(raw);                                                          // Hàm chuyển từ string raw về value (float)
    decimal_value = (value /  100);                                             // Lấy phần thập phân
    degrees = (int)(decimal_value);                                             // Lấy phần độ
    temp = (decimal_value - (int)decimal_value) / 0.6;                          // Giá trị phút thập phân

    position = (float)degrees + temp;                                           // Gộp độ thập phân và phút thập phân
    dtostrf(position, 6, 4, degrees_buffer);                                    // Chuyển position về degrees_buffer, với format 2.4
}

void convert_to_date(){                                        // Hàm chuyển về buffer ngày giở

    char date[6];
    for(int run = 0; run < 6; run++){

        date[run] = *(Date_Buff + run);
    }
    sprintf(Date_Buff, "%c%c/%c%c/%c%c", date[0], date[1], date[2], date[3],\
            date[4], date[5]);
}

void get_distance(float lat1, float lon1, float lat2, float lon2,\
                char *distance){                                                // Tính khoảng cách dựa trên công thức Haver Sine
                                                                                // https://nhanweb.com/how-to-tinh-khoang-cach-giua-2-diem-su-dung-kinh-do-vi-do-trong-php.html
                                                                        
    float radius = 6371;                                                        // Bán kính của trái đất
    float dLat = deg_to_rad(lat2-lat1);                                         // Thực hiện công thức
    float dLon = deg_to_rad(lon2-lon1);
    float a = sin(dLat/2) * sin(dLat/2) + cos(deg_to_rad(lat1))\
                    * cos(deg_to_rad(lat2)) * sin(dLon/2) * sin(dLon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    float d = radius * c;                                                       // Đơn vị khoảng cách là km
    dtostrf(d, 6, 4, distance);                                                 // Chuyển khoảng cách ra mảng char 2.4
}

float deg_to_rad(float deg){                                                    // Hàm chuyển giá từ tọa độ thập phân về tọa độ radian

    return deg * (M_PI/180);
}

bool wait_GPS(bool is_first_wating){
    
        
    memset(GPRMC_Buff, 0 , strlen(GPRMC_Buff));
    memset(Long_Buff, 0, strlen(Long_Buff));
    memset(Lat_Buff, 0, strlen(Lat_Buff));
    memset(Long_Raw, 0, strlen(Long_Raw));
    memset(Lat_Raw, 0, strlen(Lat_Raw));
    memset(GPRMC_Comma, 0, 20);
    memset(Time_Buff, 0, strlen(Time_Buff));
    memset(Date_Buff, 0, strlen(Date_Buff));
    memset(Distance_Buff, 0, strlen(Distance_Buff));
    if(is_first_wating == true){
        while(1){
            
            _delay_ms(1000);
            get_gpstime();
            get_latitude(GPRMC_Comma[1]); // Lấy vĩ độ
            get_longitude(GPRMC_Comma[3]); // Lấy kinh độ
            get_date(GPRMC_Comma[7]);
            get_distance(atof(Lat_Buff), atof(Long_Buff), HOME_LAT, HOME_LONG, Distance_Buff);
            if(atof(Lat_Buff) != 0 && atof(Long_Buff) != 0){
                
                USART_Stop(true);
                return true;
            }
        }
    }
    else{
        
        USART_Stop(false);
        _delay_ms(3000);
        get_gpstime();
        get_latitude(GPRMC_Comma[1]); // Lấy vĩ độ
        get_longitude(GPRMC_Comma[3]); // Lấy kinh độ
        get_date(GPRMC_Comma[7]);
        get_distance(atof(Lat_Buff), atof(Long_Buff), HOME_LAT, HOME_LONG, Distance_Buff);
        if(atof(Lat_Buff) != 0 && atof(Long_Buff) != 0 && atof(Distance_Buff) < 10){
            USART_Stop(true);
            return true;
        }
        else{
            USART_Stop(true);
            return false;
        }
    }
}

void update_GPS(){
    
    cli();
    char received_char = UDR;
    if(received_char =='$'){

        GPRMC_Index = 0;
        Comma_Count = 0;
        IsItGPRMCString = false;
    }

    else if(IsItGPRMCString == true){
        if(received_char == ',' ){

            GPRMC_Comma[Comma_Count++] = GPRMC_Index;
        }
        GPRMC_Buff[GPRMC_Index++] = received_char;
    }

    else if(GPRMC_Code[0] == 'R' && GPRMC_Code[1] == 'M' && GPRMC_Code[2] == 'C'){

        IsItGPRMCString = true;
        GPRMC_Code[0] = 0;
        GPRMC_Code[1] = 0;
        GPRMC_Code[2] = 0;
    }
    else{

        GPRMC_Code[0] = GPRMC_Code[1];
        GPRMC_Code[1] = GPRMC_Code[2];
        GPRMC_Code[2] = received_char;
    }
    sei();
}