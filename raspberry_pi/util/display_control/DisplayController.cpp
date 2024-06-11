//
// Created by Stefan on 24.05.2024.
//
#include "DisplayController.h"

DisplayController::DisplayController(int i2cAddress) : fd(-1) {
    if (i2cAddress != -1) {  // Check if the address is valid
        openI2CBus("/dev/i2c-0", i2cAddress);  // Open I2C bus 0
        displayInit();  // Initialize the display
    } else {
        std::cerr << "Invalid I2C address provided.\n";
        throw std::runtime_error("Invalid I2C address");
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

void DisplayController::openI2CBus(const char *filename, int i2cAddress) {
    if ((fd = open(filename, O_RDWR)) < 0) {
        std::cerr << "Failed to open the I2C bus: " << filename << " Error: " << std::strerror(errno) << "\n";
        throw std::runtime_error("Failed to open I2C bus");
    }
    if (ioctl(fd, I2C_SLAVE, i2cAddress) < 0) {
        std::cerr << "Failed to acquire bus access and/or talk to slave at address " << i2cAddress <<
                  ". Error: " << std::strerror(errno) << "\n";
        close(fd);
        throw std::runtime_error("Failed to acquire I2C bus access or talk to slave");
    }
}

void DisplayController::displayClear() const {
    sendCmd(0x01);  // Clear display
    delay(2);    // Delay for clearing to complete
}

void DisplayController::displayString(const char *str) const {
    this->displayClear();
    int charCount = 0;  // Tracks the number of characters printed on the current line
    int line = 1;       // Start on the first line

    while (*str) {
        // Check if there is a word boundary and enough space left on the line
        if (*str == ' ' || *str == '\n') {
            const char* temp = str + 1;
            int wordLength = 0;

            // Measure the next word
            while (*temp != ' ' && *temp != '\0' && *temp != '\n') {
                temp++;
                wordLength++;
            }

            // Check if the word can fit on the current line
            if (charCount + wordLength >= 20) {
                // Move to the next line if the word doesn't fit
                charCount = 0;
                line++;
                switch (line) {
                    case 2:
                        sendCmd(0xC0); // Second line
                        break;
                    case 3:
                        sendCmd(0x94); // Third line
                        break;
                    case 4:
                        sendCmd(0xD4); // Fourth line
                        break;
                    default:
                        line = 1;
                        sendCmd(0x80); // Move back to the first line
                        break;
                }
                continue; // Skip the space at the line's start
            }
        }

        sendData(*str); // Send character to display
        charCount++;    // Increment the character count
        str++;          // Move to the next character

        if (charCount == 20) {  // Check if the line is full
            charCount = 0;
            line++;
            switch (line) {
                case 2:
                    sendCmd(0xC0);
                    break;
                case 3:
                    sendCmd(0x94);
                    break;
                case 4:
                    sendCmd(0xD4);
                    break;
                default:
                    line = 1;
                    sendCmd(0x80);
                    break;
            }
        }
    }
}


void DisplayController::sendCmd(char cmd) const {
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
