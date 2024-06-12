#include "ADCReader.h"

// Constructor to initialize the ADCReader with a specific I2C address
ADCReader::ADCReader(int address) : adc_address(address)
{
    wiringPiSetup(); // Initialize wiringPi
    fd = wiringPiI2CSetup(adc_address); // Setup I2C communication with the ADC
    if (fd == -1)
    {
        std::cerr << "Failed to init I2C communication.\n"; // Error message if I2C setup fails
    }
}

// Destructor
ADCReader::~ADCReader()
{
    // Close any connections or cleanup if necessary
}

// Method to configure the ADC with a given configuration value
void ADCReader::configureADC(int config)
{
    // Extract the most significant and least significant bytes from the config value
    int msb = (config >> 8) & 0xFF;
    int lsb = config & 0xFF;

    // Combine the bytes in the correct order for transmission
    int config_to_send = (lsb << 8) | msb;

    // Write the configuration to the ADC's config register (0x01)
    if (wiringPiI2CWriteReg16(fd, 0x01, config_to_send) == -1)
    {
        std::cerr << "Failed to write configuration.\n"; // Error message if the configuration write fails
    }
}

// Method to initialize the ADC with a specific configuration
void ADCReader::initADC(int config)
{
    configureADC(config); // Call configureADC to set the configuration
}

// Method to read a value from a specific ADC channel with a given configuration
int ADCReader::readADCChannel(int channelConfig)
{
    initADC(channelConfig); // Initialize the ADC with the channel configuration
    usleep(9000); // Wait for the ADC conversion to complete (9ms)

    // Read the conversion result from the ADC's data register (0x00)
    int result = wiringPiI2CReadReg16(fd, 0x00);

    // Correct the byte order of the result
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF);

    // Return the raw ADC value
    return result;
}

// Method to run continuous ADC readings and print the values
void ADCReader::runContinuousRead()
{
    std::cout << "Starting ADC readings...\n";
    int configValue = 0xC183; // Example configuration: Adjust according to actual needed setup

    // Continuously read and print ADC values
    while (true)
    {
        int value = readADCChannel(configValue); // Read a value from the ADC
        std::cout << "ADC value: " << value << '\n'; // Print the raw ADC value
        usleep(1000000); // Wait for 1 second before the next reading
    }
}
