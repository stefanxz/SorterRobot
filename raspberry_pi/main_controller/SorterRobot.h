//
// Created by Stefan on 26.05.2024.
//
#ifndef SORTERROBOT_SORTERROBOT_H
#define SORTERROBOT_SORTERROBOT_H

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include "SorterRobot.h"
#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/ServoController.h"
#include "../util/laser_sensor/LaserReceiver.h"

class SorterRobot {
public:
    SorterRobot(int motorIN1, int motorIN2, int motorEN,
                int servoPIN, int ADC_Adr, int laserPIN, int displayPIN);

    void system_init(int laserConfig);

    void movePiston();

    void runConveyorBelt(bool direction);

    void stopConveyorBelt();

    void clearDisplay();

    void displayString(const char *str);

    bool isLaserDetected() const;


private:
    MotorController motorController;
    ServoController servoController;
    ADCReader adcReader;
    LaserReceiver laserReceiver;
    DisplayController displayController;

    int motorIN1;
    int motorIN2;
    int motorEN;
    int servoPIN;
    int ADC_Adr;
    int ADC_fd;
    int laserPIN;
    int displayPIN;
};

#endif //SORTERROBOT_SORTERROBOT_H
