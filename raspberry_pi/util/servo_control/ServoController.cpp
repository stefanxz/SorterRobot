#include "ServoController.h"

#define FORWARD 20  // PWM value for forward movement
#define BACKWARD 10  // PWM value for backward movement
#define PAUSE 0     // PWM value for stopping

ServoController::ServoController(int pin) : pin(pin) {
    softPwmCreate(this->pin, 0, 100);  // Set up PWM on the specified pin
    stopPiston();  // Ensure the piston is stopped initially
}

ServoController::~ServoController() {
    softPwmStop(pin);  // Stop PWM on the pin
    pinMode(pin, INPUT);  // Reset the pin mode
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
