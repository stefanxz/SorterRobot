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

void Car::driveToGate(int newGate)
{
    driveForward();
    gate = newGate;
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

void Car::setReady(bool car_ready)
{
    ready = car_ready;
}

void Car::handleDropoff(Servo &servo) {
    stopMotors();
    dropoffState = MOVING_TO_DROP;
    dropoffStartTime = millis();
    servo.write(160); // Start moving the servo to drop position
}

void Car::updateServo(Servo &servo) {
    unsigned long currentTime = millis();

    switch (dropoffState) {
        case MOVING_TO_DROP:
            if (currentTime - dropoffStartTime >= servoMoveDuration) {
                dropoffState = WAITING_AT_DROP;
                dropoffStartTime = currentTime; // Reset the timer
            }
            break;
        case WAITING_AT_DROP:
            if (currentTime - dropoffStartTime >= servoMoveDuration / 2) { // Reduce waiting time at drop position
                servo.write(0); // Move the servo back after waiting
                dropoffState = MOVING_BACK;
                dropoffStartTime = currentTime; // Reset the timer
            }
            break;
        case MOVING_BACK:
            if (currentTime - dropoffStartTime >= servoMoveDuration) {
                driveBackward(); // Start driving backward after the servo has moved back
                dropoffState = DONE;
            }
            break;
        case DONE:
            dropoffState = IDLE;
            break;
        case IDLE:
        default:
            break;
    }
}

bool Car::isDropoffInProgress() {
    return dropoffState != IDLE;
}

int Car::getGate() {
    return gate;
}