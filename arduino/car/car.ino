#include "ServerHandler.h"
#include "Car.h"

Car car;
ServerHandler serverHandler(car);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
  {
    delay(100);
  }
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
  serverHandler.handleClient();
}