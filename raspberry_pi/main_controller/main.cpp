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
int laserReceiverWidthPIN = 35;
int laserReceiverHeightPIN = 40;
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

    int gateNumber = 0;
    int disksInTube = 0;  // Track the number of disks

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
                std::cout << "Disk falling initiated at time: " << currentTime << std::endl;
            }
            laserWidthBlocked = false;
            objectStuckAtWidth = false;
            stuckMessageDisplayedAtWidth = false;
            std::cout << "Width Laser Cleared at time: " << currentTime << std::endl;
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
            if (!objectStuckAtWidth && !forcedClear) {
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
            } else if (!carReadyCheckInProgress && sorterRobot.getCarController().isCarReady()) {
                startConveyor = true;
                sorterRobot.getMotorController().run(true); // Start conveyor before piston action
                conveyorRunning = true;
                std::cout << "Conveyor started at time: " << currentTime << std::endl;
                carOccupied = true;
                carReadyCheckInProgress = false; // Mark car ready check as completed
                colorSensingInProgress = false; // Reset color sensing for next cycle
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
            } else if (pushDone && !pullDone && currentTime - pushTime >= pistonTime + 65) {
                sorterRobot.getServoController().pullPiston();
                std::cout << "Piston pulled at time: " << currentTime << std::endl;
                pullTime = currentTime;
                pullDone = true;
            } else if (pushDone && pullDone && !stopDone && currentTime - pullTime >= pistonTime + 65) {
                sorterRobot.getServoController().stopPiston();
                std::cout << "Piston stopped at time: " << currentTime << std::endl;
                stopTime = currentTime;
                stopDone = true;
                pushDone = false;
                pullDone = false;
                startConveyor = false; // Stop the conveyor after piston operations
                checkDiskPassed = true; // Move to the next state to check if the disk has passed carDetectionLaser
            }
        }

        // Car detection laser logic
        bool carDetectionLaserDetected = sorterRobot.getLaserReceiverCarDetection().isLaserDetected();
        if (!carDetectionLaserBlocked && !carDetectionLaserDetected) {
            std::cout << "Car Detection Laser Blocked at time: " << currentTime << std::endl;
            carDetectionLaserBlocked = true;
            laserCarDetectionBlockedTime = currentTime;
        } else if (carDetectionLaserBlocked && carDetectionLaserDetected) {
            std::cout << "Car Detection Laser Cleared at time: " << currentTime << std::endl;
            carDetectionLaserBlocked = false;
            carDetectionLaserCleared = true;
        }

        // Check if disk has passed carDetectionLaser
        if (checkDiskPassed && carDetectionLaserCleared && !driveRequestSent) {
            gateNumber = rand() % 3 + 1;
            sorterRobot.getCarController().drive(gateNumber);
            driveRequestSent = true;
            checkDiskPassed = false;
            std::cout << "Drive command sent to gate " << gateNumber << " at time: " << currentTime << std::endl;
        }

        // Reset after driving
        if (driveRequestSent && !carDetectionLaserBlocked) {
            std::cout << "Disk passed car detection laser at time: " << currentTime << std::endl;
            driveRequestSent = false;
            carOccupied = false;
            carDetectionLaserCleared = false; // Reset for next detection
            carReadyCheckInProgress = false; // Reset readiness check
            waitingForCarToPass = false; // Reset waiting for car to pass
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
