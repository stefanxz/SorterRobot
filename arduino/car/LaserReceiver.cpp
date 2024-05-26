#include "LaserReceiver.h"

LaserReceiver::LaserReceiver(int pin) : pin_(pin) {}

void LaserReceiver::init()
{
    pinMode(pin_, INPUT); // Set the pin as input
}

bool LaserReceiver::isLaserDetected() const
{
    return digitalRead(pin_) == HIGH; // Return true if the pin reads HIGH
}
