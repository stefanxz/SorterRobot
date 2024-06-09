#ifndef LASER_RECEIVER_H
#define LASER_RECEIVER_H

#include <chrono>

class LaserReceiver {
private:
    int pin;  // GPIO pin number associated with the laser sensor
    std::chrono::steady_clock::time_point detectionTime;  // To keep track of the time when laser is not detected

public:
    // Constructor initializes the laser sensor with the provided GPIO pin number
    explicit LaserReceiver(int pin);

    // Initializes the sensor's GPIO pin mode
    void init();

    // Returns true if laser is currently detected
    bool isLaserDetected() const;

    // Checks if the laser has been blocked for longer than the threshold, indicating something is stuck
    int checkLaserDetection(int stuckThreshold);
};

#endif // LASER_RECEIVER_H
