#include "ServerHandler.h"
#include "Car.h"
#include <Servo.h>

Car car;
ServerHandler serverHandler(car);
Servo servo;
ServerHandler::Request request;

bool isDrivingForward = false;
bool isDrivingBackward = false;
int ignoreLaserCount = 0;  // Counter to keep track of ignored laser detections
int gate = 0;  // Variable to store the gate number

const int laserSensorPin = 4;
const int servoPin = 6;

unsigned long lastLaserDetectedTime = 0;
const unsigned long ignoreDuration = 500; // Duration to ignore subsequent laser detections

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(100);
    }

    pinMode(laserSensorPin, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    servo.attach(servoPin);
    servo.write(0);  // Reset servo position

    WiFi.begin("Stefan’s Iphone (2)", "stefan123"); // Example SSID and password
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
    serverHandler.begin();
}

void loop() {
    request = serverHandler.handleClient();
    if (request == ServerHandler::Request::DRIVE && !isDrivingForward && !isDrivingBackward) {
        gate = car.getGate(); // Get the gate value from the server handler
        ignoreLaserCount = gate;  // Set the number of laser detections to ignore based on the gate
        isDrivingForward = true;
        car.driveForward();
        car.setReady(false);
        Serial.print("Driving forward initiated. Ignoring first ");
        Serial.print(ignoreLaserCount);
        Serial.println(" laser detections.");
    }

    if (isDrivingForward) {
        int laserState = !digitalRead(laserSensorPin);
        if (laserState == HIGH && millis() - lastLaserDetectedTime > ignoreDuration) {
            if (ignoreLaserCount > 0) {
                // Ignore this detection
                Serial.print("Ignoring laser detection, ");
                Serial.print(ignoreLaserCount);
                Serial.println(" remaining.");
                ignoreLaserCount--;
            } else {
                Serial.println("Laser detection after ignoring, stopping.");
                car.stopMotors();
                car.handleDropoff(servo);
                isDrivingForward = false;
                isDrivingBackward = true;
                ignoreLaserCount = gate;  // Reset for backward movement based on the gate
                Serial.print("Starting backward movement. Ignoring first ");
                Serial.print(ignoreLaserCount - 1);
                Serial.println(" laser detections.");
            }
            lastLaserDetectedTime = millis();  // Update last detection time
            digitalWrite(LED_BUILTIN, HIGH);  // Visual indication of detection
        }
    }

    if (isDrivingBackward && !car.isDropoffInProgress()) {
        int laserState = !digitalRead(laserSensorPin);
        if (laserState == HIGH && millis() - lastLaserDetectedTime > ignoreDuration) {
            if (ignoreLaserCount > 0) {
                // Ignore this detection
                Serial.print("Ignoring backward laser detection, ");
                Serial.print(ignoreLaserCount - 1);
                Serial.println(" remaining.");
                ignoreLaserCount--;
            } else {
                Serial.println("Backward laser detection after ignoring, stopping and setting ready.");
                car.stopMotors();
                isDrivingBackward = false;
                car.setReady(true);
                digitalWrite(LED_BUILTIN, LOW);  // Turn off LED to indicate readiness
            }
            lastLaserDetectedTime = millis();  // Reset detection time
        }
    }

    if (!isDrivingForward && !isDrivingBackward) {
        digitalWrite(LED_BUILTIN, LOW);  // Ensure LED is off when not in use
    }

    car.updateServo(servo); // Handle servo movements
}