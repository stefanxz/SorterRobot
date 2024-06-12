#include <iostream>
#include <sstream> // For stringstream
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
#include <cstdlib>
#include "../network/CarController.h"
#include <fcntl.h>  // For file control definitions
#include <sys/ioctl.h>  // For IOCTLs
#include <linux/i2c-dev.h>  // For the I2C device definitions
#include <cerrno>  // For error number definitions
#include <cstring>  // For strerror()
#include <iostream>  // For input-output streams
#include <stdexcept>  // For standard exceptions

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
int firstConveyorIN1 = 24;
int firstConveyorIN2 = 26;
int firstConveyorEN = 23;
int servoPIN = 37;
int adcAddress = 0x48;
int displayAddress = 0x27;
int laserReceiverHeightPIN = 19;
int laserReceiverWidthPIN = 7;
int laserReceiverCarDetectionPIN = 38;
int laserTransmitterBlackPIN = 31;
int laserTransmitterWhitePIN = 32;
int laserTransmitterColorPIN = 33;
int adcConfig = 0xC183;

int main() {
    system_init();
    std::string arduinoIP = "172.20.10.2";
    SorterRobot sorterRobot(motorIN1, motorIN2, motorEN, firstConveyorIN1, firstConveyorIN2, firstConveyorEN, servoPIN,
                            adcAddress, displayAddress, laserReceiverHeightPIN,
                            laserReceiverWidthPIN, laserReceiverCarDetectionPIN, laserTransmitterBlackPIN,
                            laserTransmitterWhitePIN, laserTransmitterColorPIN, arduinoIP);
    sorterRobot.robotSetup(adcConfig);

    bool laserWidthBlocked = false;
    bool laserHeightBlocked = false;
    bool carDetectionLaserBlocked = false;
    bool carDetectionLaserCleared = false;
    bool objectStuckAtHeight = false;
    bool objectStuckAtWidth = false;
    unsigned long laserWidthBlockedTime = 0;
    unsigned long laserHeightBlockedTime = 0;
    unsigned long laserCarDetectionBlockedTime = 0;
    const unsigned long stuckThreshold = 3000;
    const unsigned long carDetectionThreshold = 5000;

    int colorReadings = 0;
    bool colorSensingInProgress = false;
    bool startConveyor = false;
    bool conveyorRunning = false;
    bool carReadyWaitInProgress = false; // Flag to indicate waiting for car readiness

    bool pushDone = false;
    bool pullDone = false;
    bool stopDone = true;
    unsigned long pushTime = 0;
    unsigned long pullTime = 0;
    unsigned long stopTime = 0;
    const unsigned long pistonTime = 1500;

    bool colorDelayInProgress = false;
    unsigned long colorDelayStartTime = 0;
    const unsigned long colorDelayTime = 500;

    bool diskFalling = false;
    unsigned long diskFallStartTime = 0;
    const unsigned long diskFallTime = 1000;

    bool stuckMessageDisplayedAtHeight = false;
    bool stuckMessageDisplayedAtWidth = false;
    bool forcedClear = false;

    bool carOccupied = false;
    bool carReadyCheckInProgress = false;
    bool driveRequestSent = false;
    bool waitingForCarToPass = false;
    bool checkDiskPassed = false; // New state for checking disk passed carDetectionLaser
    bool diskTimeoutInProgress = false; // Track disk timeout state
    unsigned long diskTimeoutStartTime = 0; // Time when disk timeout starts
    const unsigned long diskTimeoutThreshold = 2000; // 2 seconds threshold

    unsigned long carReadyCheckTime = 0; // Time for last car ready check
    const unsigned long carReadyCheckInterval = 500; // Interval between car ready checks

    int gateNumber = 0;
    int disksInTube = 0;  // Track the number of disks
    bool diskPassedWidthFilter = false;  // Track if a disk has passed the width filter

    std::cout << "Entering main control loop" << std::endl;

    while (true) {
        unsigned long currentTime = millis();

        // Height laser detection logic
        bool heightLaserDetected = sorterRobot.getLaserReceiverHeight().isLaserDetected();
        if (!laserHeightBlocked && !heightLaserDetected && !objectStuckAtHeight) {
            laserHeightBlocked = true;
            laserHeightBlockedTime = currentTime;
            std::cout << "Height Laser Blocked at time: " << currentTime << std::endl;
        } else if (laserHeightBlocked && heightLaserDetected) {
            laserHeightBlocked = false;
            objectStuckAtHeight = false;
            stuckMessageDisplayedAtHeight = false;
            std::cout << "Height Laser Cleared at time: " << currentTime << std::endl;
            sorterRobot.getDisplayController().displayClear();
        } else if (laserHeightBlocked && !heightLaserDetected &&
                   currentTime - laserHeightBlockedTime >= stuckThreshold && !stuckMessageDisplayedAtHeight) {
            sorterRobot.getDisplayController().displayString("Object Stuck at Height Laser");
            objectStuckAtHeight = true;
            stuckMessageDisplayedAtHeight = true;
            std::cout << "Object Stuck at Height Laser at time: " << currentTime << std::endl;
        }

        // Width Laser Detection
        bool widthLaserDetected = sorterRobot.getLaserReceiverWidth().isLaserDetected();
        if (!laserWidthBlocked && !widthLaserDetected && !objectStuckAtWidth) {
            laserWidthBlocked = true;
            laserWidthBlockedTime = currentTime;
            forcedClear = false; // Reset forced clear on new blockage
            std::cout << "Width Laser Blocked at time: " << currentTime << std::endl;
        } else if (laserWidthBlocked && widthLaserDetected) {
            if (!objectStuckAtWidth && !forcedClear) {
                sorterRobot.incrementDisksInTube();
                diskFalling = true;
                diskFallStartTime = currentTime;
                diskPassedWidthFilter = true;  // Indicate that a disk has passed the width filter
                std::cout << "Disk falling initiated at time: " << currentTime << std::endl;
            }
            laserWidthBlocked = false;
            objectStuckAtWidth = false;
            stuckMessageDisplayedAtWidth = false;
            std::cout << "Width Laser Cleared at time: " << currentTime << std::endl;
            sorterRobot.getDisplayController().displayClear();
        } else if (laserWidthBlocked && !widthLaserDetected && currentTime - laserWidthBlockedTime >= stuckThreshold &&
                   !stuckMessageDisplayedAtWidth) {
            sorterRobot.getDisplayController().displayString("Object Stuck at Width Laser");
            objectStuckAtWidth = true;
            stuckMessageDisplayedAtWidth = true;
            forcedClear = true; // Indicate manual removal needed
            std::cout << "Object Stuck at Width Laser at time: " << currentTime << std::endl;
        }

        // Disk Settling Logic
        if (diskFalling && currentTime - diskFallStartTime >= diskFallTime) {
            diskFalling = false;
            std::cout << "Disk settled at time: " << currentTime << std::endl;
            if (!objectStuckAtWidth && !forcedClear && !carReadyWaitInProgress) { // Add check for carReadyWaitInProgress
                colorSensingInProgress = true;
                colorReadings = 0;
                std::cout << "Color sensing initiated at time: " << currentTime << std::endl;
            }
        }

        // Color Sensing Logic
        if (colorSensingInProgress) {
            if (colorReadings < 3) {
                if (!colorDelayInProgress) {
                    std::cout << "Performing color reading " << (colorReadings + 1) << std::endl;
                    colorReadings++;
                    colorDelayInProgress = true;
                    colorDelayStartTime = currentTime;
                } else if (currentTime - colorDelayStartTime >= colorDelayTime) {
                    colorDelayInProgress = false;
                }
            } else {
                if (!carReadyCheckInProgress) {
                    carReadyCheckInProgress = true;
                    carReadyCheckTime = currentTime;  // Initialize car ready check time
                }
                if (currentTime - carReadyCheckTime >= carReadyCheckInterval) {
                    carReadyCheckTime = currentTime;  // Update the last check time
                    if (sorterRobot.getCarController().isCarReady()) {
                        startConveyor = true;
                        sorterRobot.getMotorController().run(true); // Start conveyor before piston action
                        conveyorRunning = true;
                        std::cout << "Conveyor started at time: " << currentTime << std::endl;
                        carOccupied = true;
                        carReadyCheckInProgress = false; // Mark car ready check as completed
                        carReadyWaitInProgress = false; // Reset car ready wait flag
                        colorSensingInProgress = false; // Reset color sensing for next cycle
                    } else {
                        carReadyWaitInProgress = true;  // Set the flag to indicate waiting for car readiness
                    }
                }
            }
        }

        // Piston Operation Logic
        if (startConveyor) {
            if (!pushDone && !pullDone && currentTime - stopTime >= pistonTime) {
                sorterRobot.decrementDisksInTube();
                sorterRobot.getServoController().pushPiston();
                std::cout << "Piston pushed at time: " << currentTime << std::endl;
                pushTime = currentTime;
                pushDone = true;
                pullDone = false;
                stopDone = false; // Set stopDone to false here
            } else if (pushDone && !pullDone && currentTime - pushTime >= pistonTime + 130) {
                sorterRobot.getServoController().pullPiston();
                std::cout << "Piston pulled at time: " << currentTime << std::endl;
                pullTime = currentTime;
                pullDone = true;
            } else if (pushDone && pullDone && !stopDone && currentTime - pullTime >= pistonTime + 130) {
                sorterRobot.getServoController().stopPiston();
                std::cout << "Piston stopped at time: " << currentTime << std::endl;
                stopTime = currentTime;
                stopDone = true;
                pushDone = false;
                pullDone = false;
                startConveyor = false; // Stop the conveyor after piston operations
                checkDiskPassed = true; // Move to the next state to check if the disk has passed carDetectionLaser
                diskTimeoutInProgress = true; // Start the disk timeout check
                diskTimeoutStartTime = currentTime; // Record the time when the disk should be detected
            }
        }

        // Car detection laser logic
        bool carDetectionLaserDetected = sorterRobot.getLaserReceiverCarDetection().isLaserDetected();
        if (!carDetectionLaserBlocked && !carDetectionLaserDetected) {
            if (diskPassedWidthFilter) {  // Only set car detection laser state if a disk has passed the width filter
                std::cout << "Car Detection Laser Blocked at time: " << currentTime << std::endl;
                carDetectionLaserBlocked = true;
                laserCarDetectionBlockedTime = currentTime;
            }
        } else if (carDetectionLaserBlocked && carDetectionLaserDetected) {
            std::cout << "Car Detection Laser Cleared at time: " << currentTime << std::endl;
            carDetectionLaserBlocked = false;
            carDetectionLaserCleared = true;
            diskPassedWidthFilter = false;  // Reset the flag after the disk has been detected
        }

        // Check if disk has passed carDetectionLaser
        if (checkDiskPassed && carDetectionLaserCleared && !driveRequestSent) {
            gateNumber = rand() % 3 + 1;
            sorterRobot.getCarController().drive(gateNumber);
            driveRequestSent = true;
            checkDiskPassed = false;
            std::cout << "Drive command sent to gate " << gateNumber << " at time: " << currentTime << std::endl;
        }

        // Disk timeout logic
        if (diskTimeoutInProgress && (currentTime - diskTimeoutStartTime >= diskTimeoutThreshold)) {
            std::cout << "Disk did not make it to the conveyor belt at time: " << currentTime << std::endl;
            sorterRobot.getDisplayController().displayString("Disk did not make it to the conveyor belt");
            diskTimeoutInProgress = false; // Reset the disk timeout state
            checkDiskPassed = false; // Reset disk passed state
            carOccupied = false; // Reset car occupied state
            carReadyCheckInProgress = false; // Reset readiness check state
            carReadyWaitInProgress = false; // Reset car ready wait flag
            driveRequestSent = false; // Ensure no drive request is pending
            std::cout << "Disk timeout reset at time: " << currentTime << std::endl;
            if (sorterRobot.getDisksInTube() > 0) {
                std::cout << "Starting next cycle" << std::endl;
                colorSensingInProgress = true; // Prepare for next disk's color sensing
                colorReadings = 0;
            }
        }

        // Reset after driving
        if (driveRequestSent && !carDetectionLaserBlocked) {
            std::cout << "Disk passed car detection laser at time: " << currentTime << std::endl;
            driveRequestSent = false;
            carOccupied = false;
            carDetectionLaserCleared = false; // Reset for next detection
            carReadyCheckInProgress = false; // Reset readiness check
            waitingForCarToPass = false; // Reset waiting for car to pass
            diskTimeoutInProgress = false; // Ensure disk timeout is not in progress
            carReadyWaitInProgress = false; // Reset car ready wait flag
            if (sorterRobot.getDisksInTube() > 0) {
                std::cout << "Starting next cycle" << std::endl;
                colorSensingInProgress = true; // Prepare for next disk's color sensing
                colorReadings = 0;
            }
        }

        usleep(10000); // Sleep to prevent high CPU usage
    }

    return 0;
}