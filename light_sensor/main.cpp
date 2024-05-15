#include "ADCReader.h"

int main() {
    ADCReader adc(0x48); // Default I2C address for ADS1115
    adc.runContinuousRead();
    return 0;
}
