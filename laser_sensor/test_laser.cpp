#include <iostream>
#include <wiringPi.h>

#define RECEIVER_PIN 16  // Define the pin connected to the L1107 receiver

int main() {
    wiringPiSetupPhys();  // Set up WiringPi in physical pin numbering scheme
    pinMode(RECEIVER_PIN, INPUT);  // Set the receiver pin as an input

    while (true) {
        int laserDetected = digitalRead(RECEIVER_PIN);  // Read the state from the laser receiver

        if (laserDetected == HIGH) {
            std::cout << "Laser beam detected!" << std::endl;
        } else {
            std::cout << "No laser beam detected." << std::endl;
        }

        delay(1000);  // Delay for 1 second to limit the output rate
    }

    return 0;
}