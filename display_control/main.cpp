#include "DisplayController.h"

int main() {
    try {
        DisplayController displayController(0x27);  // Change to your actual I2C address
        displayController.displayString("Hello, Madalina!");
        delay(2000);
        displayController.clear();
        displayController.displayString("Raspberry Pi 3B");
        delay(2000);
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }

    return 0;
}
