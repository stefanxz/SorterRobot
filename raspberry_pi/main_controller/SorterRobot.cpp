
#include "SorterRobot.h"

SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN,
                         int servoPIN, int adcAddress, int laserPIN, int displayAddress)
        : motorController(motorIN1, motorIN2, motorEN), servoController(servoPIN),
          adcReader(adcAddress), laserReceiver(laserPIN), displayController(displayAddress),
          motorIN1(motorIN1), motorIN2(motorIN2), motorEN(motorEN),
          servoPIN(servoPIN), adcAddress(adcAddress),
          laserPIN(laserPIN), displayAddress(displayAddress) {
}


void SorterRobot::system_init(int lightSensorConfig) {
    if (wiringPiSetup() == -1) {
        std::cerr << "Error setting up wiringPi\n";
        return;
    }

    SorterRobot::setupComponents(lightSensorConfig);
}


MotorController SorterRobot::getMotorController() const {
    return motorController;
}

ServoController SorterRobot::getServoController() const {
    return servoController;
}

ADCReader SorterRobot::getAdcReader() const {
    return adcReader;
}

LaserReceiver SorterRobot::getLaserReceiver() const {
    return laserReceiver;
}

DisplayController SorterRobot::getDisplayController() const {
    return displayController;
}

void SorterRobot::setupComponents(int lightSensorConfig) {
    if (motorIN1 != -1 && motorIN2 != -1 && motorEN != -1) {
        motorController.setup();
    }
    if (laserPIN != -1) {
        laserReceiver.init();
    }
    if (ADC_Adr != -1) {
        adcReader.initADC(lightSensorConfig);
    }
    if (displayAddress != -1) {
        displayController.displayInit();
    }
}

