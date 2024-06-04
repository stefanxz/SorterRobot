#ifndef LASER_RECEIVER_H
#define LASER_RECEIVER_H

#include <Arduino.h> // Include Arduino standard library

class LaserReceiver
{
public:
    explicit LaserReceiver(int pin); // Constructor with pin number
    void init();                     // Initialize the pin as input
    bool isLaserDetected() const;    // Check if laser is detected

private:
    int pin_; // Pin number where the laser receiver is connected
};

#endif // LASER_RECEIVER_H
