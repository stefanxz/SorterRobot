#include "ServerHandler.h"

ServerHandler::ServerHandler(Car &car) : car(car) {}

void ServerHandler::begin() {
  server.begin();
}

ServerHandler::Request ServerHandler::handleClient() {
  WiFiClient client = server.available();
  if (!client) return Request::NONE;

  Serial.println("Client connected");

  String line, method, path, body;
  bool currentLineIsBlank = true;
  bool isPost = false;
  ServerHandler::Request request;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLineIsBlank && isPost) {
          // If it's a POST request and the headers are done, read the body
          body = client.readStringUntil('\n');
          request = handlePostRequest(client, path, body);
          break;
        }
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
      line += c;
      if (line.endsWith("\r\n") && method.isEmpty()) {
        // This should only run once, the first line containing the method and path
        int firstSpace = line.indexOf(' ');
        int secondSpace = line.indexOf(' ', firstSpace + 1);
        method = line.substring(0, firstSpace);
        path = line.substring(firstSpace + 1, secondSpace);
        isPost = method.equals("POST");
        line = ""; // Reset for next line
      }
    }
  }
  if (method == "GET") {
    request = handleGetRequest(client, path);
  }
  client.stop();
  return request;
}

ServerHandler::Request ServerHandler::handleGetRequest(WiFiClient &client, String &path) {
  if (path == "/") {
    return serveHomePage(client);
  }
  // Other GET paths can be handled here later
}

ServerHandler::Request ServerHandler::handlePostRequest(WiFiClient &client, String &path, String &body) {
  Serial.println("Post request received.");
  if (path == "/drive") {
    return handleDriveRequest(client, body);
  }
  // Other POST paths can be handled here later
}

ServerHandler::Request ServerHandler::serveHomePage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><title>Arduino Car Controller</title></head>");
  client.println("<body><h1>Welcome to the Arduino Car Controller</h1>");
  client.println("<form action=\"/drive\" method=\"post\">");
  client.println("<input type=\"text\" name=\"gate\" placeholder=\"Enter gate number\">");
  client.println("<input type=\"submit\" value=\"Drive\">");
  client.println("</form></body></html>");

  return Request::OTHER;
}

ServerHandler::Request ServerHandler::handleDriveRequest(WiFiClient &client, String &body) {
  int startPos = body.indexOf("gate=") + 5;
  int endPos = body.indexOf('&', startPos);
  if (endPos == -1) endPos = body.length();
  int gate = body.substring(startPos, endPos).toInt();
  car.driveToGate(gate);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("Command received");

  return Request::DRIVE;
}