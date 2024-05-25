#ifndef COLOR_DISPLAY_SYSTEM_H
#define COLOR_DISPLAY_SYSTEM_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "ADCReader.h"
#include "DisplayController.h"
#include <string>
#include <unistd.h> // for usleep

class ColorDisplaySystem {
public:
    ColorDisplaySystem(int adcAddress, int displayAddress);
    ~ColorDisplaySystem();
    void setup();
    void run();

private:
    ADCReader adc;
    DisplayController display;
    std::string getColorFromSensor();
    void displayColor(const std::string& color);
};

#endif // COLOR_DISPLAY_SYSTEM_H
