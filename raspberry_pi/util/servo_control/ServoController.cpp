//
// Created by Stefan on 24.05.2024.
//
#include "ServoController.h"
#include <iostream>
#include <chrono>

#define FORWARD 18
#define BACKWARD 4
#define PAUSE 0


ServoController::ServoController(int pin) : pin(pin) {
    std::cout << "Initializing ServoController with pin: " << pin << std::endl;
    softPwmCreate(this->pin, 4, 24);
}
ServoController::~ServoController() {
    softPwmStop(pin);
    pinMode(pin, INPUT);
}

void ServoController::movePiston() {
    unsigned long currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    if (currentTime >= nextActionTime) {
        switch (state) {
            case IDLE:
                pushPiston();
                state = PUSHING;
                nextActionTime = currentTime + time;
                break;
            case PUSHING:
                pullPiston();
                state = PULLING;
                nextActionTime = currentTime + time;
                break;
            case PULLING:
                pausePiston();
                state = PAUSING;
                nextActionTime = currentTime + time;
                break;
            case PAUSING:
                state = IDLE;
                break;
        }
    }
}

void ServoController::pushPiston() {
    softPwmWrite(pin, FORWARD);
}

void ServoController::pullPiston() {
    softPwmWrite(pin, BACKWARD);
}

void ServoController::pausePiston() {
    softPwmWrite(pin, PAUSE);
}

ServoState ServoController::getState() const {
    return state;
}