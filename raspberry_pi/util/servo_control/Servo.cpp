//
// Created by Stefan on 24.05.2024.
//
#include "Servo.h"

#define FORWARD 18   // Minimum effective PWM position
#define BACKWARD 4 // Maximum effective PWM position


Servo::Servo(int pin) : pin(pin), pwmMin(FORWARD), pwmMax(BACKWARD) {
    static bool initialized = false;
    if (!initialized) {
        wiringPiSetupPhys(); // Use the physical pin numbering scheme
        initialized = true;
    }
    softPwmCreate(pin, 4, 24); // Setup PWM on the pin with adjusted range
}

Servo::~Servo() {
    softPwmStop(pin); // Stop PWM
    pinMode(pin, INPUT); // Reset pin to input to stop signal
}

void Servo::movePiston(int time) {
    std::cout << "Moving piston on pin" << " " << pin << std::endl;
    pushPiston(time);
    pullPiston(time);
}

void Servo::pushPiston(int time) {
    softPwmWrite(pin, FORWARD); // Use defined FORWARD position
    usleep(time);
}

void Servo::pullPiston(int time) {
    softPwmWrite(pin, BACKWARD); // Use defined BACKWARD position
    usleep(time - 150000);
}
