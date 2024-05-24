#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h> // for usleep

#define FORWARD 18   // Minimum effective PWM position
#define BACKWARD 4 // Maximum effective PWM position

class Servo {
public:
    explicit Servo(int pin) : pin(pin), pwmMin(FORWARD), pwmMax(BACKWARD) {
        static bool initialized = false;
        if (!initialized) {
            wiringPiSetupPhys(); // Use the physical pin numbering scheme
            initialized = true;
        }
        softPwmCreate(pin, 4, 24); // Setup PWM on the pin with adjusted range
    }

    ~Servo() {
        std::cout << "Stopping servo on pin " << pin << std::endl;
        softPwmStop(pin); // Stop PWM
        pinMode(pin, INPUT); // Reset pin to input to stop signal
    }

    void movePiston(int time) {
        std::cout << "Moving piston on pin " << pin << std::endl;
        pushPiston(time);
        pullPiston(time);
    }

private:
    void pushPiston(int time) {
        std::cout << "Pushing piston on pin " << pin << std::endl;
        softPwmWrite(pin, FORWARD); // Use defined FORWARD position
        usleep(time);
    }

    void pullPiston(int time) {
        std::cout << "Pulling piston on pin " << pin << std::endl;
        softPwmWrite(pin, BACKWARD); // Use defined BACKWARD position
        usleep(time - 150000);
    }

    int pin;
    int pwmMin, pwmMax;
};

#endif // SERVO_H
