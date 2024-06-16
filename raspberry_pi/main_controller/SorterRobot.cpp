#include "SorterRobot.h"

// Constructor to initialize all components of the sorter robot
SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN,
                         int firstConveyorIN1, int firstConveyorIN2, int firstConveyorEN,
                         int servoPIN, int adcAddress, int displayAddress, int laserReceiverHeightPIN,
                         int laserReceiverWidthPIN, int laserReceiverCarDetectionPIN, int laserTransmitterBlackPIN,
                         int laserTransmitterWhitePIN, int laserTransmitterColorPIN,
                         const std::string &carControllerBaseUrl)
        : motorController(motorIN1, motorIN2, motorEN),
          firstConveyor(firstConveyorIN1, firstConveyorIN2, firstConveyorEN),
          servoController(servoPIN), adcReader(adcAddress), displayController(displayAddress),
          laserReceiverHeight(laserReceiverHeightPIN), laserReceiverWidth(laserReceiverWidthPIN),
          laserReceiverCarDetection(laserReceiverCarDetectionPIN), laserTransmitterBlack(laserTransmitterBlackPIN),
          laserTransmitterWhite(laserTransmitterWhitePIN), laserTransmitterColor(laserTransmitterColorPIN),
          carController(carControllerBaseUrl) {
    // Initialize the state to IDLE
    currentState = IDLE;
}

// Accessor methods for various controllers

MotorController &SorterRobot::getMotorController() {
    return motorController;
}

