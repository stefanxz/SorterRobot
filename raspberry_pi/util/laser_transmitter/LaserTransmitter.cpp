#include "LaserTransmitter.h"
#include <iostream>

LaserTransmitter::LaserTransmitter(int pin) : pin(pin) {}

void LaserTransmitter::init() {
    wiringPiSetupPhys();  // Initialize wiringPi using physical pin numbering
    pinMode(pin, OUTPUT); // Set the specified pin to output mode
}

void LaserTransmitter::turnOn() {
    digitalWrite(pin, HIGH);  // Set the pin high to turn on the laser
}

void LaserTransmitter::turnOff() {
    digitalWrite(pin, LOW);  // Set the pin low to turn off the laser
}
