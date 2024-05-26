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

int motorIN1;
int motorIN2;
int motorEN;
int servoPIN;
int ADC_Adr;
int laserPIN;
int displayPIN;
int laserConfig;

int main() {

    SorterRobot sorterRobot(motorIN1, motorIN2, motorEN, servoPIN,  ADC_Adr, laserPIN, displayPIN);
    sorterRobot.system_init(laserConfig);
    return 0;
}