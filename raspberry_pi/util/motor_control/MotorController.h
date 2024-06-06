#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

class MotorController {
private:
    int in1;
    int in2;
    int en;
    int speed = 100;

public:
    MotorController(int pinIn1, int pinIn2, int pinEn);

    void setup();

    void run(bool direction) const;

    void stop();

    void changeSpeed() const;
};

#endif
