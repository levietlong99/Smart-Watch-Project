/*
*
*   LCD.c
*
*/

#include "LCD.h"

void LCD_Command( unsigned char cmnd )
{

    LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);                               // Gửi 4 bit đầu của command đến LCD PORT
                                                                                // D0; D1; D2; D3 - LCD đặt = 0
                                                                                // Xuất 4 bit đầu của lệnh vào PORT đầu ra của LCD
    LCD_Port &= ~ (1<<RS);                                                      // Chân Reset set về 0 - Nhận biết gửi lệnh
    LCD_Port |= (1<<EN);                                                        // Kích hoạt và tắt chân Enable để tạo xung giao tiếp giữa ST706x và MCU
    _delay_us(1);
    LCD_Port &= ~ (1<<EN);
    _delay_us(200);

    LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);                                 // Gửi 4 bit cuối của lệnh vào PORT đầu ra của LCD
                                                                                // D0; D1; D2; D3 - LCD đặt = 0
                                                                                // Xuất 4 bit cuối của lệnh vào PORT đầu ra của LCD
    LCD_Port |= (1<<EN);                                                        // Kích hoạt và tắt chân Enable để tạo xung giao tiếp giữa ST706x và MCU
    _delay_us(1);
    LCD_Port &= ~ (1<<EN);
    _delay_ms(2);
}

void LCD_Char(char data)                                                        // Hàm gửi ký tự cho LCD 1602
{

    LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);                               // Gửi 4 bit đầu của dữ liệu data
                                                                                // 4 bit đầu của chân Data LCD set = 0
                                                                                // Lấy 4 bit đầu của data để gửi vào LCD PORT
    LCD_Port |= (1<<RS);                                                        //Chân RESET đưa lên 1 - Nhận biết gửi dữ liệu
    LCD_Port |= (1<<EN);                                                        // Set và pull chân enable nhằm tạo xung để giao tiếp giữa ST706x và MCU
    _delay_us(1);
    LCD_Port &= ~ (1<<EN);
    _delay_us(200);

    LCD_Port = (LCD_Port & 0x0F) | (data << 4);                                 // Gửi 4 bit cuối của dữ liệu data
                                                                                // Lấy 4 bit cuối của Data để gửi vào LCD  PORT
    LCD_Port |= (1<<EN);                                                        // Set và pull chân enable nhằm tạo xung để giao tiếp giữa ST706x và MCU
    _delay_us(1);
    LCD_Port &= ~ (1<<EN);
    _delay_ms(2);
}

void LCD_Init (void)                                                            //  Hàm khởi tạo cài đặt cho LCD
{

    LCD_Dir = 0xFF;                                                             // Ở đây ta dùng 4 bit mode (datasheet) nên chỉ set 4 chân đầu của PORT làm OUTPUT
    _delay_ms(20);                                                              // Thời gian chờ khởi động của LCD > 15ms - Datasheet
    
    LCD_Command(0x02);                                                          // Khởi động chế độ 4 bit - xem Fuction set trong LCD datasheet
    LCD_Command(0x28);                                                          // Set màn hình 2 dòng và chế  độ 1 cỡ chữ 8 pixel nhỏ
    LCD_Command(0x0c);                                                          // Bật màn hình và tắt con trỏ - Display onoff datasheet
    LCD_Command(0x06);                                                          // Đẩy con trỏ chuột sang bên phải
    LCD_Command(0x01);                                                          // Xóa màn hình LCD
    _delay_ms(2);
}

void LCD_String (char *str)                                                     // Hàm gửi chuỗi cho LCD
{

    for(int runVar = 0; str[runVar] != 0; runVar++)                             //Kiểm tra String cho đến NULL
    {
        LCD_Char (str[runVar]);
    }
}

void LCD_String_xy (char row, char pos, char *str)                              // Gửi ký tự tại tọa độ X, Y trên LCD
{

    if (row == 0 && pos < 16){                                                  // Gửi lệnh hiệu chỉnh địa chỉ ô và di chuyển con trỏ đên ô mong muốn
                                                                                // 2 Bit đầu set 11 hoặc 10 - DDRAM address tham khảo datasheet
        LCD_Command((pos & 0x0F) | 0x80);
    }
    else if (row == 1 && pos < 16){

        LCD_Command((pos & 0x0F)|0xC0);
    }
    LCD_String(str);                                                            // Gọi hàm in string
}

void LCD_Clear()
{

    LCD_Command (0x01);                                                         // Câu lệnh xóa màn hình LCD
    _delay_ms(2);
    LCD_Command (0x80);                                                         // Di chuyển trỏ về lại vị trí ban đầu
}