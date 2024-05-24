#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h>

// Define the default piston time
#define PISTONTIME 1500000

class Servo {
public:
    explicit Servo(int pin);  // Constructor
    ~Servo();  // Destructor

    void movePiston(int time = PISTONTIME);  // Default time parameter

private:
    void pushPiston(int time);  // Push piston for specified time
    void pullPiston(int time);  // Pull piston for specified time

    int pin;  // GPIO pin number
    int pwmMin, pwmMax;  // PWM range
};

#endif // SERVO_H
