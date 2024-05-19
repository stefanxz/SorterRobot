#include "ServerHandler.h"
#include "Car.h"

Car car;
ServerHandler serverHandler(car);

void setup() {
  Serial.begin(9600);
  Serial.println("hello");
  WiFi.begin("Holland2Stay", "solutio@365"); // Include actual SSID and password

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  serverHandler.begin();
}

void loop() {
  serverHandler.handleClient();
}
