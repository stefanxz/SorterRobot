// main.cpp
#include "Servo.h"

int main() {
    int pin = 11; // Physical pin 11 corresponds to WiringPi pin 0
    Servo servo(pin);

    for(;;){
        for (int i = 0; i <= 180; i+=45)
        {
            servo.moveToAngle(i);
        }

        for (int i = 135; i >= 0; i-=45)
        {
            servo.moveToAngle(i);
        }
    }
    
    

    return 0;
}