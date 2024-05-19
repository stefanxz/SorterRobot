#ifndef ServerHandler_h
#define ServerHandler_h

#include "WiFiS3.h"
#include "Car.h"

class ServerHandler {
public:
  ServerHandler(Car &car);
  void begin();
  void handleClient();

private:
  WiFiServer server;
  Car &car;
  void handlePostRequest(WiFiClient &client, String &header);
};

#endif
