#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

const int adc_address = 0x48; // ADS1115 default address
const int config_register = 0x01;
const int conversion_register = 0x00;

const int white = 8;
const int black = 1;

int channel_config = 0xC183; // Configs for channels 0 to 3

void init_adc(int fd, int config) {
    int msb = (config >> 8) & 0xFF;
    int lsb = config & 0xFF;
    int config_to_send = (lsb << 8) | msb;
    if (wiringPiI2CWriteReg16(fd, config_register, config_to_send) == -1) {
        std::cerr << "Failed to write configuration.\n";
    }
}

int read_adc_channel(int fd, int channel_config) {
    init_adc(fd, channel_config);
    usleep(9000); // Wait for conversion to complete
    int result = wiringPiI2CReadReg16(fd, conversion_register);
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF); // Correct the byte order
    return static_cast<int>(result * 0.125); // Scale to mV based on gain
}

int main() {
    int fd = wiringPiI2CSetup(adc_address);
    if (fd == -1) {
        std::cerr << "Failed to init I2C communication.\n";
        return 1;
    }

    std::cout << "I2C setup successful. Starting ADC readings...\n";
    while (true) {
        int value = read_adc_channel(fd, channel_config);
        std::cout << "ADC value: " << value << "mV\n";
        if (abs(value - white) <= 1) {
            std::cout << "White detected.\n";
        } else if (value <= black) {
            std::cout << "Black detected.\n";
        } else {
            std::cout << "Error color.\n";
        }
        usleep(1000000); // 1s delay
    }

    return 0;
}
