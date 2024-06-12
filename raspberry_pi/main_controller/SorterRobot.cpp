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

// Return the motor controller
MotorController &SorterRobot::getMotorController() {
    return motorController;
}

// Return the first conveyor controller
MotorController &SorterRobot::getFirstConveyor() {
    return firstConveyor;
}

// Return the servo controller
ServoController &SorterRobot::getServoController() {
    return servoController;
}

// Return the ADC reader
ADCReader &SorterRobot::getAdcReader() {
    return adcReader;
}

// Return the display controller
DisplayController &SorterRobot::getDisplayController() {
    return displayController;
}

// Return the height laser receiver
LaserReceiver &SorterRobot::getLaserReceiverHeight() {
    return laserReceiverHeight;
}

// Return the width laser receiver
LaserReceiver &SorterRobot::getLaserReceiverWidth() {
    return laserReceiverWidth;
}

// Return the car detection laser receiver
LaserReceiver &SorterRobot::getLaserReceiverCarDetection() {
    return laserReceiverCarDetection;
}

// Return the black laser transmitter
LaserTransmitter &SorterRobot::getLaserTransmitterBlack() {
    return laserTransmitterBlack;
}

// Return the white laser transmitter
LaserTransmitter &SorterRobot::getLaserTransmitterWhite() {
    return laserTransmitterWhite;
}

// Return the color laser transmitter
LaserTransmitter &SorterRobot::getLaserTransmitterColor() {
    return laserTransmitterColor;
}

// Return the car controller
CarController &SorterRobot::getCarController() {
    return carController;
}

// Increment the count of disks in the tube
void SorterRobot::incrementDisksInTube() {
    disksInTube++;
}

// Decrement the count of disks in the tube if greater than zero
void SorterRobot::decrementDisksInTube() {
    if (disksInTube > 0) {
        disksInTube--;
    }
}

// Get the current count of disks in the tube
int SorterRobot::getDisksInTube() const {
    return disksInTube;
}

// Setup the robot with the given ADC configuration
void SorterRobot::robotSetup(int adcConfig) {
    adcReader.initADC(adcConfig);
}

// Handle the height laser sensor logic
void SorterRobot::handleHeightLaser(unsigned long currentTime) {
    bool heightLaserDetected = getLaserReceiverHeight().isLaserDetected();
    if (!laserHeightBlocked && !heightLaserDetected && !objectStuckAtHeight) {
        // Laser is blocked for the first time
        laserHeightBlocked = true;
        laserHeightBlockedTime = currentTime;
        std::cout << "Height Laser Blocked at time: " << currentTime << std::endl;
        currentState = HEIGHT_LASER_BLOCKED;
    } else if (laserHeightBlocked && heightLaserDetected) {
        // Laser is cleared
        laserHeightBlocked = false;
        objectStuckAtHeight = false;
        stuckMessageDisplayedAtHeight = false;
        std::cout << "Height Laser Cleared at time: " << currentTime << std::endl;
        currentState = IDLE;
    } else if (laserHeightBlocked && !heightLaserDetected &&
               currentTime - laserHeightBlockedTime >= stuckThreshold && !stuckMessageDisplayedAtHeight) {
        // Object is stuck at the height laser
        getDisplayController().displayString("Object Stuck at Height Laser");
        objectStuckAtHeight = true;
        stuckMessageDisplayedAtHeight = true;
        std::cout << "Object Stuck at Height Laser at time: " << currentTime << std::endl;
    }
}

// Handle the width laser sensor logic
void SorterRobot::handleWidthLaser(unsigned long currentTime) {
    bool widthLaserDetected = getLaserReceiverWidth().isLaserDetected();
    if (!laserWidthBlocked && !widthLaserDetected && !objectStuckAtWidth) {
        // Laser is blocked for the first time
        laserWidthBlocked = true;
        laserWidthBlockedTime = currentTime;
        forcedClear = false; // Reset forced clear on new blockage
        std::cout << "Width Laser Blocked at time: " << currentTime << std::endl;
        currentState = WIDTH_LASER_BLOCKED;
    } else if (laserWidthBlocked && widthLaserDetected) {
        // Laser is cleared
        if (!objectStuckAtWidth && !forcedClear) {
            incrementDisksInTube();
            diskFalling = true;
            diskFallStartTime = currentTime;
            diskPassedWidthFilter = true;  // Indicate that a disk has passed the width filter
            std::cout << "Disk falling initiated at time: " << currentTime << std::endl;
            currentState = DISK_FALLING;
        }
        laserWidthBlocked = false;
        objectStuckAtWidth = false;
        stuckMessageDisplayedAtWidth = false;
        std::cout << "Width Laser Cleared at time: " << currentTime << std::endl;
    } else if (laserWidthBlocked && !widthLaserDetected && currentTime - laserWidthBlockedTime >= stuckThreshold &&
               !stuckMessageDisplayedAtWidth) {
        // Object is stuck at the width laser
        getDisplayController().displayString("Object Stuck at Width Laser");
        objectStuckAtWidth = true;
        stuckMessageDisplayedAtWidth = true;
        forcedClear = true; // Indicate manual removal needed
        std::cout << "Object Stuck at Width Laser at time: " << currentTime << std::endl;
    }
}

