/*
*
*   I2C.c
*
*/
#include "I2C.h"

volatile bool new_data_rec = false;
uint8_t semicolon = 0;

void I2C_Init(){
    
    TWSR = 0x00;
    TWBR = BIT_RATE(TWSR);
}

void I2C_Start_Condition(){                                                     // Gửi tín hiệu START
    
    I2C_Init();
    uint8_t status;
    while(1){

        TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);                       // Kích hoạt đường truyền, gửi bit start
        WAITE_TWINT;
        status = TWSR & 0xF8;                                                   // Kiểm tra 5 bit có trọng số cao trong TWSR
        if(status != START_SENT) continue;                                      // Gửi lại nếu tín hiệu ko gửi thành công
        break;
    }
}

void  I2C_Restart_Condition(){                                                  // Gửi tín  hiệu lặp lại

    uint8_t status;
    while(1){

        TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
        WAITE_TWINT;
        status = TWSR &= 0xF8;
        if(status != RSTART_SENT) continue;                                     // Gửi lại nếu tín hiệu ko gửi thành công
        break;
    }
}

void I2C_Stop_Condition(){                                                      // Gửi tín hiệu dừng
    
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while(TWCR & (1 << TWSTO));                                                 // Chờ đến khi bit STOP clear
    TWCR &= ~(1 << TWSTA);
}

void I2C_Address_Package(uint8_t address){                                      // Gửi gói địa chỉ

    uint8_t status;
    while(1){

        TWDR = address + WRITE;                                                 // Chế độ ghi
        TWCR = (1 << TWEN) | (1 << TWINT);
        WAITE_TWINT;
        status = TWSR &= 0xF8;
        if(status != SLA_W_SENT_ACK_REC){                                       //Dừng và mở trở lại đường truyền nếu add ko gửi T/Công
            I2C_Stop_Condition();
            I2C_Start_Condition();
            continue;
        }
        break;
    }
}

void I2C_Data_Package(char data, uint8_t address){

    uint8_t status;
    while(1){

        TWDR = data;                                                            // Ghi dữ liệu cần gửi vào TWDR
        TWCR |= (1 << TWEN) | (1 << TWINT);                                     // Gửi dữ liệu
        WAITE_TWINT;
        status = TWSR &= 0xF8;
        if(status != DATA_SENT_ACK_REC){
            
            I2C_Stop_Condition();
            I2C_Start_Condition();
            I2C_Address_Package(address);                                       // Gửi lại gói địa chỉ và gói dữ liệu
            continue;
        }
        break;
    }
}

void I2C_Send_String(char *string, uint8_t address){                            // Gửi một string đến slave

    uint8_t size = strlen(string);                                              // Kiểm tra kích thức mảng char strlen
    I2C_Start_Condition();                                                      // Gửi tín hiệu START
    I2C_Address_Package(address);                                               // Gửi gói địa chỉ
    for(int run = 0; run <= size; run++){

        I2C_Data_Package(string[run], address);                                 // Gửi liên tiếp các gói dữ liệu trên địa chỉ đã  xác lập
    }
    I2C_Stop_Condition();
    _delay_ms(10);                                                              // Gửi tín hiệu STOP khi đã hoàn thành gói dữ liệu
}

void I2C_Slave_Init(uint8_t slave_address)
{
    
    sei();                                                    // Set bit ng?t toàn c?c trên SREG
    TWAR = slave_address;                                     // Gán ??a ch? cho slave
    TWCR = (1<<TWEN) | (1 << TWIE) | (1<<TWEA) | (1 << TWINT);// Kh?i ??ng giao ti?p I2c, g?i bit START và ng?t
}

void I2C_Slave_Receive5(){


    uint8_t status = TWSR & 0xF8;
    switch(status){
        case REC_ADD_ACK: // Tr??ng h?p nh?n ra ??a ch? c?a mình trong thanh ghi

        memset(rec_string, 0, strlen(rec_string));
        TWCR |= (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
        break;

        case REC_DATA_ACK: // Tr??ng h?p ?ã nh?n ???c gói data

        rec_string[strlen(rec_string)] = TWDR;
        if(rec_string[strlen(rec_string)] == ';'){

            semicolon++;
        }
        if(semicolon == 5){

            new_data_rec = true;
        }
        TWCR |= (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
        break;

        case RS_CONDITION: // Tr??ng h?p nh?n tín hi?u REPEAT START ho?c STOP

        TWCR |= (1 << TWINT) | (1 << TWEA);
        break;
    }
}

// char *I2C_Master_Receive(){                                                         // Hàm được sử dụng để nhận 5 string được định sẵn bởi dấu;
//
// }
