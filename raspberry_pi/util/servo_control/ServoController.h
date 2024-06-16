#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

class ServoController {
public:
    // Constructor: Initializes the servo controller with the given pin
    explicit ServoController(int pin);

    // Destructor: Cleans up resources
    ~ServoController();

    // Pushes the piston forward
    void pushPiston();

    // Pulls the piston backward
    void pullPiston();

    // Stops the piston
    void stopPiston();

private:
    int pin; // Pin number connected to the servo
};

#endif // SERVO_H