// Handle the disk settling logic
void SorterRobot::handleDiskSettling(unsigned long currentTime) {
    if (diskFalling && currentTime - diskFallStartTime >= diskFallTime) {
        // Disk has settled
        diskFalling = false;
        std::cout << "Disk settled at time: " << currentTime << std::endl;
        if (!objectStuckAtWidth && !forcedClear) {
            // Start color sensing
            colorSensingInProgress = true;
            colorReadings = 0;
            std::cout << "Color sensing initiated at time: " << currentTime << std::endl;
            currentState = COLOR_SENSING;
        }
    }
}

// Handle the color sensing logic
void SorterRobot::handleColorSensing(unsigned long currentTime) {
    if (colorSensingInProgress) {  // Check if color sensing is currently in progress
        if (colorReadings < 3) {  // Perform three color readings
            if (!colorDelayInProgress) {  // Check if delay between readings is not in progress
                // Perform a color reading
                std::cout << "Performing color reading " << (colorReadings + 1) << std::endl;
                colorReadings++;  // Increment the count of readings performed
                colorDelayInProgress = true;  // Set the delay in progress flag
                colorDelayStartTime = currentTime;  // Record the start time of the delay
            } else if (currentTime - colorDelayStartTime >= colorDelayTime) {
                // If the delay time has passed, reset the delay flag
                colorDelayInProgress = false;
            }
        } else if (!carReadyCheckInProgress) {
            // After three readings, check if the car readiness check is in progress
            // If not, start the car readiness check process
            carReadyCheckInProgress = true;
            carReadyCheckStartTime = currentTime;  // Record the start time for the car readiness check
        } else if (currentTime - carReadyCheckStartTime >= carReadyCheckInterval) {
            // If the car readiness check interval has passed, proceed to check if the car is ready
            carReadyCheckStartTime = currentTime;  // Update the start time for the next check
            if (getCarController().isCarReady()) {
                // If the car is ready, start the conveyor and piston operations
                startConveyor = true;
                getMotorController().run(true);  // Start the conveyor motor
                conveyorRunning = true;
                std::cout << "Conveyor started at time: " << currentTime << std::endl;
                carOccupied = true;  // Mark the car as occupied
                carReadyCheckInProgress = false;  // Reset the car readiness check flag
                colorSensingInProgress = false;  // Reset the color sensing flag for the next cycle
                currentState = PISTON_OPERATION;  // Move to the piston operation state
            }
        }
    }
}

// Handle the piston operation logic
void SorterRobot::handlePistonOperation(unsigned long currentTime) {
    if (startConveyor) {
        if (!pushDone && !pullDone && currentTime - stopTime >= pistonTime) {
            // Push the piston
            decrementDisksInTube();
            getServoController().pushPiston();
            std::cout << "Piston pushed at time: " << currentTime << std::endl;
            pushTime = currentTime;
            pushDone = true;
            pullDone = false;
            stopDone = false;  // Set stopDone to false here
        } else if (pushDone && !pullDone && currentTime - pushTime >= pistonTime + 65) {
            // Pull the piston
            getServoController().pullPiston();
            std::cout << "Piston pulled at time: " << currentTime << std::endl;
            pullTime = currentTime;
            pullDone = true;
        } else if (pushDone && pullDone && !stopDone && currentTime - pullTime >= pistonTime + 65) {
            // Stop the piston
            getServoController().stopPiston();
            std::cout << "Piston stopped at time: " << currentTime << std::endl;
            stopTime = currentTime;
            stopDone = true;
            pushDone = false;
            pullDone = false;
            startConveyor = false;  // Stop the conveyor after piston operations
            checkDiskPassed = true;  // Move to the next state to check if the disk has passed carDetectionLaser
            diskTimeoutInProgress = true;  // Start the disk timeout check
            diskTimeoutStartTime = currentTime;  // Record the time when the disk should be detected
            currentState = CHECK_CAR_DETECTION;  // Move to the car detection state
        }
    }
}

