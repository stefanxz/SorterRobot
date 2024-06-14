#include "ADCReader.h"

// Constructor to initialize the ADCReader with a specific I2C address
ADCReader::ADCReader(int address) : adc_address(address) {
    fd = wiringPiI2CSetup(adc_address); // Setup I2C communication with the ADC
    if (fd == -1) {
        std::cerr << "Failed to init I2C communication.\n"; // Error message if I2C setup fails
    }
}

// Destructor
ADCReader::~ADCReader() {}

void ADCReader::initADC(int config) {
    // Extract the most significant and least significant bytes from the config value
    int msb = (config >> 8) & 0xFF;
    int lsb = config & 0xFF;

    // Combine the bytes in the correct order for transmission
    int config_to_send = (lsb << 8) | msb;

    // Write the configuration to the ADC's config register (0x01)
    if (wiringPiI2CWriteReg16(fd, 0x01, config_to_send) == -1) {
        std::cerr << "Failed to write configuration.\n"; // Error message if the configuration write fails
    }
}


// Method to read a value from a specific ADC channel with a given configuration
int ADCReader::readADCChannel(int channelConfig) {
    initADC(channelConfig); // Initialize the ADC with the channel configuration
    usleep(9000); // Wait for the ADC conversion to complete (9ms)

    // Read the conversion result from the ADC's data register (0x00)
    int result = wiringPiI2CReadReg16(fd, 0x00);

    return result;
}


std::string ADCReader::detectColor(int value) {
    if (value >= 28 && value <= 40) {
        return "black";
    } else if (value >= 59 && value <= 75) {
        return "white";
    } else {
        return "other";
    }
}

// Method to run continuous ADC readings and print the values
void ADCReader::runContinuousRead() {
    std::cout << "Starting ADC readings...\n";
    // Set the configuration value to a reasonable gain (±2.048V range for ADS1115)
    int configValue = 0xC183; // Existing configuration for the ADS1115
    configValue &= ~0x0E00; // Clear the gain bits
    configValue |= 0x0200; // Set gain to ±2.048V for a balanced sensitivity

    while (true) {
        int value = readADCChannel(configValue);
        std::string color = detectColor(value);
        std::cout << "ADC value: " << value << " - Detected color: " << color << '\n';
        usleep(1000000); // 1 second delay between readings
    }
}