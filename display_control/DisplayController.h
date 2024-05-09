#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class DisplayController {
private:
    int fd; // File descriptor for the I2C device

    void sendCmd(char cmd) {
        char data_u = cmd & 0xF0;         // Upper nibble
        char data_l = (cmd << 4) & 0xF0;  // Lower nibble

        wiringPiI2CWrite(fd, data_u | 0x0C);  // EN=1, RS=0
        wiringPiI2CWrite(fd, data_u | 0x08);  // EN=0, RS=0
        wiringPiI2CWrite(fd, data_l | 0x0C);  // EN=1, RS=0
        wiringPiI2CWrite(fd, data_l | 0x08);  // EN=0, RS=0

        delay(2);  // Wait for the command to execute
    }

    void sendData(char data) {
        char data_u = data & 0xF0;         // Upper nibble
        char data_l = (data << 4) & 0xF0;  // Lower nibble

        wiringPiI2CWrite(fd, data_u | 0x0D);  // EN=1, RS=1
        wiringPiI2CWrite(fd, data_u | 0x09);  // EN=0, RS=1
        wiringPiI2CWrite(fd, data_l | 0x0D);  // EN=1, RS=1
        wiringPiI2CWrite(fd, data_l | 0x09);  // EN=0, RS=1

        delay(2);  // Wait for the data to be processed
    }

public:
    DisplayController(int i2cAddress) {
        wiringPiSetup();  // Initialize wiringPi
        fd = wiringPiI2CSetup(i2cAddress);

        if (fd == -1) {
            std::cerr << "Failed to initialize I2C communication.\n";
            throw std::runtime_error("I2C Setup failed");
        }

        init();  // Initialize the display
    }

    void init() {
        sendCmd(0x33);  // Initialize for 4-bit mode
        sendCmd(0x32);  // Set to 4-bit mode
        sendCmd(0x28);  // Function Set: 2 lines, 5x7 matrix
        sendCmd(0x0C);  // Display ON, Cursor OFF
        sendCmd(0x06);  // Entry mode set: Increment cursor, no display shift
        clear();
    }

    void clear() {
        sendCmd(0x01);  // Clear display
        delay(2000);    // Delay for clearing to complete
    }

    void displayString(const char *str) {
        while (*str) sendData(*str++);
    }
};

#endif // DISPLAYCONTROLLER_H
