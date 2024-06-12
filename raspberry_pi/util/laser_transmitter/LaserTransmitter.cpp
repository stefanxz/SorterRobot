#include "LaserTransmitter.h"
#include <iostream>

// Constructor to initialize the LaserTransmitter with a specific pin
LaserTransmitter::LaserTransmitter(int pin) : pin(pin) {}

// Initializes the laser transmitter by setting up wiringPi and configuring the pin mode
void LaserTransmitter::init() {
    wiringPiSetupPhys();  // Initialize wiringPi using physical pin numbering
    pinMode(pin, OUTPUT); // Set the specified pin to output mode
}

// Turns on the laser by setting the pin high
void LaserTransmitter::turnOn() {
    digitalWrite(pin, HIGH);  // Set the pin high to turn on the laser
}

// Turns off the laser by setting the pin low
void LaserTransmitter::turnOff() {
    digitalWrite(pin, LOW);  // Set the pin low to turn off the laser
}
