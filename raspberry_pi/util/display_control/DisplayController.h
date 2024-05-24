#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class DisplayController {
private:
    int fd; // File descriptor for the I2C device
    void sendCmd(char cmd);

    void sendData(char data);

public:
    explicit DisplayController(int i2cAddress);

    void displayInit();

    void displayClear();

    void displayString(const char *str);
};

#endif // DISPLAYCONTROLLER_H
