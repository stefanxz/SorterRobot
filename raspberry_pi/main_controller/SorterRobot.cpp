
#include "SorterRobot.h"


SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN, int servoPIN, int adcAddress, int displayAddress,
                         int laserReceiverHeightPIN, int laserReceiverWidthPIN, int laserReceiverCarDetectionPIN,
                         int laserTransmitterBlackPIN, int laserTransmitterWhitePIN, int laserTransmitterColorPIN)
        : motorIN1(motorIN1), motorIN2(motorIN2), motorEN(motorEN),
          servoPIN(servoPIN), adcAddress(adcAddress),
          laserReceiverHeightPIN(laserReceiverHeightPIN), laserReceiverWidthPIN(laserReceiverWidthPIN),
          laserReceiverCarDetectionPIN(laserReceiverCarDetectionPIN),
          laserTransmitterBlackPIN(laserTransmitterBlackPIN), laserTransmitterWhitePIN(laserTransmitterWhitePIN),
          laserTransmitterColorPIN(laserTransmitterColorPIN), displayAddress(displayAddress),
          motorController(motorIN1, motorIN2, motorEN),
          servoController(servoPIN),
          adcReader(adcAddress),
          displayController(displayAddress),
          laserReceiverHeight(laserReceiverHeightPIN), laserReceiverWidth(laserReceiverWidthPIN),
          laserReceiverCarDetection(laserReceiverCarDetectionPIN), laserTransmitterBlack(laserTransmitterBlackPIN),
          laserTransmitterWhite(laserTransmitterWhitePIN), laserTransmitterColor(laserTransmitterColorPIN) {
    std::cout << "MotorController initiated with IN1: " << motorIN1 << ", IN2: " << motorIN2 << ", EN: " << motorEN
              << std::endl;
    std::cout << "ServoController initiated with servoPIN: " << servoPIN << std::endl;
    std::cout << "ADCReader initiated with adcAddress: " << adcAddress << std::endl;
    std::cout << "DisplayController initiated with displayAddress: " << displayAddress << std::endl;
    std::cout << "LaserReceiverHeight initiated with laserReceiverHeightPIN: " << std::endl;
    std::cout << "LaserReceiverWidth initiated with laserReceiverWidthPIN: " << std::endl;
    std::cout << "LaserReceiverCarDetection initiated with laserReceiverCarDetectionPIN: "
              << laserReceiverCarDetectionPIN << std::endl;
    std::cout << "LaserTransmitterBlack initiated with laserTransmitterBlackPIN: " << laserTransmitterBlackPIN
              << std::endl;
    std::cout << "LaserTransmitterWhite initiated with laserTransmitterWhitePIN: " << laserTransmitterWhitePIN
              << std::endl;
    std::cout << "LaserTransmitterColor initiated with laserTransmitterColorPIN: " << laserTransmitterColorPIN
              << std::endl;
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

DisplayController &SorterRobot::getDisplayController() {
    return displayController;
}

LaserReceiver &SorterRobot::getLaserReceiverCarDetection() {
    return laserReceiverCarDetection;
}

LaserReceiver &SorterRobot::getLaserReceiverWidth() {
    return laserReceiverWidth;
}

LaserReceiver &SorterRobot::getLaserReceiverHeight() {
    return laserReceiverHeight;
}

LaserTransmitter &SorterRobot::getLaserTransmitterBlack() {
    return laserTransmitterBlack;
}

LaserTransmitter &SorterRobot::getLaserTransmitterWhite() {
    return laserTransmitterWhite;
}

LaserTransmitter &SorterRobot::getLaserTransmitterColor() {
    return laserTransmitterColor;
}


void SorterRobot::robotSetup(int adcConfig) {
    // Initializing conveyor belt motors
    if (motorIN1 != -1 && motorIN2 != -1 && motorEN != -1) {
        motorController.setup();
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






