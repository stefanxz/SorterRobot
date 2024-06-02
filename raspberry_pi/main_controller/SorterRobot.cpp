
#include "SorterRobot.h"

SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN,
                         int servoPIN, int adcAddress, int laserPIN, int displayAddress)
        : motorIN1(motorIN1), motorIN2(motorIN2), motorEN(motorEN),
          servoPIN(servoPIN), adcAddress(adcAddress),
          laserPIN(laserPIN), displayAddress(displayAddress),
          motorController(motorIN1, motorIN2, motorEN),
          servoController(servoPIN),
          adcReader(adcAddress),
          laserReceiver(laserPIN),
          displayController(displayAddress) {
    std::cout << "SorterRobot constructor called with:" << std::endl;
    std::cout << "servoPIN: " << servoPIN << std::endl;
    std::cout << "adcAddress: " << adcAddress << std::endl;
    std::cout << "laserPIN: " << laserPIN << std::endl;
    std::cout << "displayAddress: " << displayAddress << std::endl;
}

MotorController &SorterRobot::getMotorController() {
    return motorController;
}

ServoController &SorterRobot::getServoController() {
    return servoController;
}

ADCReader &SorterRobot::getAdcReader() {
    return adcReader;
}

LaserReceiver &SorterRobot::getLaserReceiver() {
    return laserReceiver;
}

DisplayController &SorterRobot::getDisplayController() {
    return displayController;
}

void SorterRobot::movePiston() {
    servoController.movePiston();
}

void SorterRobot::robotSetup(int adcConfig) {
    if (motorIN1 != -1 && motorIN2 != -1 && motorEN != -1) {
        motorController.setup();
        std::cout << "Conveyor Belt initialized" << '\n';
    }
    if (laserPIN != -1) {
        laserReceiver.init();
        std::cout << "Laser sensor initialized" << '\n';
    }
    if (adcAddress != -1) {
        adcReader.initADC(adcConfig);
        std::cout << "Light(color) sensor initialized" << '\n';
    }
    if (displayAddress != -1) {
        displayController.displayInit();
        std::cout << "Display initialized" << '\n';
    }
}

