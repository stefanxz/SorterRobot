#ifndef Car_h
#define Car_h

class Car
{
public:
  Car();                                // Constructor to initialize the pins and setup
  void driveForward(int milliseconds);  // Drive forward for a specified number of milliseconds
  void driveForward();
  void driveBackward(int milliseconds); // Drive backward for a specified number of milliseconds
  void driveToGate(int gate);           // Drive to a specific gate (implementation needed)
  void stopMotors();                    // Stops both motors
  void setSpeed(int newSpeed);          // Sets a new speed for the motors

  bool isReady();

private:
  void setupMotors(); // Setup motor pins as outputs

  // Motor pins
  const int motor1Pin1 = 7;
  const int motor1Pin2 = 8;
  const int motor1PWM = 9;

  const int motor2Pin1 = 10;
  const int motor2Pin2 = 11;
  const int motor2PWM = 12;

  int speed = 75; // Default speed, mutable to change at runtime
  bool ready = true;
};

#endif