#include "SorterRobot.h"
#include <iostream>
#include <sstream>

SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN, int firstConveyorIN1, int firstConveyorIN2,
                         int firstConveyorEN, int servoPIN,
                         int adcAddress, int displayAddress,
                         int laserReceiverHeightPIN, int laserReceiverWidthPIN, int laserReceiverCarDetectionPIN,
                         int laserTransmitterBlackPIN, int laserTransmitterWhitePIN, int laserTransmitterColorPIN,
                         const std::string &carControllerBaseUrl)
        : motorIN1(motorIN1), motorIN2(motorIN2), motorEN(motorEN),
          firstConveyorIN1(firstConveyorIN1), firstConveyorIN2(firstConveyorEN), firstConveyorEN(firstConveyorEN),
          servoPIN(servoPIN), adcAddress(adcAddress),
          laserReceiverHeightPIN(laserReceiverHeightPIN), laserReceiverWidthPIN(laserReceiverWidthPIN),
          laserReceiverCarDetectionPIN(laserReceiverCarDetectionPIN),
          laserTransmitterBlackPIN(laserTransmitterBlackPIN), laserTransmitterWhitePIN(laserTransmitterWhitePIN),
          laserTransmitterColorPIN(laserTransmitterColorPIN), displayAddress(displayAddress),
          motorController(motorIN1, motorIN2, motorEN),
          firstConveyor(firstConveyorIN1, firstConveyorIN2, firstConveyorEN),
          servoController(servoPIN),
          adcReader(adcAddress),
          displayController(displayAddress),
          laserReceiverHeight(laserReceiverHeightPIN), laserReceiverWidth(laserReceiverWidthPIN),
          laserReceiverCarDetection(laserReceiverCarDetectionPIN), laserTransmitterBlack(laserTransmitterBlackPIN),
          laserTransmitterWhite(laserTransmitterWhitePIN), laserTransmitterColor(laserTransmitterColorPIN),
          carController(carControllerBaseUrl) {
    // Initialization debug messages...
}

MotorController &SorterRobot::getMotorController() { return motorController; }

MotorController &SorterRobot::getFirstConveyor() { return firstConveyor; }

ServoController &SorterRobot::getServoController() { return servoController; }

ADCReader &SorterRobot::getAdcReader() { return adcReader; }

DisplayController &SorterRobot::getDisplayController() { return displayController; }

LaserReceiver &SorterRobot::getLaserReceiverCarDetection() { return laserReceiverCarDetection; }

LaserReceiver &SorterRobot::getLaserReceiverWidth() { return laserReceiverWidth; }

LaserReceiver &SorterRobot::getLaserReceiverHeight() { return laserReceiverHeight; }

LaserTransmitter &SorterRobot::getLaserTransmitterBlack() { return laserTransmitterBlack; }

LaserTransmitter &SorterRobot::getLaserTransmitterWhite() { return laserTransmitterWhite; }

LaserTransmitter &SorterRobot::getLaserTransmitterColor() { return laserTransmitterColor; }

CarController &SorterRobot::getCarController() { return carController; }

void SorterRobot::incrementDisksInTube() { disksInTube++; }

void SorterRobot::decrementDisksInTube() { if (disksInTube > 0) disksInTube--; }

int SorterRobot::getDisksInTube() const { return disksInTube; }


void SorterRobot::robotSetup(int adcConfig) {
    // Initializing conveyor belt motors
    if (motorIN1 != -1 && motorIN2 != -1 && motorEN != -1) {
        motorController.setup();
        std::cout << "Second conveyor Belt initialized" << '\n';
    }

    if (firstConveyorIN1 != -1 && firstConveyorIN2 != -1 && firstConveyorEN != -1) {
        firstConveyor.setup();
        firstConveyor.slowerDaddy(1);
        firstConveyor.run(true);
        std::cout << "Conveyor Belt initialized" << '\n';
    }

    if (laserReceiverCarDetectionPIN != -1) {
        laserReceiverCarDetection.init();
        std::cout << "Laser sensor for CAR DETECTION initialized" << '\n';
    }
    if (laserReceiverWidthPIN != -1) {
        laserReceiverWidth.init();
        std::cout << "Laser sensor for WIDTH FILTER initialized" << '\n';
    }
    if (laserReceiverHeightPIN != -1) {
        laserReceiverHeight.init();
        std::cout << "Laser sensor for HEIGHT FILTER initialized" << '\n';
    }

    if (laserTransmitterBlackPIN != -1) {
        laserTransmitterBlack.init();
        std::cout << "Laser transmitter for BLACK COLOR GATE  initialized" << '\n';
    }
    if (laserTransmitterWhitePIN != -1) {
        laserTransmitterWhite.init();
        std::cout << "Laser transmitter for WHITE COLOR GATE  initialized" << '\n';
    }
    if (laserTransmitterColorPIN != -1) {
        laserTransmitterColor.init();
        std::cout << "Laser transmitter for COLOR DETECTION GATE initialized" << '\n';
    }

    if (adcAddress != -1) {
        adcReader.initADC(adcConfig);
        std::cout << "ADC Reader for LIGHT/COLOR sensor initialized" << '\n';
    }

    if (displayAddress != -1) {
        displayController.displayInit();
        std::cout << "Display initialized" << '\n';
    }

    std::cout << "Robot setup complete." << '\n';

}


