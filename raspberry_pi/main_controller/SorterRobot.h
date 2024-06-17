#ifndef SORTERROBOT_SORTERROBOT_H
#define SORTERROBOT_SORTERROBOT_H

#include <iostream>
#include <chrono>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/ServoController.h"
#include "../util/laser_sensor/LaserReceiver.h"
#include "../util/laser_transmitter/LaserTransmitter.h"
#include "../network/CarController.h"

class SorterRobot {
public:
    enum State {
        IDLE,
        HEIGHT_LASER_BLOCKED,
        WIDTH_LASER_BLOCKED,
        DISK_FALLING,
        COLOR_SENSING,
        WAITING_FOR_CAR_READY,
        PISTON_OPERATION,
        CHECK_CAR_DETECTION,
        DRIVE_COMMAND_SENT,
        DISK_TIMEOUT,
        RESET_AFTER_DRIVING
    };

    SorterRobot(int motorIN1, int motorIN2, int motorEN,
                int firstConveyorIN1, int firstConveyorIN2, int firstConveyorEN,
                int servoPIN, int adcAddress, int displayAddress, int laserReceiverHeightPIN,
                int laserReceiverWidthPIN, int laserReceiverCarDetectionPIN, int laserTransmitterBlackPIN,
                int laserTransmitterWhitePIN, int laserTransmitterColorPIN, const std::string &carControllerBaseUrl);

    ~SorterRobot() {
        getMotorController().stop();
        getFirstConveyor().stop();
    }

    MotorController &getMotorController();
    MotorController &getFirstConveyor();
    ServoController &getServoController();
    ADCReader &getAdcReader();
    DisplayController &getDisplayController();
    LaserReceiver &getLaserReceiverHeight();
    LaserReceiver &getLaserReceiverWidth();
    LaserReceiver &getLaserReceiverCarDetection();
    LaserTransmitter &getLaserTransmitterBlack();
    LaserTransmitter &getLaserTransmitterWhite();
    LaserTransmitter &getLaserTransmitterColor();
    CarController &getCarController();

    void incrementDisksInTube();
    void decrementDisksInTube();
    int getDisksInTube() const;

    void robotSetup(int adcConfig);

    void handleHeightLaser(unsigned long currentTime);
    void handleWidthLaser(unsigned long currentTime);
    void handleDiskSettling(unsigned long currentTime);
    void handleColorSensing(unsigned long currentTime);
    void handlePistonOperation(unsigned long currentTime);
    void handleCarDetectionLaser(unsigned long currentTime);
    void handleDiskTimeout(unsigned long currentTime);
    void resetAfterDriving(unsigned long currentTime);
    void handleGateLasers(int gateNumber);
    void testPistonOperation();
    void autoTestPistonOperation();
    void run();

private:
    MotorController firstConveyor;
    MotorController motorController;
    ServoController servoController;
    ADCReader adcReader;
    DisplayController displayController;
    LaserReceiver laserReceiverHeight;
    LaserReceiver laserReceiverWidth;
    LaserReceiver laserReceiverCarDetection;
    LaserTransmitter laserTransmitterBlack;
    LaserTransmitter laserTransmitterWhite;
    LaserTransmitter laserTransmitterColor;
    CarController carController;

    State currentState = IDLE;

    int firstConveyorIN1{};
    int firstConveyorIN2{};
    int firstConveyorEN{};
    int motorIN1{};
    int motorIN2{};
    int motorEN{};
    int servoPIN{};
    int adcAddress{};
    int displayAddress{};
    int laserReceiverHeightPIN{};
    int laserReceiverWidthPIN{};
    int laserReceiverCarDetectionPIN{};
    int laserTransmitterBlackPIN{};
    int laserTransmitterWhitePIN{};
    int laserTransmitterColorPIN{};
    int disksInTube{};

    unsigned long laserWidthBlockedTime = 0;
    unsigned long laserHeightBlockedTime = 0;
    unsigned long laserCarDetectionBlockedTime = 0;
    unsigned long colorDelayStartTime = 0;
    unsigned long diskFallStartTime = 0;
    unsigned long pushTime = 0;
    unsigned long pullTime = 0;
    unsigned long stopTime = 0;
    unsigned long diskTimeoutStartTime = 0;
    unsigned long carReadyCheckTime = 0;
    unsigned long carReadyCheckStartTime = 0;

    bool laserWidthBlocked = false;
    bool laserHeightBlocked = false;
    bool carDetectionLaserBlocked = false;
    bool carDetectionLaserCleared = false;
    bool objectStuckAtHeight = false;
    bool objectStuckAtWidth = false;
    bool forcedClear = false;
    bool diskFalling = false;
    bool colorDelayInProgress = false;
    bool colorSensingInProgress = false;
    bool startConveyor = false;
    bool conveyorRunning = false;
    bool pushDone = false;
    bool pullDone = false;
    bool stopDone = false;
    bool carOccupied = false;
    bool carReadyCheckInProgress = false;
    bool driveRequestSent = false;
    bool waitingForCarToPass = false;
    bool checkDiskPassed = false;
    bool diskTimeoutInProgress = false;
    bool diskPassedWidthFilter = false;
    bool expectingDiskAtCarDetection = false;

    int colorReadings = 0;
    int gateNumber = 0;

    const unsigned long stuckThreshold = 3000;
    const unsigned long carDetectionThreshold = 5000;
    const unsigned long colorDelayTime = 500;
    const unsigned long diskFallTime = 2000;
    const unsigned long pistonTime = 1900;
    const unsigned long diskTimeoutThreshold = 5000;
    const unsigned long carReadyCheckInterval = 500;

    bool stuckMessageDisplayedAtHeight = false;
    bool stuckMessageDisplayedAtWidth = false;

    static int getGateNumberFromColor(const std::string &color);
};

#endif //SORTERROBOT_SORTERROBOT_H