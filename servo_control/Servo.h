// Servo.h
#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <cmath>    // for std::round
#include <unistd.h> // for usleep

class Servo {
public:
    Servo(int pin) : pin(pin), rangeStart(4), rangeEnd(26){
        wiringPiSetupPhys(); // Use the physical pin numbering scheme
        softPwmCreate(pin, rangeStart, rangeEnd); // Setup PWM on the pin
    }

    ~Servo() {
        std::cout << "Stopping servo" << std::endl;
        softPwmStop(pin); // Stop PWM
        pinMode(pin, INPUT); // Reset pin to input to stop signal
    }

    void moveToAngle(int angle) {
        if (angle < 0 || angle > 180) {
            std::cerr << "Angle out of range" << std::endl;
            return;
        }

        int position = std::round((angle / 180.0) * 21); // Calculate the position from 0 to 19
        std::cout << "Computed position " << position << std::endl;
        moveToPosition(position);
    }

    void moveToPosition(int pos) {
        int actualPos = pos + 4; // Adjust for internal PWM range shift
        std::cout << "Actual position " << actualPos << std::endl;
        if (actualPos < rangeStart || actualPos > rangeEnd) {
            std::cerr << "Position out of range" << std::endl;
            return;
        }
        std::cout << "Moving to position: " << pos << " (actual PWM position: " << actualPos << ")" << std::endl;
        softPwmWrite(pin, actualPos);
        usleep(1000000); // Delay to allow servo to move
    }

private:
    int pin;
    int rangeStart;
    int rangeEnd;
};

#endif // SERVO_H
