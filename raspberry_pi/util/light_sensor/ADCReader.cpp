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

void ADCReader::configureADC(int config)
{
    int msb = (config >> 8) & 0xFF;
    int lsb = config & 0xFF;
    int config_to_send = (lsb << 8) | msb;
    if (wiringPiI2CWriteReg16(fd, 0x01, config_to_send) == -1)
    {
        std::cerr << "Failed to write configuration.\n";
    }
}

void ADCReader::initADC(int config)
{
    configureADC(config);
}

int ADCReader::readADCChannel(int channelConfig)
{
    initADC(channelConfig);
    usleep(9000); // Wait for conversion to complete
    int result = wiringPiI2CReadReg16(fd, 0x00);
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF); // Correct the byte order

    // Adjusting the result interpretation based on actual ADC capabilities
    // This line needs adjustment according to actual ADC resolution and the required sensitivity
    return result; // Return raw ADC value to better understand the scale and needed adjustments
}

void ADCReader::runContinuousRead()
{
    std::cout << "Starting ADC readings...\n";
    int configValue = 0xC183; // Example configuration: Adjust according to actual needed setup
    while (true)
    {
        int value = readADCChannel(configValue);
        std::cout << "ADC value: " << value << '\n'; // Changed to raw value for better calibration
        usleep(1000000);                             // 1s delay
    }
}
