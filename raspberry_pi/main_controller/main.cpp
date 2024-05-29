//
// Created by Stefan on 24.05.2024.
//
#include <iostream>
#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/ServoController.h"
#include "../util/laser_sensor/LaserReceiver.h"
#include "SorterRobot.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <unistd.h>

using namespace std;

int motorIN1 = -1;
int motorIN2 = -1;
int motorEN = -1;
int servoPIN = 11;
int adcAddress = -1;
int laserPIN = -1;
int displayAddress = -1;
int adcConfig = 0;

int main() {
    SorterRobot sorterRobot(motorIN1, motorIN2, motorEN, servoPIN, adcAddress, laserPIN, displayAddress);
    sorterRobot.system_init(adcConfig = 0);
    sorterRobot.getServoController().movePiston();
    return 0;
}