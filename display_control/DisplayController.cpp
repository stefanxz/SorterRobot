#include "DisplayController.h"

// Constructor implementation
DisplayController::DisplayController(int i2cAddress) {
    wiringPiSetup();  // Initialize wiringPi
    fd = wiringPiI2CSetup(i2cAddress);

    if (fd == -1) {
        std::cerr << "Failed to initialize I2C communication.\n";
        throw std::runtime_error("I2C Setup failed");
    }

    init();  // Initialize the display
}

// Initialize display settings
void DisplayController::init() {
    sendCmd(0x33);  // Initialize for 4-bit mode
    sendCmd(0x32);  // Set to 4-bit mode
    sendCmd(0x28);  // Function Set: 2 lines, 5x7 matrix
    sendCmd(0x0C);  // Display ON, Cursor OFF
    sendCmd(0x06);  // Entry mode set: Increment cursor, no display shift
    clear();
}

// Clear the display
void DisplayController::clear() {
    sendCmd(0x01);  // Clear display
    delay(2000);    // Delay for clearing to complete
}

// Send a command to the display
void DisplayController::sendCmd(char cmd) {
    char data_u = cmd & 0xF0;         // Upper nibble
    char data_l = (cmd << 4) & 0xF0;  // Lower nibble

    wiringPiI2CWrite(fd, data_u | 0x0C);  // EN=1, RS=0
    wiringPiI2CWrite(fd, data_u | 0x08);  // EN=0, RS=0
    wiringPiI2CWrite(fd, data_l | 0x0C);  // EN=1, RS=0
    wiringPiI2CWrite(fd, data_l | 0x08);  // EN=0, RS=0

    delay(2);  // Wait for the command to execute
}

// Send data to the display
void DisplayController::sendData(char data) {
    char data_u = data & 0xF0;         // Upper nibble
    char data_l = (data << 4) & 0xF0;  // Lower nibble

    wiringPiI2CWrite(fd, data_u | 0x0D);  // EN=1, RS=1
    wiringPiI2CWrite(fd, data_u | 0x09);  // EN=0, RS=1
    wiringPiI2CWrite(fd, data_l | 0x0D);  // EN=1, RS=1
    wiringPiI2CWrite(fd, data_l | 0x09);  // EN=0, RS=1

    delay(2);  // Wait for the data to be processed
}

// Display a string on the display
void DisplayController::displayString(const char *str) {
    while (*str) sendData(*str++);
}
