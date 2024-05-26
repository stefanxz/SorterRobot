//
// Created by Stefan on 24.05.2024.
//
#include "MotorController.h"

MotorController::MotorController(int pinIn1, int pinIn2, int pinEn) : in1(pinIn1), in2(pinIn2), en(pinEn) {}

void MotorController::setup() {
    wiringPiSetupGpio(); // Initialize wiringPi using the Broadcom GPIO pin numbers
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(en, OUTPUT);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    softPwmCreate(en, 0, 100); // Setup PWM using wiringPi PWM
}

// Run the motor in either forward or backward direction based on the boolean direction
void MotorController::run(bool direction, int speed) {
    digitalWrite(in1, direction ? HIGH : LOW);
    digitalWrite(in2, direction ? LOW : HIGH);
    softPwmWrite(en, speed);
    std::cout << (direction ? "Forward" : "Backward") << std::endl;
}

// Stop the motor
void MotorController::stop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    std::cout << "Stop" << std::endl;
}

// Change the PWM speed
void MotorController::changeSpeed(int speed) {
    softPwmWrite(en, speed);
    std::cout << "Speed changed to " << speed << "%" << std::endl;
}