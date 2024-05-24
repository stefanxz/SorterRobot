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
    // Constructor to initialize the GPIO pins
    MotorController(int pinIn1, int pinIn2, int pinEn)
        : in1(pinIn1), in2(pinIn2), en(pinEn) {}

    // Setup function for the motor controller
    void setup() {
        wiringPiSetupGpio(); // Initialize wiringPi using the Broadcom GPIO pin numbers
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(en, OUTPUT);
        
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        softPwmCreate(en, 0, 100); // Setup PWM using wiringPi PWM
    }

    // Run the motor in either forward or backward direction based on the boolean direction
    void run(bool direction, int speed) {
        digitalWrite(in1, direction ? HIGH : LOW);
        digitalWrite(in2, direction ? LOW : HIGH);
        softPwmWrite(en, speed);
        std::cout << (direction ? "Forward" : "Backward") << std::endl;
    }

    // Stop the motor
    void stop() {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        std::cout << "Stop" << std::endl;
    }

    // Change the PWM speed
    void changeSpeed(int speed) {
        softPwmWrite(en, speed);
        std::cout << "Speed changed to " << speed << "%" << std::endl;
    }
};

#endif
