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

    // Sends a command to the LCD
    void sendCmd(char cmd) const;

    // I2C address of the display
    int i2cAddress;

    // Sends data to the LCD
    void sendData(char data) const;

    // Opens the I2C bus
    void openI2CBus(const char* filename, int i2cAddress);

public:
    // Constructor that initializes the DisplayController with the given I2C address
    explicit DisplayController(int);

    // Initializes the display
    void displayInit();

    // Clears the display
    void displayClear() const;

    // Displays a string on the LCD
    void displayString(const char *str) const;

    void displayDisk(std::string disk) const;
};

#endif // DISPLAYCONTROLLER_H
