//
// Created by Stefan on 24.05.2024.
//
#include "ServoController.h"

#define FORWARD 18   // Minimum effective PWM position
#define BACKWARD 4 // Maximum effective PWM position


ServoController::ServoController(int pin) {
    softPwmCreate(pin, 4, 24); // Setup PWM on the pin with adjusted range
}

ServoController::~ServoController() {
    softPwmStop(pin); // Stop PWM
    pinMode(pin, INPUT); // Reset pin to input to stop signal
}

void ServoController::movePiston() {
    std::cout << "Moving piston on pin" << " " << pin << std::endl;
    pushPiston();
    pullPiston();
}

void ServoController::pushPiston() {
    softPwmWrite(pin, FORWARD); // Use defined FORWARD position
    usleep(time);
}

void ServoController::pullPiston() {
    softPwmWrite(pin, BACKWARD); // Use defined BACKWARD position
    usleep(time - 150000);
}
