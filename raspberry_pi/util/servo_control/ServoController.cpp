#include "ServoController.h"

#define FORWARD 18  // PWM value for forward movement
#define BACKWARD 4  // PWM value for backward movement
#define PAUSE 0     // PWM value for stopping

// Constructor: Initializes the servo controller with the given pin
ServoController::ServoController(int pin) : pin(pin) {
    std::cout << "servo on" << std::endl;
    softPwmCreate(this->pin, 4, 24);  // Set up PWM with initial value and range
    stopPiston();  // Ensure the piston is stopped initially
}

// Destructor: Stops the PWM and resets the pin mode
ServoController::~ServoController() {
    softPwmStop(pin);  // Stop PWM on the pin
    pinMode(pin, INPUT);  // Set the pin to input mode
}

// Pushes the piston forward by writing the FORWARD PWM value
void ServoController::pushPiston() {
    softPwmWrite(pin, FORWARD);
}

// Pulls the piston backward by writing the BACKWARD PWM value
void ServoController::pullPiston() {
    softPwmWrite(pin, BACKWARD);
}

// Stops the piston by writing the PAUSE PWM value
void ServoController::stopPiston() {
    softPwmWrite(pin, PAUSE);
}
