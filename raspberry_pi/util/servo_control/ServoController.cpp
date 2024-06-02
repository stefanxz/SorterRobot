//
// Created by Stefan on 24.05.2024.
//
#include "ServoController.h"

#define FORWARD 18
#define BACKWARD 4


ServoController::ServoController(int pin) : pin(pin) {
    std::cout << "Initializing ServoController with pin: " << pin << std::endl;
    softPwmCreate(this->pin, 4, 24);
}
ServoController::~ServoController() {
    softPwmStop(pin);
    pinMode(pin, INPUT);
}

void ServoController::movePiston() const {
    if (pin != -1) {
        std::cout << "Moving piston on pin" << " " << pin << std::endl;
        pushPiston();
        pullPiston();
    }
}

void ServoController::pushPiston() const {
    softPwmWrite(pin, FORWARD);
    usleep(time);
}

void ServoController::pullPiston() const {
    softPwmWrite(pin, BACKWARD);
    usleep(time - 150000);
}
