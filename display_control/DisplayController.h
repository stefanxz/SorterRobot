//
// Created by Stefan on 07.05.2024.
//


#ifndef INC_2IO75_DISPLAYCONTROLLER_H
#define INC_2IO75_DISPLAYCONTROLLER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

class DisplayController {
private:
private:
    int fd;
    int address;

    void display_send_data(char data) { // send data via the i2c bus by using the upper and lower nibble of data
        char data_u, data_l;
        data_u = data & 0xf0;
        data_l = (data << 4) & 0xf0;
        wiringPiI2CWrite(fd, data_u | 0x0D);
        wiringPiI2CWrite(fd, data_u | 0x09);
        wiringPiI2CWrite(fd, data_l | 0x0D);
        wiringPiI2CWrite(fd, data_l | 0x09);
    }

public:
    DisplayController(int address)
            : address(address) {} // Class constructor

    ~DisplayController() { // Class destructor
        clear();
        send_cmd(0x01); // Turn off the display
    }

    void display_setup() {
        wiringPiSetup();
        fd = wiringPiI2CSetup(0x27);

        lcd_send_cmd(0x02);  // Initialize LCD in 4-bit mode
        lcd_send_cmd(0x28);  // 2 lines, 5x7 matrix
        lcd_send_cmd(0x0C);  // Turn cursor off
        lcd_send_cmd(0x06);  // Auto increment cursor
        lcd_clear();         // Clear display
    }

    void display_string(const char *str) { // display strings
        while (*str) display_send_data(*str++);
    }

    void display_clear() {
        lcd_send_cmd(0x01);  // Clear display
        delay(2000);         // Delay for 2 seconds
    }
};

#endif //INC_2IO75_DISPLAYCONTROLLER_H
