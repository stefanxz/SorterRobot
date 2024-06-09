#include "LaserReceiver.h"
#include <iostream>
#include <unistd.h>  // For usleep()
#include <wiringPi.h> // Include wiringPi for GPIO functions

// Constructor
LaserReceiver::LaserReceiver(int pin) : pin(pin), detectionTime(std::chrono::steady_clock::time_point()) {}

// Initializes the sensor's GPIO pin mode
void LaserReceiver::init() {
    wiringPiSetupPhys();  // Initialize wiringPi using physical pin numbering
    pinMode(this->pin, INPUT); // Set the specified pin to input mode
}

// Returns true if laser is currently detected
bool LaserReceiver::isLaserDetected() const {
    return digitalRead(this->pin) == HIGH;
}

// Checks if the laser has been blocked for longer than the threshold, indicating something is stuck
int LaserReceiver::checkLaserDetection(int stuckThreshold) {
    if (!isLaserDetected()) {
        if (detectionTime == std::chrono::steady_clock::time_point()) {  // Start timing when laser first not detected
            detectionTime = std::chrono::steady_clock::now();
        } else {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - detectionTime).count();
            if (elapsedTime >= stuckThreshold) {
                detectionTime = std::chrono::steady_clock::time_point(); // Reset detection time
                return -1;  // Object stuck more than stuckThreshold milliseconds
            }
        }
    } else {
        if (detectionTime != std::chrono::steady_clock::time_point()) {  // Laser was previously not detected
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - detectionTime).count();
            detectionTime = std::chrono::steady_clock::time_point(); // Reset detection time
            if (elapsedTime < stuckThreshold) {
                return 1;  // Object not stuck or stuck less than stuckThreshold milliseconds
            }
        }
    }
    return 0;  // No conclusion reached, continue checking
}
