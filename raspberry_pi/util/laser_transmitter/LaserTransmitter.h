#ifndef LASER_TRANSMITTER_H
#define LASER_TRANSMITTER_H

#include <wiringPi.h>

class LaserTransmitter {
public:
    explicit LaserTransmitter(int pin);
    void init();
    void turnOn();
    void turnOff();

private:
    int pin;
};

#endif // LASER_TRANSMITTER_H
