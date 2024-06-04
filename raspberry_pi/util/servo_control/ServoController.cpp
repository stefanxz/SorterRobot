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

void ServoController::movePiston() const {
    if (pin != -1) {
        std::cout << "Moving piston on pin" << " " << pin << std::endl;
        pushPiston();
        pullPiston();
        pausePiston();
    }
}



void ServoController::pushPiston() const {
    softPwmWrite(pin, FORWARD);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::microseconds(time);

    while (std::chrono::high_resolution_clock::now() < end) {
    }
}

void ServoController::pullPiston() const {
    softPwmWrite(pin, BACKWARD);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::microseconds(time);

    while (std::chrono::high_resolution_clock::now() < end) {
    }
}

void ServoController::pausePiston() const {
    softPwmWrite(pin, PAUSE);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::microseconds(time);

    while (std::chrono::high_resolution_clock::now() < end) {
    }
}
