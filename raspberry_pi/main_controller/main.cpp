#include <iostream>
#include "../util/light_sensor/ADCReader.h"
#include <sstream> // For stringstream
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>  // For file control definitions
#include <sys/ioctl.h>  // For IOCTLs
#include <linux/i2c-dev.h>  // For the I2C device definitions
#include <cerrno>  // For error number definitions
#include <cstring>  // For strerror()
#include <iostream>  // For input-output streams
#include <stdexcept>  // For standard exceptions

#include "../util/display_control/DisplayController.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/motor_control/MotorController.h"
#include "../util/servo_control/ServoController.h"
#include "../util/laser_sensor/LaserReceiver.h"
#include "../util/laser_transmitter/LaserTransmitter.h"
#include "../network/CarController.h"
#include "SorterRobot.h"

// Function to initialize the system
void system_init() {
    std::cout << "Starting system initialization." << std::endl;
    if (wiringPiSetupPhys() == -1) {
        std::cerr << "Error setting up wiringPi. Initialization failed." << std::endl;
        return;
    }
    std::cout << "System init done!" << '\n';
}

int main() {
    system_init(); // Call system initialization

    // Define the IP address of the Arduino
    std::string arduinoIP = "172.20.10.2";

    // Instantiate SorterRobot with appropriate pin configurations and addresses
    SorterRobot sorterRobot(13, 15, 11, 24, 26, 23, 37, 0x48, 0x27, 40, 35, 38, 31, 32, 33, arduinoIP);

    // Setup the robot with ADC configuration
    sorterRobot.robotSetup(0xC183);

    // Run the main control loop of the sorter robot
    sorterRobot.run();
    return 0; // Exit the program
}
>>>>>>> comments_for_all_files
