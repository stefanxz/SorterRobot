#ifndef ServerHandler_h
#define ServerHandler_h

#include <WiFiS3.h> 
#include "Car.h"

class ServerHandler {
public:
  ServerHandler(Car &car);
  enum Request {DRIVE, READY, OTHER, NONE, ERROR};


  void begin();
  Request handleClient();
  Request handleIsReadyRequest(WiFiClient &client);

  String trimString(String str) {
    if (str.length() == 0) return str;

    // Trim leading spaces
    int start = 0;
    while (start < str.length() && isSpace(str[start])) {
        start++;
    }

    // Trim trailing spaces
    int end = str.length() - 1;
    while (end > 0 && isSpace(str[end])) {
        end--;
    }

    if (end < start) return "";  // All spaces
    return str.substring(start, end + 1);
  }

bool isSpace(char c) {
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

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