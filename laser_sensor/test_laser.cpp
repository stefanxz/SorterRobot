#include <iostream>
#include <wiringPi.h>
#include <VL53L0X.h> // Include the library for the VL53L0X sensor

#define ACTION_PIN 40

int main() {
    wiringPiSetupPhys(); // Set up WiringPi in physical pin numbering scheme
    pinMode(ACTION_PIN, OUTPUT); // Set the action pin as an output

    VL53L0X sensor;
    sensor.init(); // Initialize the sensor
    sensor.setTimeout(500); // Set a timeout for sensor reading
    sensor.startContinuous(); // Start continuous measurement

    while (true) {
        int distance = sensor.readRangeContinuousMillimeters(); // Read distance in millimeters
        if (sensor.timeoutOccurred()) { 
            std::cout << "Sensor timeout!" << std::endl;
        } else {
            std::cout << "Distance: " << distance << " mm" << std::endl;
        }

        if (distance < 200) { // Example threshold distance
            digitalWrite(ACTION_PIN, HIGH); // Set the action pin high
            std::cout << "Detected close object!" << std::endl;
        } else {
            digitalWrite(ACTION_PIN, LOW); // Set the action pin low
        }

        delay(1000); // Delay for 1 second
    }

    return 0;
}