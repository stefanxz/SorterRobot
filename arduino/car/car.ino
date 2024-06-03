#include "ServerHandler.h"
#include "Car.h"
#include <Servo.h>

Car car;
ServerHandler serverHandler(car);
Servo servo;
ServerHandler::Request request;

bool isDrivingForward = false;
bool isDrivingBackward = false;
const int laserSensorPin = 4;
const int servoPin = 6;

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

void loop()
{
  request = serverHandler.handleClient();
  if (request == ServerHandler::Request::DRIVE) {
    isDrivingForward = true;
  }

  if (isDrivingForward && digitalRead(laserSensorPin) == HIGH) {
    car.handleDropoff(servo);
    isDrivingForward = false;
    isDrivingBackward = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (isDrivingBackward && digitalRead(laserSensorPin) == HIGH) {
    car.stopMotors();
    isDrivingBackward = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
}