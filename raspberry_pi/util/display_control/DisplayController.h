#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class DisplayController {
private:
    int fd; // File descriptor for the I2C device

    void sendCmd(char cmd);
    int i2cAddress;
    void sendData(char data) const;

public:
    explicit DisplayController(int);

    void displayInit();

    void displayClear();

    void displayString(const char *str) const;
};

#endif // DISPLAYCONTROLLER_H
