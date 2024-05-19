#include "Car.h"

Car::Car() {
  // Initialization code here
}

void Car::driveToGate(int gate) {
  // Drive car to the specified gate
  Serial.print("Driving to gate: ");
  Serial.println(gate);
}
