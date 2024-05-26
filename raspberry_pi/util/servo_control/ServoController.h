#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h>

// Define the default piston time
#define PISTONTIME 1500000

class ServoController {
public:
    explicit ServoController(int pin);  // Constructor
    ~ServoController();  // Destructor

    void movePiston();  // Default time parameter

private:
    void pushPiston();  // Push piston for specified time
    void pullPiston();  // Pull piston for specified time

    int pin;  // GPIO pin number
    int pwmMin, pwmMax;  // PWM range
    int time = PISTONTIME;
};

#endif // SERVO_H
