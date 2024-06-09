//
// Created by Stefan on 24.05.2024.
//
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
int displayAddress = -1;
int laserReceiverHeightPIN = -1;
int laserReceiverWidthPIN = -1;
int laserReceiverCarDetectionPIN = -1;
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
    unsigned long laserWidthBlockedTime = 0;
    unsigned long laserHeightBlockedTime = 0;
    const unsigned long stuckThreshold = 3000; // 3 seconds in milliseconds

    int colorReadings = 0;
    bool colorSensingInProgress = false;
    bool startConveyor = true;

    while (true) {
        unsigned long currentTime = millis();
        /**
        // Check for initial laser blockage
        if (!laserWidthBlocked && sorterRobot.getLaserReceiverWidth().isLaserDetected()) {
            laserWidthBlocked = true;
            laserWidthBlockedTime = currentTime;
            sorterRobot.getDisplayController().displayString("Width Laser Blocked");
        }

        // Check if the laser is still blocked after 3 seconds
        if (laserWidthBlocked && sorterRobot.getLaserReceiverWidth().isLaserDetected()) {
            if (currentTime - laserWidthBlockedTime >= stuckThreshold) {
                sorterRobot.getDisplayController().displayString("Object Stuck at Width Laser");
                // Handle stuck condition, e.g., stop system or raise an alarm
                laserWidthBlocked = false; // Reset the blockage flag after handling
            }
        }

        // Reset blockage flag if the object moves away before 3 seconds
        if (laserWidthBlocked && !sorterRobot.getLaserReceiverWidth().isLaserDetected()) {
            sorterRobot.getDisplayController().displayString("Width Laser Cleared");
            laserWidthBlocked = false;
            sorterRobot.incrementDisksInTube(); // Increment disksInTube when a disk passes the second laser
        }

        // Similar logic for height laser
        if (!laserHeightBlocked && sorterRobot.getLaserReceiverHeight().isLaserDetected()) {
            laserHeightBlocked = true;
            laserHeightBlockedTime = currentTime;
            sorterRobot.getDisplayController().displayString("Height Laser Blocked");
        }

        if (laserHeightBlocked && sorterRobot.getLaserReceiverHeight().isLaserDetected()) {
            if (currentTime - laserHeightBlockedTime >= stuckThreshold) {
                sorterRobot.getDisplayController().displayString("Object Stuck at Height Laser");
                laserHeightBlocked = false;
            }
        }

        if (laserHeightBlocked && !sorterRobot.getLaserReceiverHeight().isLaserDetected()) {
            sorterRobot.getDisplayController().displayString("Height Laser Cleared");
            laserHeightBlocked = false;
        }

        if (sorterRobot.getDisksInTube() > 0 && !colorSensingInProgress) {
            colorSensingInProgress = true;
            colorReadings = 0;
        }

        // Perform color sensing if it's in progress
        if (colorSensingInProgress) {
            // Perform a color reading
            // For now, we'll just print a message
            std::cout << "Performing color reading " << (colorReadings + 1) << std::endl;

            colorReadings++;

            if (colorReadings >= 3) {
                // We've done three color readings, so we can start the conveyor belt
                startConveyor = true;
                colorSensingInProgress = false;
            }
        }
        **/

        if (startConveyor) {
            sorterRobot.getServoController().movePiston();

            // Check if the piston has returned to the idle state
            if (sorterRobot.getServoController().getState() == IDLE) {
                // The piston has completed its movement, so we can reset the conveyor flag
                startConveyor = false;
            }
        }

        usleep(100000); // Sleep for 100ms to reduce CPU usage
    }

    return 0;
}