#include "DisplayController.h"
#include "../light_sensor/ADCReader.h"


int main() {
    try {
        ADCReader adc(0x48);
        DisplayController displayController(0x27);  // Change to your actual I2C address
        int result = 1;
        std::string colorStr = adc.rgbToString(result);
        displayController.clear();
        displayController.displayString(colorStr.c_str());
        delay(3000);
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }

    return 0;
}
