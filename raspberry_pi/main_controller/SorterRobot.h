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
#include "../util/laser_transmitter/LaserTransmitter.h"

class SorterRobot {
public:
    SorterRobot(int motorIN1, int motorIN2, int motorEN,
                int servoPIN, int adcAddress, int displayAddress, int laserReceiverHeightPIN,
                int laserReceiverWidthPIN, int laserReceiverCarDetectionPIN, int laserTransmitterBlackPIN,
                int laserTransmitterWhitePIN, int laserTransmitterColorPIN
    );

    MotorController &getMotorController();

    ServoController &getServoController();

    ADCReader &getAdcReader();

    DisplayController &getDisplayController();

    LaserReceiver &getLaserReceiverHeight();

    LaserReceiver &getLaserReceiverWidth();

    LaserReceiver &getLaserReceiverCarDetection();

    LaserTransmitter &getLaserTransmitterBlack();

    LaserTransmitter &getLaserTransmitterWhite();

    LaserTransmitter &getLaserTransmitterColor();

	void incrementDisksInTube();

    void decrementDisksInTube();

    int getDisksInTube() const;

    void robotSetup(int adcConfig);

private:
    MotorController motorController;
    ServoController servoController;
    ADCReader adcReader;
    DisplayController displayController;
    LaserReceiver laserReceiverHeight;
    LaserReceiver laserReceiverWidth;
    LaserReceiver laserReceiverCarDetection;
    LaserTransmitter laserTransmitterBlack;
    LaserTransmitter laserTransmitterWhite;
    LaserTransmitter laserTransmitterColor;

    int motorIN1;
    int motorIN2;
    int motorEN;
    int servoPIN;
    int adcAddress;
    int displayAddress;
    int laserReceiverHeightPIN;
    int laserReceiverWidthPIN;
    int laserReceiverCarDetectionPIN;
    int laserTransmitterBlackPIN;
    int laserTransmitterWhitePIN;
    int laserTransmitterColorPIN;
    int disksInTube;

};

#endif //SORTERROBOT_SORTERROBOT_H