MotorController &SorterRobot::getFirstConveyor() {
    return firstConveyor;
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

LaserReceiver &SorterRobot::getLaserReceiverHeight() {
    return laserReceiverHeight;
}

LaserReceiver &SorterRobot::getLaserReceiverWidth() {
    return laserReceiverWidth;
}

LaserReceiver &SorterRobot::getLaserReceiverCarDetection() {
    return laserReceiverCarDetection;
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

CarController &SorterRobot::getCarController() {
    return carController;
}

void SorterRobot::incrementDisksInTube() {
    disksInTube++;
}

void SorterRobot::decrementDisksInTube() {
    if (disksInTube > 0) {
        disksInTube--;
    }
}

int SorterRobot::getDisksInTube() const {
    return disksInTube;
}

void SorterRobot::robotSetup(int adcConfig) {
    firstConveyor.setup();
    motorController.setup();
    displayController.displayInit();
    adcReader.initADC(adcConfig);
    firstConveyor.slowerDaddy(50);
    firstConveyor.run(true);
}

void SorterRobot::handleHeightLaser(unsigned long currentTime) {
    bool heightLaserDetected = getLaserReceiverHeight().isLaserDetected();
    if (!laserHeightBlocked && !heightLaserDetected && !objectStuckAtHeight) {
        laserHeightBlocked = true;
        laserHeightBlockedTime = currentTime;
        std::cout << "Height Laser Blocked at time: " << currentTime << std::endl;
        currentState = HEIGHT_LASER_BLOCKED;
    } else if (laserHeightBlocked && heightLaserDetected) {
        laserHeightBlocked = false;
        objectStuckAtHeight = false;
        stuckMessageDisplayedAtHeight = false;
        std::cout << "Height Laser Cleared at time: " << currentTime << std::endl;
        getDisplayController().displayClear();
        currentState = IDLE;
    } else if (laserHeightBlocked && !heightLaserDetected &&
               currentTime - laserHeightBlockedTime >= stuckThreshold && !stuckMessageDisplayedAtHeight) {
        getDisplayController().displayString("Object Stuck at Height Laser");
        objectStuckAtHeight = true;
        stuckMessageDisplayedAtHeight = true;
        std::cout << "Object Stuck at Height Laser at time: " << currentTime << std::endl;
    }
}

void SorterRobot::handleWidthLaser(unsigned long currentTime) {
    bool widthLaserDetected = getLaserReceiverWidth().isLaserDetected();
    if (!laserWidthBlocked && !widthLaserDetected && !objectStuckAtWidth) {
        laserWidthBlocked = true;
        laserWidthBlockedTime = currentTime;
        forcedClear = false;
        std::cout << "Width Laser Blocked at time: " << currentTime << std::endl;
        currentState = WIDTH_LASER_BLOCKED;
    } else if (laserWidthBlocked && widthLaserDetected) {
        if (!objectStuckAtWidth && !forcedClear) {
            incrementDisksInTube();
            diskFalling = true;
            diskFallStartTime = currentTime;
            diskPassedWidthFilter = true;
            std::cout << "Disk falling initiated at time: " << currentTime << std::endl;
            currentState = DISK_FALLING;
        }
        laserWidthBlocked = false;
        objectStuckAtWidth = false;
        stuckMessageDisplayedAtWidth = false;
        std::cout << "Width Laser Cleared at time: " << currentTime << std::endl;
        getDisplayController().displayClear();
    } else if (laserWidthBlocked && !widthLaserDetected && currentTime - laserWidthBlockedTime >= stuckThreshold &&
               !stuckMessageDisplayedAtWidth) {
        getDisplayController().displayString("Object Stuck at Width Laser");
        objectStuckAtWidth = true;
        stuckMessageDisplayedAtWidth = true;
        forcedClear = true;
        std::cout << "Object Stuck at Width Laser at time: " << currentTime << std::endl;
    }
}

void SorterRobot::handleDiskSettling(unsigned long currentTime) {
    if (diskFalling && currentTime - diskFallStartTime >= diskFallTime) {
        diskFalling = false;
        std::cout << "Disk settled at time: " << currentTime << std::endl;
        if (!objectStuckAtWidth && !forcedClear) {
            colorSensingInProgress = true;
            colorReadings = 0;
            std::cout << "Color sensing initiated at time: " << currentTime << std::endl;
            currentState = COLOR_SENSING;
        }
    }
}

void SorterRobot::handleColorSensing(unsigned long currentTime) {
    if (colorSensingInProgress) {
        if (colorReadings < 1) {
            if (!colorDelayInProgress) {
                std::cout << "Performing color reading " << (colorReadings + 1) << std::endl;
                int colorValue = getAdcReader().readADCChannel(0xC183);
                std::string color = getAdcReader().detectColor(colorValue);
                std::cout << "Color value: " << colorValue << std::endl;
                gateNumber = getGateNumberFromColor(color);
                getDisplayController().displayDisk(color);
                colorReadings++;
                colorDelayInProgress = true;
                colorDelayStartTime = currentTime;
            } else if (currentTime - colorDelayStartTime >= colorDelayTime) {
                colorDelayInProgress = false;
            }
        } else if (!carReadyCheckInProgress) {
            carReadyCheckInProgress = true;
            carReadyCheckStartTime = currentTime;
        } else if (currentTime - carReadyCheckStartTime >= carReadyCheckInterval) {
            carReadyCheckStartTime = currentTime;
            if (getCarController().isCarReady()) {
                startConveyor = true;
                getMotorController().run(true);
                conveyorRunning = true;
                std::cout << "Conveyor started at time: " << currentTime << std::endl;
                carOccupied = true;
                carReadyCheckInProgress = false;
                colorSensingInProgress = false;
                currentState = PISTON_OPERATION;
            }
        }
    }
}

void SorterRobot::handlePistonOperation(unsigned long currentTime) {
    // Check if the conveyor has started and the piston operation is due
    if (startConveyor) {
        // Condition to push the piston
        if (!pushDone && !pullDone && currentTime - stopTime >= pistonTime) {
            decrementDisksInTube(); // Decrease the count of disks in the tube
            getServoController().pushPiston(); // Command the servo to push
            std::cout << "Piston pushed at time: " << currentTime << std::endl;
            pushTime = currentTime; // Set the time when the piston was pushed
            pushDone = true; // Mark that the push operation is done
            pullDone = false; // Reset pullDone for safety
            stopDone = false; // Reset stopDone to allow for subsequent operations
        }
            // Condition to pull the piston back after pushing
        else if (pushDone && !pullDone && currentTime - pushTime >= pistonTime + 65) {
            getServoController().pullPiston(); // Command the servo to pull back
            std::cout << "Piston pulled at time: " << currentTime << std::endl;
            pullTime = currentTime; // Set the time when the piston was pulled
            pullDone = true; // Mark that the pull operation is done
        }
            // Condition to stop the piston after pulling
        else if (pushDone && pullDone && !stopDone && currentTime - pullTime >= pistonTime + 65) {
            getServoController().stopPiston(); // Command the servo to stop
            std::cout << "Piston stopped at time: " << currentTime << std::endl;
            stopTime = currentTime; // Set the time when the piston was stopped
            stopDone = true; // Mark that the stop operation is done
            pushDone = false; // Reset pushDone for the next cycle
            pullDone = false; // Reset pullDone for the next cycle
            startConveyor = false; // Stop the conveyor as the piston cycle is complete
            checkDiskPassed = true; // Set to check if the disk has passed the next sensor
            diskTimeoutInProgress = true; // Start the timeout monitoring
            diskTimeoutStartTime = currentTime; // Initialize the timeout start time
            currentState = CHECK_CAR_DETECTION; // Change state to check car detection
        }
    }
}


void SorterRobot::handleCarDetectionLaser(unsigned long currentTime) {
    bool carDetectionLaserDetected = getLaserReceiverCarDetection().isLaserDetected();
    if (!carDetectionLaserBlocked && !carDetectionLaserDetected) {
        if (diskPassedWidthFilter) {
            std::cout << "Car Detection Laser Blocked at time: " << currentTime << std::endl;
            carDetectionLaserBlocked = true;
            laserCarDetectionBlockedTime = currentTime;
        }
    } else if (carDetectionLaserBlocked && carDetectionLaserDetected) {
        std::cout << "Car Detection Laser Cleared at time: " << currentTime << std::endl;
        carDetectionLaserBlocked = false;
        carDetectionLaserCleared = true;
        diskPassedWidthFilter = false;

        if (!driveRequestSent) {
            getCarController().drive(gateNumber);
            driveRequestSent = true;
            std::cout << "Drive request sent at time: " << currentTime << std::endl;
        }

        currentState = RESET_AFTER_DRIVING;
    }
}

void SorterRobot::handleDiskTimeout(unsigned long currentTime) {
    if (diskTimeoutInProgress && (currentTime - diskTimeoutStartTime >= diskTimeoutThreshold)) {
        std::cout << "Disk did not make it to the conveyor belt at time: " << currentTime << std::endl;
        getDisplayController().displayString("Disk did not make it to the conveyor belt");
        diskTimeoutInProgress = false;
        checkDiskPassed = false;
        carOccupied = false;
        carReadyCheckInProgress = false;
        driveRequestSent = false;
        std::cout << "Disk timeout reset at time: " << currentTime << std::endl;
        if (getDisksInTube() > 0) {
            std::cout << "Starting next cycle" << std::endl;
            colorSensingInProgress = true;
            colorReadings = 0;
            currentState = COLOR_SENSING;
        } else {
            currentState = IDLE;
        }
    }
}

void SorterRobot::resetAfterDriving(unsigned long currentTime) {
    if (driveRequestSent && !carDetectionLaserBlocked) {
        std::cout << "Disk passed car detection laser at time: " << currentTime << std::endl;
        driveRequestSent = false;
        carOccupied = false;
        carDetectionLaserCleared = false;
        carReadyCheckInProgress = false;
        waitingForCarToPass = false;
        diskTimeoutInProgress = false;
        if (getDisksInTube() > 0) {
            std::cout << "Starting next cycle" << std::endl;
            colorSensingInProgress = true;
            colorReadings = 0;
            currentState = COLOR_SENSING;
        } else {
            currentState = IDLE;
        }
    }
}

void SorterRobot::run() {
    while (true) {
        unsigned long currentTime = millis();

        handleHeightLaser(currentTime);
        handleWidthLaser(currentTime);
        handleDiskSettling(currentTime);
        handleColorSensing(currentTime);
        handlePistonOperation(currentTime);
        handleCarDetectionLaser(currentTime);
        handleDiskTimeout(currentTime);
        resetAfterDriving(currentTime);

        usleep(10000);
    }
}

void SorterRobot::handleGateLasers(int gateNumber) {
    switch (gateNumber) {
        case 1:
            getLaserTransmitterBlack().turnOff();
            getLaserTransmitterColor().turnOff();
            getLaserTransmitterWhite().turnOn();
            break;
        case 2:
            getLaserTransmitterWhite().turnOff();
            getLaserTransmitterColor().turnOff();
            getLaserTransmitterBlack().turnOn();
            break;
        case 3:
            getLaserTransmitterBlack().turnOff();
            getLaserTransmitterWhite().turnOff();
            getLaserTransmitterColor().turnOn();
            break;
    }
}

int SorterRobot::getGateNumberFromColor(const std::string &color) {
    if (color == "white") {
        return 1;
    } else if (color == "black") {
        return 2;
    } else {
        return 3;
    }
}

void SorterRobot::testPistonOperation() {
    unsigned long currentTime, lastActionTime = 0;
    int action = 0; // Action to be taken (1 = Push, 2 = Pull, 3 = Stop)

    std::cout << "Enter 1 to start piston test sequence, or 0 to exit:" << std::endl;
    std::cin >> action;

    while (action != 0) {
        currentTime = millis(); // Get current time in milliseconds

        switch (action) {
            case 1: // Push the piston
                if (!pushDone && !pullDone && currentTime - lastActionTime >= pistonTime) {
                    //getServoController().pushPiston();
                    softPwmWrite(29, 20);
                    std::cout << "Piston pushed at time: " << currentTime << std::endl;
                    lastActionTime = currentTime;
                    pushDone = true;
                    pullDone = false;
                    stopDone = false;
                }
                break;
            case 2: // Pull the piston
                if (pushDone && !pullDone && currentTime - lastActionTime >= pistonTime + 65) {
                    //getServoController().pullPiston();
                    softPwmWrite(29, 10);
                    std::cout << "Piston pulled at time: " << currentTime << std::endl;
                    lastActionTime = currentTime;
                    pullDone = true;
                }
                break;
            case 3: // Stop the piston
                if (pushDone && pullDone && !stopDone && currentTime - lastActionTime >= pistonTime + 65) {
                    //getServoController().stopPiston();
                    softPwmWrite(29, 0);
                    std::cout << "Piston stopped at time: " << currentTime << std::endl;
                    lastActionTime = currentTime;
                    stopDone = true;
                    pushDone = false;
                    pullDone = false;
                    // Reset the test for next trigger
                    std::cout << "Enter 1 to restart the piston test, or 0 to exit:" << std::endl;
                }
                break;
            default:
                std::cout << "Invalid action. Enter 1 (push), 2 (pull), 3 (stop), or 0 to exit:" << std::endl;
                break;
        }

        if (stopDone) { // Allow restart or exit after the sequence completes
            std::cout << "Test sequence complete. Enter 1 to restart, or 0 to exit:" << std::endl;
        }

        std::cin >> action;
    }

    std::cout << "Exiting piston test." << std::endl;
}



