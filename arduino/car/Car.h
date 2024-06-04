#ifndef Car_h
#define Car_h

#include <Servo.h>

class Car
{
public:
  Car();                                // Constructor to initialize the pins and setup
  void driveForward(int milliseconds);  // Drive forward for a specified number of milliseconds
  void driveForward();
  void driveBackward(int milliseconds); // Drive backward for a specified number of milliseconds
  void driveBackward();
  void driveToGate(int gate);  
  void stopMotors();  
  void setSpeed(int newSpeed);
  bool isReady();
  void handleDropoff(Servo &servo);
  void setReady(bool car_ready);

private:
  void setupMotors();

  // Motor pins
  const int motor1Pin1 = 7;
  const int motor1Pin2 = 8;
  const int motor1PWM = 9;

  const int motor2Pin1 = 10;
  const int motor2Pin2 = 11;
  const int motor2PWM = 12;

  int speed = 75;
  bool ready = true;
};

#endif