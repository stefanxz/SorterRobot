#ifndef LASER_RECEIVER_H
#define LASER_RECEIVER_H

#include <wiringPi.h>

class LaserReceiver {
public:
    explicit LaserReceiver(int pin);
    void init();
    bool isLaserDetected() const;

private:
    int pin;
};

#endif // LASER_RECEIVER_H
