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

public:
    MotorController(int pinIn1, int pinIn2, int pinEn);
    void setup();
    void run(bool direction, int speed);
    void stop();
    void changeSpeed(int speed);
};

#endif
