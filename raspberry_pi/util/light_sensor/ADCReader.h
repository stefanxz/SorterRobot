#ifndef ADCREADER_H
#define ADCREADER_H

#include <iostream>
#include <unistd.h>      // For usleep
#include <wiringPiI2C.h> // For I2C communication
#include <wiringPi.h>    // For WiringPi setup

class ADCReader
{
public:
    explicit ADCReader(int address = 0x48);
    ~ADCReader();
    void initADC(int config) const;
    int readADCChannel(int channelConfig) const;
    void runContinuousRead() const;
private:
    int adc_address;
    int fd;
    void configureADC(int config) const;
};

#endif // ADCREADER_H
