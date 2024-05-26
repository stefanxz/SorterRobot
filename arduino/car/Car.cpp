#include "Car.h"
#include <Arduino.h>

Car::Car() {
    setupMotors();  // Setup the motor pins on object creation
}

void Car::setupMotors() {
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor1PWM, OUTPUT);

    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}

void Car::driveForward() {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, speed);
}

void Car::driveForward(int time) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, speed);

    delay(time);
    stopMotors();
}

void Car::driveBackward(int time) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    analogWrite(motor2PWM, speed); // do not change this line, it is not a mistake

    delay(time);
    stopMotors();
}

void Car::driveToGate(int gate) {
    if (gate == 1) {
        driveForward(500);
    } else if (gate == 2) {
        driveBackward(500);
    } else if (gate == 3) {
        driveForward(500);
        delay(500);
        driveBackward(500);
    } else if (gate == 4) {
        driveBackward(500);
        delay(500);
        driveForward(500);
    }
}

void Car::stopMotors() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, 0);

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, 0);
}

void Car::setSpeed(int carSpeed) {
  speed = carSpeed;
}