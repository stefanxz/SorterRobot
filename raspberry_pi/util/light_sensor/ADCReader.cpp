#include "ADCReader.h"

ADCReader::ADCReader(int address) : adc_address(address) {
    if (adc_address != -1) {
        fd = wiringPiI2CSetup(adc_address);
        if (fd == -1) {
            std::cerr << "Failed to init I2C communication.\n";
        }
    }
}

ADCReader::~ADCReader() {
    // Close any connections or cleanup if necessary
}

void ADCReader::configureADC(int config) const {
    if (config != 0) {
        int msb = (config >> 8) & 0xFF;
        int lsb = config & 0xFF;
        int config_to_send = (lsb << 8) | msb;
        if (wiringPiI2CWriteReg16(fd, 0x01, config_to_send) == -1) {
            std::cerr << "Failed to write configuration.\n";
        }
    }
}

void ADCReader::initADC(int config) const {
    if (adc_address != -1) { configureADC(config); }
}

int ADCReader::readADCChannel(int channelConfig) const {
    initADC(channelConfig);
    usleep(9000); // Wait for conversion to complete
    int result = wiringPiI2CReadReg16(fd, 0x00);
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF); // Correct the byte order
    return static_cast<int>(result * 0.125); // Scale to mV based on gain
}

void ADCReader::runContinuousRead() const {
    std::cout << "Starting ADC readings...\n";
    while (true) {
        int value = readADCChannel(0xC183);
        std::cout << "ADC value: " << value << "mV\n";
        usleep(1000000); // 1s delay
    }
}
