#include "ColorDisplaySystem.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

ColorDisplaySystem::ColorDisplaySystem(int adcAddress, int displayAddress)
    : adc(adcAddress), display(displayAddress) {}

ColorDisplaySystem::~ColorDisplaySystem() {}

void ColorDisplaySystem::setup() {
    adc.initADC(0xC183);  // Configure ADC for continuous read mode
    display.init();  // Initialize display
}

std::string ColorDisplaySystem::getColorFromSensor() {
    int adcValue = adc.readADCChannel(0x01);
    if (adcValue < 1000) {
        return "Black";
    } else if (adcValue < 2000) {
        return "Another color";
    } else {
        return "White";
    }
}

void ColorDisplaySystem::displayColor(const std::string& color) {
    display.clear();
    display.displayString(color.c_str());
}

void ColorDisplaySystem::run() {
    while (true) {
        std::string color = getColorFromSensor();
        displayColor(color);
        usleep(1000000);  // Update every second
    }
}
