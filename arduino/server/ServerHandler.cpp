#include "ServerHandler.h"

ServerHandler::ServerHandler(Car &car) : server(80), car(car) {}

void ServerHandler::begin() {
  // WiFi setup and server begin logic
  server.begin();
}

void ServerHandler::handleClient() {
  WiFiClient client = server.available();
  if (!client) return;

  String header = "";
  bool currentLineIsBlank = true;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;

      if (c == '\n' && currentLineIsBlank) {
        // We have the full header up to a blank line
        handlePostRequest(client, header);
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }
  client.stop();
}

void ServerHandler::handlePostRequest(WiFiClient &client, String &header) {
  // Split the header into lines
  String line;
  while (client.connected() && (line = client.readStringUntil('\n'))) {
    // Check if the line contains the "gate" parameter
    if (line.startsWith("gate=")) {
      // Extract the value of the "gate" parameter
      int index = line.indexOf("=") + 1;
      int gate = line.substring(index).toInt();
      // Use the extracted gate value
      car.driveToGate(gate);
      break; // Exit the loop after processing the "gate" parameter
    }
  }

  // Send HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("Command received");
}

