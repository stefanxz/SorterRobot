#ifndef ADCREADER_H
#define ADCREADER_H

#include <iostream>
#include <unistd.h>      // For usleep
#include <wiringPiI2C.h> // For I2C communication
#include <wiringPi.h>    // For WiringPi setup
#include <vector>
#include <numeric>
#include <cmath>

class ADCReader
{
public:
    explicit ADCReader(int address = 0x48); // Constructor with default I2C address
    ~ADCReader();                           // Destructor

    void initADC(int config);              // Initializes the ADC with the specified configuration
    int readADCChannel(int channelConfig); // Reads an ADC channel with the given configuration
    void runContinuousRead();              // Runs continuous ADC readings and prints them

    std::string detectColor(int value);    // Detects color based on ADC value

private:
    int adc_address; // I2C address of the ADC
    int fd;          // File descriptor for I2C communication
};

#endif // ADCREADER_H