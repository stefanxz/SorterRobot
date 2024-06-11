#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <fcntl.h>  // For file control definitions
#include <unistd.h>  // For UNIX standard function definitions
#include <sys/ioctl.h>  // For IOCTLs
#include <linux/i2c-dev.h>  // For the I2C device definitions
#include <cerrno>  // For error number definitions
#include <cstring>  // For strerror()
#include <iostream>  // For input-output streams
#include <stdexcept>  // For standard exceptions


class DisplayController {
private:
    int fd; // File descriptor for the I2C device

    void sendCmd(char cmd) const ;
    int i2cAddress;
    void sendData(char data) const;
    void openI2CBus(const char* filename, int i2cAddress);

public:
    explicit DisplayController(int);

    void displayInit();

    void displayClear() const;

    void displayString(const char *str) const;
};

#endif // DISPLAYCONTROLLER_H
