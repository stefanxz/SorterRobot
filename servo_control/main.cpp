// main.cpp
#include "Servo.h"

int main() {
    int pin = 11; // Physical pin 11 corresponds to WiringPi pin 0
    Servo servo(pin);
    servo.movePiston(1000000); // 1 Second in microseconds.
    return 0;
}