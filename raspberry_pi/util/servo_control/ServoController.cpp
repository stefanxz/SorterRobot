#include "ServoController.h"

#define FORWARD 18
#define BACKWARD 4
#define PAUSE 0

ServoController::ServoController(int pin) : pin(pin) {
    std::cout << "servo on"<< std::endl;
    softPwmCreate(this->pin, 4, 24);
    stopPiston();
}

ServoController::~ServoController() {
    softPwmStop(pin);
    pinMode(pin, INPUT);
}

void ServoController::pushPiston() {
    softPwmWrite(pin, FORWARD);
}

void ServoController::pullPiston() {
    softPwmWrite(pin, BACKWARD);
}

void ServoController::stopPiston() {
    softPwmWrite(pin, PAUSE);
}