#include "LaserReceiver.h"
#include <iostream>

LaserReceiver::LaserReceiver(int pin) : pin_(pin) {}

void LaserReceiver::init() {
    wiringPiSetupPhys();  // Initialize wiringPi using physical pin numbering
    pinMode(pin_, INPUT); // Set the specified pin to input mode
}

bool LaserReceiver::isLaserDetected() const {
    return digitalRead(pin_) == HIGH;
}
