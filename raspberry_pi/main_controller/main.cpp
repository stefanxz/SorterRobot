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

int main() {
    // Laser receiver setup
    LaserReceiver laserReceiver(16);  // Assuming the pin is 16 for this example
    laserReceiver.init();

    const int noLaserDetectionThreshold = 4000; // Threshold for laser non-detection in milliseconds
    const int runLaserLoopDelay = 1; // Delay in seconds for the laser detection Loop
    int detectionTime = millis();  // Initialize detection time to current time

    while (true) {
        if (!laserReceiver.isLaserDetected()) {
            int currentTime = millis();
            int elapsedTime = currentTime - detectionTime;
            if (elapsedTime >= noLaserDetectionThreshold) {
                cout << "Unfit object detected" << endl;
                detectionTime = currentTime; // Reset detection time
            }
        } else {
            detectionTime = millis(); // Reset the timer when laser is detected
        }
        sleep(runLaserLoopDelay);  // Pause the loop for runLaserLoopDelay seconds before next check
    }

    

    return 0;
}
