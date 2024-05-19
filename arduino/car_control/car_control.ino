// Define motor 1 pins
const int motor1Pin1 = 7;
const int motor1Pin2 = 8;
const int motor1PWM = 9;

// Define motor 2 pins
const int motor2Pin1 = 10;
const int motor2Pin2 = 11;
const int motor2PWM = 12;

void setup() {
  // Set motor 1 pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);

  // Set motor 2 pins as outputs
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
}

void loop() {
  // Run motor 1 forward at half speed
  for(int i = 0; i < 5; i++) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(motor1PWM, 255); // Speed range is 0 to 255

    // Run motor 2 backward at half speed
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor2PWM, 255); // Speed range is 0 to 255

    delay(2000); // Run for 2 seconds

    // Stop both motors
    stopMotors();

    delay(1000); // Wait for 1 second
  }
}

void stopMotors() {
  // Stop motor 1
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(motor1PWM, 0);

  // Stop motor 2
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor2PWM, 0);
}
