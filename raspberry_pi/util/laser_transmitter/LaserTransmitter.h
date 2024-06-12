#ifndef LASER_TRANSMITTER_H
#define LASER_TRANSMITTER_H

#include <wiringPi.h>

// Class representing a laser transmitter
class LaserTransmitter {
public:
    // Constructor to initialize the LaserTransmitter with a specific pin
    explicit LaserTransmitter(int pin);

    // Initializes the laser transmitter
    void init();

    // Turns on the laser
    void turnOn();

    // Turns off the laser
    void turnOff();

private:
    int pin;  // The pin used to control the laser
};

#endif // LASER_TRANSMITTER_H
