#ifndef LASER_RECEIVER_H
#define LASER_RECEIVER_H

#include <wiringPi.h>

class LaserReceiver {
public:
    // Constructor to initialize the LaserReceiver with a specific pin
    explicit LaserReceiver(int pin);

    // Initializes the laser receiver by setting the pin mode
    void init();

    // Checks if the laser is detected by reading the pin value
    bool isLaserDetected() const;

    // Continuously checks for laser detection and identifies if an object is stuck
    // Returns -1 if an object is stuck for more than the stuckThreshold milliseconds
    // Returns 1 if an object is not stuck or stuck for less than the stuckThreshold milliseconds
    int checkLaserDetection(int stuckThreshold);

private:
    int pin;  // Pin number where the laser receiver is connected
};

#endif // LASER_RECEIVER_H
