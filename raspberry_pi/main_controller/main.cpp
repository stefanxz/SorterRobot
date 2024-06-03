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

    LaserReceiver receiverHeightFilter(16);  // Assuming the pin is 16
    LaserReceiver receiverSlope(18);         // Assuming the pin is 18
    LaserReceiver receiverEndConveyor(11);   // Assuming the pin is 11

    // Initialize the laser receivers
    receiverHeightFilter.init();
    receiverSlope.init();
    receiverEndConveyor.init();

    int stuckThreshold = 4000;  // Common threshold for all receivers

    // Check laser detection for the height filter
    int resultHeightFilter = checkLaserDetection(receiverHeightFilter, stuckThreshold);
    if (resultHeightFilter == -1) {
        cout << "Object is stuck in filter" << endl;
    } else {
        cout << "Object passed through filter successfully" << endl;
    }

    // Check laser detection for the slope
    int resultSlope = checkLaserDetection(receiverSlope, stuckThreshold);
    if (resultSlope == -1) {
        cout << "Object is stuck on the slope" << endl;
    } else {
        cout << "Object passed through slope successfully" << endl;
    }

    // Check if the object has reached the end of the conveyor belt
    int resultEndConveyor = checkLaserDetection(receiverEndConveyor, stuckThreshold);
    if (resultEndConveyor == -1) {
        cout << "Object is waiting at the end of the conveyor belt" << endl;
    } else {
        cout << "Something went wrong" << endl;
    }

    // Initialize the laser transmitters for the three gates
    LaserTransmitter laserGateWhite(29); // Assuming the pin is 29
    LaserTransmitter laserGateBlack(31); // Assuming the pin is 31
    LaserTransmitter laserGateOther(37); // Assuming the pin is 37

    // Initialize the laser transmitters
    laserGateWhite.init();
    laserGateBlack.init();
    laserGateOther.init();

    // EXAMPLE CODE - Turn on the laser for the white gate 
    int gate = 1;
    if (gate == 1) {
        laserGateWhite.turnOn();
        laserGateBlack.turnOff();
        laserGateOther.turnOff();
    }

    return 0;
}
