#ifndef ServerHandler_h
#define ServerHandler_h

#include <WiFiS3.h> 
#include "Car.h"

class ServerHandler {
public:
  ServerHandler(Car &car);
  void begin();
  void handleClient();

private:
  WiFiServer server;
  Car &car;
  
  void serveHomePage(WiFiClient &client);
  void handlePostRequest(WiFiClient &client, String &path, String &body);
  void handleDriveRequest(WiFiClient &client, String &body);
  void processRequest(WiFiClient &client, String &header);
  void handleGetRequest(WiFiClient &client, String &path);
};

#endif // ServerHandler_h
