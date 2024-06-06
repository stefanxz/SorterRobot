#include "ServerHandler.h"
#include "Car.h"
#include <Servo.h>

Car car;
ServerHandler serverHandler(car);
Servo servo;
ServerHandler::Request request;

bool isDrivingForward = false;
bool isDrivingBackward = false;
bool ignoreFirstLaser = true;

const int laserSensorPin = 4;
const int servoPin = 6;

unsigned long ignoreEndTimeBackward = 0;
unsigned long ignoreEndTimeForward = 0;
const unsigned long ignoreDuration = 500; // ignore for 500 milliseconds

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    delay(100);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(servoPin);
  servo.write(0);

  WiFi.begin("Kleon", "22222222"); // Include actual SSID and password
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  serverHandler.begin();
}

void loop() {
    request = serverHandler.handleClient();
    if (request == ServerHandler::Request::DRIVE) {
        isDrivingForward = true;
        ignoreFirstLaser = true;
        car.setReady(false);
        ignoreEndTimeForward = millis() + ignoreDuration;
    }

    if (isDrivingForward) {
        if (digitalRead(laserSensorPin) == HIGH && !ignoreFirstLaser && millis() > ignoreEndTimeForward) {
            car.stopMotors();
            car.handleDropoff(servo);
            isDrivingForward = false;
            isDrivingBackward = true;
            ignoreFirstLaser = true;
            ignoreEndTimeBackward = millis() + ignoreDuration;
            digitalWrite(LED_BUILTIN, LOW); 
        } else if (digitalRead(laserSensorPin) == HIGH && ignoreFirstLaser) {
            ignoreFirstLaser = false;
            ignoreEndTimeForward = millis() + ignoreDuration;
            digitalWrite(LED_BUILTIN, HIGH); 
        }
    }

    if (isDrivingBackward) {
        if (digitalRead(laserSensorPin) == HIGH && !ignoreFirstLaser && millis() > ignoreEndTimeBackward) {
            car.stopMotors();
            isDrivingBackward = false;
            ignoreFirstLaser = true;
            ignoreEndTimeForward = millis() + ignoreDuration;
            digitalWrite(LED_BUILTIN, LOW); 
            car.setReady(true);
        } else if (digitalRead(laserSensorPin) == HIGH && ignoreFirstLaser) {
            ignoreFirstLaser = false;
            ignoreEndTimeBackward = millis() + ignoreDuration;
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }
}