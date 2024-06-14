#include "MotorController.h"

// Constructor: Initializes motor control pins
MotorController::MotorController(int pinIn1, int pinIn2, int pinEn) : in1(pinIn1), in2(pinIn2), en(pinEn) {}

// Sets up the motor control pins and initializes them
void MotorController::setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(en, OUTPUT);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    softPwmCreate(en, 0, 100); // Setup PWM using wiringPi PWM
}

// Runs the motor in the specified direction
void MotorController::run(bool direction) const {
    if (in1 != -1 && in2 != -1 && en != -1) {
        digitalWrite(in1, direction ? HIGH : LOW);
        digitalWrite(in2, direction ? LOW : HIGH);
        softPwmWrite(en, speed);
    }
}

// Stops the motor
void MotorController::stop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    std::cout << "Stop" << std::endl;
}

// Changes the PWM speed
void MotorController::changeSpeed() const {
    softPwmWrite(en, this->speed);
    std::cout << "Speed ch+anged to " << speed << "%" << std::endl;
}

// Changes the PWM speed to a specified slower speed
void MotorController::slowerDaddy(int slowerSpeed) const {
    softPwmWrite(en, slowerSpeed);
    std::cout << "Yes daddy " << slowerSpeed << "%" << std::endl;
}
