
#include "SorterRobot.h"

SorterRobot::SorterRobot(int motorIN1, int motorIN2, int motorEN,
                         int servoPIN, int ADC_Adr, int laserPIN, int displayPIN)
        : motorController(motorIN1, motorIN2, motorEN), servoController(servoPIN),
          adcReader(ADC_Adr), laserReceiver(laserPIN), displayController(displayPIN),
          motorIN1(motorIN1), motorIN2(motorIN2), motorEN(motorEN),
          servoPIN(servoPIN), ADC_Adr(ADC_Adr),
          laserPIN(laserPIN), displayPIN(displayPIN) {
}

void SorterRobot::system_init(int lightSensorConfig) {
    wiringPiSetupPhys();
    motorController.setup();
    laserReceiver.init();
    adcReader.initADC(lightSensorConfig);
    displayController.displayInit();
}

void SorterRobot::clearDisplay() {
    displayController.displayClear();
}

void SorterRobot::displayString(const char *str) {
    displayController.displayString(str);
}

void SorterRobot::movePiston() {
    servoController.movePiston();
}

void SorterRobot::runConveyorBelt(bool direction) {
    motorController.run(direction);
}

void SorterRobot::stopConveyorBelt() {
    motorController.stop();
}

bool SorterRobot::isLaserDetected() const {
    return laserReceiver.isLaserDetected();
}
