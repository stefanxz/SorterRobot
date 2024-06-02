//
// Created by Stefan on 24.05.2024.
//
#include "DisplayController.h"

DisplayController::DisplayController(int i2cAddress) {
    if (i2cAddress != -1) {
        fd = wiringPiI2CSetup(i2cAddress);

        if (fd == -1) {
            std::cerr << "Failed to initialize I2C communication.\n";
            throw std::runtime_error("I2C Setup failed");
        }

        displayInit();  // Initialize the display
    }
}

void DisplayController::displayInit() {
    sendCmd(0x33);  // Initialize for 4-bit mode
    sendCmd(0x32);  // Set to 4-bit mode
    sendCmd(0x28);  // Function Set: 2 lines, 5x7 matrix
    sendCmd(0x0C);  // Display ON, Cursor OFF
    sendCmd(0x06);  // Entry mode set: Increment cursor, no display shift
    displayClear();
}

void DisplayController::displayClear() {
    sendCmd(0x01);  // Clear display
    delay(2000);    // Delay for clearing to complete
}

void DisplayController::displayString(const char *str) const {
    while (*str) sendData(*str++);
}

void DisplayController::sendCmd(char cmd) {
    char data_u = cmd & 0xF0;         // Upper nibble
    char data_l = (cmd << 4) & 0xF0;  // Lower nibble

    wiringPiI2CWrite(fd, data_u | 0x0C);  // EN=1, RS=0
    wiringPiI2CWrite(fd, data_u | 0x08);  // EN=0, RS=0
    wiringPiI2CWrite(fd, data_l | 0x0C);  // EN=1, RS=0
    wiringPiI2CWrite(fd, data_l | 0x08);  // EN=0, RS=0

    delay(2);  // Wait for the command to execute
}

void DisplayController::sendData(char data) const {
    char data_u = data & 0xF0;         // Upper nibble
    char data_l = (data << 4) & 0xF0;  // Lower nibble

    wiringPiI2CWrite(fd, data_u | 0x0D);  // EN=1, RS=1
    wiringPiI2CWrite(fd, data_u | 0x09);  // EN=0, RS=1
    wiringPiI2CWrite(fd, data_l | 0x0D);  // EN=1, RS=1
    wiringPiI2CWrite(fd, data_l | 0x09);  // EN=0, RS=1

    delay(2);  // Wait for the data to be processed
}
