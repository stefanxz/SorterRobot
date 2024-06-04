#include "LaserReceiver.h"
#include <iostream>

LaserReceiver::LaserReceiver(int pin) : pin(pin) {}


void LaserReceiver::init() {
    if (pin != -1) { pinMode(pin, INPUT); } // Set the specified pin to input mode
}

bool LaserReceiver::isLaserDetected() const
{
    return digitalRead(pin) == HIGH;
}
