#ifndef ServerHandler_h
#define ServerHandler_h

#include <WiFiS3.h> 
#include "Car.h"

class ServerHandler {
public:
  enum Request {READY, DRIVE, OTHER, NONE};
  
  ServerHandler(Car &car);
  void begin();
  Request handleClient();
private:
  WiFiServer server;
  Car &car;
  
  Request serveHomePage(WiFiClient &client);
  Request handlePostRequest(WiFiClient &client, String &path, String &body);
  Request handleDriveRequest(WiFiClient &client, String &body);
  Request processRequest(WiFiClient &client, String &header);
  Request handleGetRequest(WiFiClient &client, String &path);
};

#endif // ServerHandler_h