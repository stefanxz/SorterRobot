//
// Created by Stefan on 24.05.2024.
//
#include "MotorController.h"

MotorController::MotorController(int pinIn1, int pinIn2, int pinEn) : in1(pinIn1), in2(pinIn2), en(pinEn) {}

void MotorController::setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(en, OUTPUT);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    softPwmCreate(en, 0, 100); // Setup PWM using wiringPi PWM
}

void MotorController::run(bool direction) const {
    if (in1 != -1 && in2 != -1 && en != -1) {
        digitalWrite(in1, direction ? HIGH : LOW);
        digitalWrite(in2, direction ? LOW : HIGH);
        softPwmWrite(en, speed);
    }
}

// Stop the motor
void MotorController::stop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    std::cout << "Stop" << std::endl;
}

// Change the PWM speed
void MotorController::changeSpeed() const {
    softPwmWrite(en, this->speed);
    std::cout << "Speed changed to " << speed << "%" << std::endl;
}

void MotorController::slowerDaddy(int slowerSpeed) const{
    softPwmWrite(en, slowerSpeed);
    std::cout << "Yes daddy" << slowerSpeed << "%" << std::endl;
}