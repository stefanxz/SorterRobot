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
    if (startConveyor) {
        // Push the piston
        if (!pushDone && !pullDone && currentTime - stopTime >= pistonTime) {
            decrementDisksInTube();
            getServoController().pushPiston();
            std::cout << "Piston pushed at time: " << currentTime << std::endl;
            pushTime = currentTime;
            pushDone = true;
            expectingDiskAtCarDetection = true; // Flag that the piston has pushed a disk
        }
            // Pull the piston back
        else if (pushDone && !pullDone && currentTime - pushTime >= pistonTime + 150) {
            getServoController().pullPiston();
            std::cout << "Piston pulled at time: " << currentTime << std::endl;
            pullTime = currentTime;
            pullDone = true;
        }
            // Stop the piston after pulling
        else if (pushDone && pullDone && !stopDone && currentTime - pullTime >= pistonTime ) {
            getServoController().stopPiston();
            std::cout << "Piston stopped at time: " << currentTime << std::endl;
            stopTime = currentTime;
            stopDone = true;
            pushDone = false;
            pullDone = false;
            startConveyor = false;
            diskTimeoutInProgress = true; // Monitor for disk timeout starting now
            diskTimeoutStartTime = currentTime; // Set the start time for timeout monitoring
            currentState = CHECK_CAR_DETECTION; // Move to the next state for car detection
        }
    }
}


void SorterRobot::handleCarDetectionLaser(unsigned long currentTime) {
    bool carDetectionLaserDetected = getLaserReceiverCarDetection().isLaserDetected();
    if (!carDetectionLaserBlocked && !carDetectionLaserDetected && expectingDiskAtCarDetection) {
        std::cout << "Car Detection Laser Blocked at time: " << currentTime << std::endl;
        carDetectionLaserBlocked = true;
        laserCarDetectionBlockedTime = currentTime;
    } else if (carDetectionLaserBlocked && carDetectionLaserDetected) {
        std::cout << "Car Detection Laser Cleared at time: " << currentTime << std::endl;
        carDetectionLaserBlocked = false;
        if (expectingDiskAtCarDetection) {
            if (!driveRequestSent) {
                getCarController().drive(gateNumber);
                driveRequestSent = true;
                std::cout << "Drive request sent at time: " << currentTime << std::endl;
            }
            expectingDiskAtCarDetection = false; // Reset the flag
        }
        currentState = RESET_AFTER_DRIVING;
    }
}

void SorterRobot::handleDiskTimeout(unsigned long currentTime) {
    if (diskTimeoutInProgress && (currentTime - diskTimeoutStartTime >= diskTimeoutThreshold)) {
        std::cout << "Disk did not make it to the conveyor belt at time: " << currentTime << std::endl;
        getDisplayController().displayString("Disk did not make it to the conveyor belt");
        diskTimeoutInProgress = false;
        expectingDiskAtCarDetection = false; // Reset the flag on timeout
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
    unsigned long currentTime = 0;
    int action = 0; // Action to be taken (1 = Push, 2 = Pull, 3 = Stop, 0 = Exit)

    std::cout << "Piston Operation Test\n";
    std::cout << "Enter 1 to push, 2 to pull, 3 to stop, or 0 to exit:\n";

    while (std::cin >> action && action != 0) {
        currentTime = millis(); // Get current time in milliseconds, simulate or use actual function

        switch (action) {
            case 1: // Push the piston
                if (!pushDone) {
                    getServoController().pushPiston(); // Simulate push
                    std::cout << "Piston pushed at time: " << currentTime << std::endl;
                    pushTime = currentTime; // Record push time
                    pushDone = true;
                    pullDone = false;
                    stopDone = false;
                } else {
                    std::cout << "Piston already pushed, reset to push again.\n";
                }
                break;
            case 2: // Pull the piston
                if (pushDone && !pullDone) {
                    getServoController().pullPiston(); // Simulate pull
                    std::cout << "Piston pulled at time: " << currentTime << std::endl;
                    pullTime = currentTime; // Record pull time
                    pullDone = true;
                } else {
                    std::cout << "Push piston first or piston already pulled.\n";
                }
                break;
            case 3: // Stop the piston
                if (pushDone && pullDone && !stopDone) {
                    getServoController().stopPiston(); // Simulate stop
                    std::cout << "Piston stopped at time: " << currentTime << std::endl;
                    stopTime = currentTime; // Record stop time
                    stopDone = true;
                    pushDone = false;
                    pullDone = false;
                } else {
                    std::cout << "Pull piston first or piston already stopped.\n";
                }
                break;
            default:
                std::cout << "Invalid action. Enter 1 (push), 2 (pull), 3 (stop), or 0 to exit:\n";
                break;
        }
    }

    std::cout << "Exiting piston test.\n";
}

void SorterRobot::autoTestPistonOperation() {
    unsigned long currentTime = millis();
    unsigned long startTestTime = currentTime;
    unsigned long pushDuration = pistonTime;      // Duration to push the piston
    unsigned long pullDelay = pistonTime + 150;   // Delay after push to start pull
    unsigned long stopDelay = pistonTime + 25;         // Delay after pull to stop

    startConveyor = true;  // Ensure conveyor is started for the test

    // Simulate the push operation
    while (millis() - startTestTime < pushDuration) {
        handlePistonOperation(millis());
    }

    // Simulate the pull operation
    startTestTime = millis(); // Reset the start time for the pull operation
    while (millis() - startTestTime < pullDelay) {
        handlePistonOperation(millis());
    }

    // Simulate the stop operation
    startTestTime = millis(); // Reset the start time for the stop operation
    while (millis() - startTestTime < stopDelay) {
        handlePistonOperation(millis());
    }

    // After completing the test, ensure all flags and controls are reset
    pushDone = false;
    pullDone = false;
    stopDone = false;
    startConveyor = false;
    currentState = IDLE;  // Set robot back to idle state
    std::cout << "Auto test of piston operation completed." << std::endl;
}



