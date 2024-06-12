#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h>

class ServoController {
public:
    ServoController(int pin);
    ~ServoController();

    void pushPiston();
    void pullPiston();
    void stopPiston();

private:
    int pin;
};

#endif // SERVO_H