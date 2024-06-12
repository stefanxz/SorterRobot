//
// Created by Stefan on 24.05.2024.
//
#include <iostream>
#include "../util/light_sensor/ADCReader.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
    // Setup wiringPi library
    if (wiringPiSetup() == -1)
    {
        std::cerr << "Failed to initialize wiringPi.\n";
        return 1;
    }

    // Initialize ADCReader with default I2C address
    ADCReader adc;
    
    // Run continuous ADC readings and detect colors
    adc.runContinuousRead();

    return 0;
}