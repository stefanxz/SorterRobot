#ifndef ADCREADER_H
#define ADCREADER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

class ADCReader {
public:
    explicit ADCReader(int address);
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
