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
int displayAddress = 0x27;
int laserReceiverWidthPIN = 35;
int laserReceiverHeightPIN = 40;
int laserReceiverCarDetectionPIN = 38;
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

    bool laserWidthBlocked = false;
    bool laserHeightBlocked = false;
    bool objectStuck = false;
    bool stuckMessageDisplayed = false; // New variable
    unsigned long laserWidthBlockedTime = 0;
    unsigned long laserHeightBlockedTime = 0;
    const unsigned long stuckThreshold = 3000; // 3 seconds in milliseconds

    int colorReadings = 0;
    bool colorSensingInProgress = false;
    bool startConveyor = false;

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
    const unsigned long diskFallTime = 1000; // 1 second for the disk to fall into the tube

    std::cout << "Entering while loop" << std::endl;

    while (true) {
        unsigned long currentTime = millis();

        bool heightLaserDetected = sorterRobot.getLaserReceiverHeight().isLaserDetected();
        if (!laserHeightBlocked && !heightLaserDetected && !objectStuck) {
            std::cout << "Height Laser Blocked" << std::endl;
            laserHeightBlocked = true;
            laserHeightBlockedTime = currentTime;
        }

        if (laserHeightBlocked && heightLaserDetected) {
            std::cout << "Height Laser Cleared" << std::endl;
            laserHeightBlocked = false;
            objectStuck = false;
            stuckMessageDisplayed = false; // Reset the stuckMessageDisplayed variable when the laser is cleared
        }

        if (laserHeightBlocked && !heightLaserDetected) {
            if (currentTime - laserHeightBlockedTime >= stuckThreshold && !stuckMessageDisplayed) {
                std::cout << "Object Stuck at Height Laser" << std::endl;
                sorterRobot.getDisplayController().displayString("Object Stuck at Height Laser");
                objectStuck = true;
                stuckMessageDisplayed = true; // Set the stuckMessageDisplayed variable to true when the message is displayed
            }
        }

        bool widthLaserDetected = sorterRobot.getLaserReceiverWidth().isLaserDetected();
        if (!laserWidthBlocked && !widthLaserDetected && !objectStuck) {
            std::cout << "Width Laser Blocked" << std::endl;
            laserWidthBlocked = true;
            laserWidthBlockedTime = currentTime;
        }

        if (laserWidthBlocked && widthLaserDetected) {
            std::cout << "Width Laser Cleared" << std::endl;
            laserWidthBlocked = false;
            sorterRobot.incrementDisksInTube();

            // Check if color sensing is not already in progress
            if (!colorSensingInProgress) {
                diskFalling = true;
                diskFallStartTime = currentTime;
            }
        }

        if (laserWidthBlocked && !widthLaserDetected) {
            if (currentTime - laserWidthBlockedTime >= stuckThreshold && !stuckMessageDisplayed) {
                std::cout << "Object Stuck at Width Laser" << std::endl;
                sorterRobot.getDisplayController().displayString("Object Stuck at Width Laser");
                objectStuck = true;
                stuckMessageDisplayed = true; // Set the stuckMessageDisplayed variable to true when the message is displayed
            }
        }

        if (diskFalling && currentTime - diskFallStartTime >= diskFallTime) {
            diskFalling = false;

            if (!colorSensingInProgress) {
                colorSensingInProgress = true;
                colorReadings = 0;
            }
        }

        if (colorSensingInProgress) {
            if (!colorDelayInProgress) {
                std::cout << "Performing color reading " << (colorReadings + 1) << std::endl;

                colorReadings++;

                colorDelayInProgress = true;
                colorDelayStartTime = currentTime;
            }
            else if (currentTime - colorDelayStartTime >= colorDelayTime) {
                colorDelayInProgress = false;
            }

            if (colorReadings >= 3) {
                startConveyor = true;
                colorSensingInProgress = false;
            }
        }

        if (startConveyor) {
            if (!pushDone && currentTime - stopTime >= pistonTime) {
                sorterRobot.getServoController().pushPiston();
                std::cout << "Piston pushed at time: " << currentTime << std::endl;
                pushTime = currentTime;
                pushDone = true;
                pullDone = false;
            }

            if (pushDone && !pullDone && currentTime - pushTime >= pistonTime) {
                sorterRobot.getServoController().pullPiston();
                std::cout << "Piston pulled at time: " << currentTime << std::endl;
                pullTime = currentTime;
                pullDone = true;
                stopDone = false;
            }

            if (pullDone && !stopDone && currentTime - pullTime >= pistonTime) {
                sorterRobot.getServoController().stopPiston();
                std::cout << "Piston stopped at time: " << currentTime << std::endl;
                stopTime = currentTime;
                stopDone = true;
                pushDone = false;

                sorterRobot.decrementDisksInTube();

                if (sorterRobot.getDisksInTube() > 0) {
                    colorSensingInProgress = true;
                    colorReadings = 0;
                }

                startConveyor = false; // Add this line to stop the conveyor after the piston has moved
            }
        }

        usleep(50000);
    }

    return 0;
}