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

    void movePiston() const;  // Default time parameter

private:
    void pushPiston() const;  // Push piston for specified time
    void pullPiston() const;  // Pull piston for specified time

    int pin;  // GPIO pin number
    int pwmMin, pwmMax;  // PWM range
    int time = PISTONTIME;
};

#endif // SERVO_H
