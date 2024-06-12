#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

class MotorController {
private:
    int in1;  // GPIO pin for IN1
    int in2;  // GPIO pin for IN2
    int en;   // GPIO pin for EN (PWM)
    int speed = 50;  // Default speed (PWM duty cycle percentage)

public:
    // Constructor: Initializes motor control pins
    MotorController(int pinIn1, int pinIn2, int pinEn);

    // Sets up the motor control pins and initializes them
    void setup();

    // Runs the motor in the specified direction
    void run(bool direction) const;

    // Stops the motor
    void stop();

    // Changes the PWM speed
    void changeSpeed() const;

    // Changes the PWM speed to a specified slower speed
    void slowerDaddy(int) const;
};

#endif // MOTORCONTROLLER_H
