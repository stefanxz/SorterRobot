#include "LaserReceiver.h"
#include <iostream>
#include <unistd.h>  // For usleep()

// Constructor to initialize the LaserReceiver with a specific pin
LaserReceiver::LaserReceiver(int pin) : pin(pin) {}

// Initializes the laser receiver by setting the pin mode to INPUT
void LaserReceiver::init() {
    pinMode(this->pin, INPUT); // Set the specified pin to input mode
}

// Checks if the laser is detected by reading the pin value
bool LaserReceiver::isLaserDetected() const {
    return digitalRead(this->pin) == HIGH;
}

// Continuously checks for laser detection and identifies if an object is stuck
int LaserReceiver::checkLaserDetection(int stuckThreshold) {
    int detectionTime = 0;  // Variable to track the time when laser is not detected

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
