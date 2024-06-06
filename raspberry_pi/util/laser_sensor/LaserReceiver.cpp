#include "LaserReceiver.h"
#include <iostream>
#include <unistd.h>  // For usleep()

LaserReceiver::LaserReceiver(int pin) : pin(pin) {}

void LaserReceiver::init() {
    wiringPiSetupPhys();  // Initialize wiringPi using physical pin numbering
    pinMode(this->pin, INPUT); // Set the specified pin to input mode
}

bool LaserReceiver::isLaserDetected() const {
    return digitalRead(this->pin) == HIGH;
}

int LaserReceiver::checkLaserDetection(int stuckThreshold) {
    int detectionTime = 0;

    while (true) {
        if (!isLaserDetected()) {
            if (detectionTime == 0) {  // Start timing when laser first not detected
                detectionTime = millis();
            } else {
                int currentTime = millis();
                int elapsedTime = currentTime - detectionTime;
                if (elapsedTime >= stuckThreshold) {
                    return -1;  // Object stuck more than stuckThreshold milliseconds
                }
            }
        } else {
            if (detectionTime != 0) {  // Laser was previously not detected
                int currentTime = millis();
                int elapsedTime = currentTime - detectionTime;
                if (elapsedTime < stuckThreshold) {
                    return 1;  // Object not stuck or stuck less than stuckThreshold milliseconds
                }
            }
            detectionTime = 0;  // Reset detection time
        }
        usleep(100000);  // Check every 0.1 seconds
    }
}


