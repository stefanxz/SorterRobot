//
// Created by Stefan on 24.05.2024.
//
#include <iostream>
#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/ServoController.h"
#include "../util/laser_sensor/LaserReceiver.h"
#include "../util/laser_transmitter/LaserTransmitter.h"
#include "SorterRobot.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <unistd.h>

void system_init() {
    std::cout << "Starting system initialization." << std::endl;
    if (wiringPiSetupPhys() == -1) {
        std::cerr << "Error setting up wiringPi. Initialization failed." << std::endl;
        return;
    }
    std::cout << "System init done!" << '\n';
}

int motorIN1 = 13;
int motorIN2 = 15;
int motorEN = 11;
int servoPIN = 37;
int adcAddress = 0x48;
int displayAddress = -1;
int laserReceiverHeightPIN = 29;
int laserReceiverWidthPIN = -1;
int laserReceiverCarDetectionPIN = -1;
int laserTransmitterBlackPIN = 31;
int laserTransmitterWhitePIN = 32;
int laserTransmitterColorPIN = 33;
int adcConfig = 0xC183;

#include "SorterRobot.h"

int main() {
    system_init();
    SorterRobot sorterRobot(motorIN1, motorIN2, motorEN, servoPIN, adcAddress, displayAddress, laserReceiverHeightPIN,
                            laserReceiverWidthPIN, laserReceiverCarDetectionPIN, laserTransmitterBlackPIN,
                            laserTransmitterWhitePIN, laserTransmitterColorPIN);
    sorterRobot.robotSetup(adcConfig);
    while(true) { 
    bool result = sorterRobot.getLaserReceiverHeight().isLaserDetected();
    std::cout << "Laser is " << (result ? "detected" : "not detected") << std::endl;
    }
    
    return 0;
}