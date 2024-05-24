#ifndef ADCREADER_H
#define ADCREADER_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

class ADCReader {
public:
    ADCReader(int address);
    ~ADCReader();
    void initADC(int config);
    int readADCChannel(int channelConfig);
    void runContinuousRead();

private:
    int adc_address;
    int fd;
    void configureADC(int config);
};

#endif // ADCREADER_H
