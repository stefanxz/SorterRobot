#include "LaserReceiver.h"
#include <iostream>
#include <unistd.h> // for sleep function

int main() {
    int receiverPin = 16;  // Change this to the physical pin connected to your laser receiver
    LaserReceiver receiver(receiverPin);

    receiver.init(); // Initialize the laser receiver

    while (true) {
        if (receiver.isLaserDetected()) {
            std::cout << "Laser beam detected!" << std::endl;
        } else {
            std::cout << "No laser beam detected." << std::endl;
        }
        sleep(1); // Sleep for 1 second
    }

    return 0;
}