// Handle the car detection laser logic
void SorterRobot::handleCarDetectionLaser(unsigned long currentTime) {
    bool carDetectionLaserDetected = getLaserReceiverCarDetection().isLaserDetected();
    if (!carDetectionLaserBlocked && !carDetectionLaserDetected) {
        // Laser is blocked for the first time
        if (diskPassedWidthFilter) {  // Only set car detection laser state if a disk has passed the width filter
            std::cout << "Car Detection Laser Blocked at time: " << currentTime << std::endl;
            carDetectionLaserBlocked = true;
            laserCarDetectionBlockedTime = currentTime;
        }
    } else if (carDetectionLaserBlocked && carDetectionLaserDetected) {
        // Laser is cleared
        std::cout << "Car Detection Laser Cleared at time: " << currentTime << std::endl;
        carDetectionLaserBlocked = false;
        carDetectionLaserCleared = true;
        diskPassedWidthFilter = false;  // Reset the flag after the disk has been detected
    }
}

// Handle the disk timeout logic
void SorterRobot::handleDiskTimeout(unsigned long currentTime) {
    if (diskTimeoutInProgress && (currentTime - diskTimeoutStartTime >= diskTimeoutThreshold)) {
        // Disk did not make it to the conveyor belt in time
        std::cout << "Disk did not make it to the conveyor belt at time: " << currentTime << std::endl;
        getDisplayController().displayString("Disk did not make it to the conveyor belt");
        diskTimeoutInProgress = false;  // Reset the disk timeout state
        checkDiskPassed = false;  // Reset disk passed state
        carOccupied = false;  // Reset car occupied state
        carReadyCheckInProgress = false;  // Reset readiness check state
        driveRequestSent = false;  // Ensure no drive request is pending
        std::cout << "Disk timeout reset at time: " << currentTime << std::endl;
        if (getDisksInTube() > 0) {
            // Start the next cycle if there are more disks in the tube
            std::cout << "Starting next cycle" << std::endl;
            colorSensingInProgress = true;  // Prepare for next disk's color sensing
            colorReadings = 0;
            currentState = COLOR_SENSING;
        } else {
            currentState = IDLE;
        }
    }
}

// Reset the state after driving
void SorterRobot::resetAfterDriving(unsigned long currentTime) {
    if (driveRequestSent && !carDetectionLaserBlocked) {
        std::cout << "Disk passed car detection laser at time: " << currentTime << std::endl;
        driveRequestSent = false;
        carOccupied = false;
        carDetectionLaserCleared = false;  // Reset for next detection
        carReadyCheckInProgress = false;  // Reset readiness check
        waitingForCarToPass = false;  // Reset waiting for car to pass
        diskTimeoutInProgress = false;  // Ensure disk timeout is not in progress
        if (getDisksInTube() > 0) {
            // Start the next cycle if there are more disks in the tube
            std::cout << "Starting next cycle" << std::endl;
            colorSensingInProgress = true;  // Prepare for next disk's color sensing
            colorReadings = 0;
            currentState = COLOR_SENSING;
        } else {
            currentState = IDLE;
        }
    }
}

// Main loop to run the sorter robot
void SorterRobot::run() {
    while (true) {
        unsigned long currentTime = millis();  // Get the current time

        // Handle different states of the sorter robot
        switch (currentState) {
            case IDLE:
                handleHeightLaser(currentTime);
                handleWidthLaser(currentTime);
                break;
            case HEIGHT_LASER_BLOCKED:
                handleHeightLaser(currentTime);
                break;
            case WIDTH_LASER_BLOCKED:
                handleWidthLaser(currentTime);
                break;
            case DISK_FALLING:
                handleDiskSettling(currentTime);
                break;
            case COLOR_SENSING:
                handleColorSensing(currentTime);
                break;
            case WAITING_FOR_CAR_READY:
                handleColorSensing(currentTime);
                break;
            case PISTON_OPERATION:
                handlePistonOperation(currentTime);
                break;
            case CHECK_CAR_DETECTION:
                handleCarDetectionLaser(currentTime);
                break;
            case DRIVE_COMMAND_SENT:
                handleCarDetectionLaser(currentTime);
                resetAfterDriving(currentTime);
                break;
            case DISK_TIMEOUT:
                handleDiskTimeout(currentTime);
                break;
            case RESET_AFTER_DRIVING:
                resetAfterDriving(currentTime);
                break;
        }

        usleep(10000);  // Sleep to prevent high CPU usage
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
