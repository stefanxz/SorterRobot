#include "ADCReader.h"

ADCReader::ADCReader(int address) : adc_address(address)
{
    wiringPiSetup();
    fd = wiringPiI2CSetup(adc_address);
    if (fd == -1)
    {
        std::cerr << "Failed to init I2C communication.\n";
    }
}

ADCReader::~ADCReader()
{
    // Close any connections or cleanup if necessary
}

void ADCReader::initADC(int config)
{
    int msb = (config >> 8) & 0xFF;
    int lsb = config & 0xFF;
    int config_to_send = (lsb << 8) | msb;
    if (wiringPiI2CWriteReg16(fd, 0x01, config_to_send) == -1)
    {
        std::cerr << "Failed to write configuration.\n";
    }
}

int ADCReader::readADCChannel(int channelConfig)
{
    initADC(channelConfig);
    usleep(9000); // Wait for conversion to complete
    int result = wiringPiI2CReadReg16(fd, 0x00);
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF); // Correct the byte order

    return result;
}

std::string ADCReader::detectColor(int value)
{
    if (value <= 11)
    {
        return "black";
    }
    else if (value >= 18)
    {
        return "white";
    }
    else
    {
        return "other color";
    }
}

void ADCReader::runContinuousRead()
{
    std::cout << "Starting ADC readings...\n";

    // Set the configuration value to a reasonable gain (±2.048V range for ADS1115)
    int configValue = 0xC183; // Existing configuration for the ADS1115
    configValue &= ~0x0E00; // Clear the gain bits
    configValue |= 0x0200; // Set gain to ±2.048V for a balanced sensitivity

    while (true)
    {
        int value = readADCChannel(configValue);
        std::string color = detectColor(value);
        std::cout << "ADC value: " << value << " - Detected color: " << color << '\n';
        usleep(1000000); // 1 second delay between readings
    }
}