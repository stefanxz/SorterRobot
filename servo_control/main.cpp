// main.cpp
#include "Servo.h"

int main() {
    int pin = 11; // Physical pin 11 corresponds to WiringPi pin 0
    Servo servo(pin);
    for(int i = 1; i <= 5; i ++ )
        servo.movePiston(1000000+500000); // 1 Second in microseconds.
    return 0;
}