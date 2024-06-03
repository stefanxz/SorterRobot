#include "Car.h"
#include <Arduino.h>
#include <Servo.h>

Car::Car()
{
    setupMotors(); // Setup the motor pins on object creation
}

void Car::setupMotors()
{
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor1PWM, OUTPUT);

    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}
void Car::driveForward(int milliseconds)
{
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, speed);

    delay(milliseconds);
    stopMotors();
}
void Car::driveForward()
{
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, speed);
}
void Car::driveBackward(int milliseconds)
{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    analogWrite(motor2PWM, speed);

    delay(milliseconds);
    stopMotors();
}
void Car::driveBackward()
{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    analogWrite(motor1PWM, speed);

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    analogWrite(motor2PWM, speed);
}

void Car::driveToGate(int gate)
{
    driveForward();
}

void Car::stopMotors()
{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, 0);

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, 0);
}

void Car::setSpeed(int newSpeed)
{
    if (speed >= 0 && speed <= 100)
    {
        speed = newSpeed; // Update the speed variable
    }
}

bool Car::isReady()
{
    return ready;
}

void Car::handleDropoff(Servo &servo) {
  stopMotors();
  servo.write(160);
  delay(2000);
  servo.write(0);
  delay(2000);
  driveBackward();
}