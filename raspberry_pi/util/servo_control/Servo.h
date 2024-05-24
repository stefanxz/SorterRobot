#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h>

class Servo {
public:
    explicit Servo(int pin);

    ~Servo();

    void movePiston(int time);

private:
    void pushPiston(int time);

    void pullPiston(int time);

    int pin;
    int pwmMin, pwmMax;
};

#endif // SERVO_H
