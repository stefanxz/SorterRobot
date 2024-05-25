#include "ColorDisplaySystem.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int main() {
    ColorDisplaySystem colorSystem(0x48, 0x27); // ADC and display I2C addresses
    ColorDisplaySystem.setup();
    ColorDisplaySystem.run();
    return 0;
}
