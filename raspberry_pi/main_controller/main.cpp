#include <iostream>
#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/Servo.h"
#include "../util/laser_sensor/LaserReceiver.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <unistd.h>

using namespace std;

// Function to check the laser detection status without outputting to the terminal
int checkLaserDetection(LaserReceiver& laserReceiver) {
    int detectionTime = 0;
    const int stuckThreshold = 4000; // 4 seconds in milliseconds

    while (true) {
        if (!laserReceiver.isLaserDetected()) {
            if (detectionTime == 0) {  // Start timing when laser first not detected
                detectionTime = millis();
            } else {
                int currentTime = millis();
                int elapsedTime = currentTime - detectionTime;
                if (elapsedTime >= stuckThreshold) {
                    return -1;  // Object stuck more than 4 seconds
                }
            }
        } else {
            if (detectionTime != 0) {  // Laser was previously not detected
                int currentTime = millis();
                int elapsedTime = currentTime - detectionTime;
                if (elapsedTime < stuckThreshold) {
                    return 1;  // Object not stuck or stuck less than 4 seconds
                }
            }
            detectionTime = 0;  // Reset detection time
        }
        usleep(100000);  // Check every 0.1 seconds
    }
}

int main() {
    LaserReceiver laserReceiver(16);  // Assuming the pin is 7
    laserReceiver.init();

    // Call the function to check laser detection
    int result = checkLaserDetection(laserReceiver);

    // Optionally use result for further logic
    if (result == -1) { std::cout << "Object is stuck in filter"; }
    else {std::cout << "Object passed through filter successfully"; } 

    return 0;
}