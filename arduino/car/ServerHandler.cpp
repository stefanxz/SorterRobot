#include "ServerHandler.h"

ServerHandler::ServerHandler(Car &car) : car(car) {}

void ServerHandler::begin() {
  server.begin();
}

void ServerHandler::handleClient() {
    WiFiClient client = server.available();
    if (!client) return;

    Serial.println("Client connected");

    String line, method, path, body;
    bool headersComplete = false;

    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            line += c;

            // Debug output to see what's being read
            Serial.print(c);

            // Check if we've reached the end of the headers
            if (line.endsWith("\r\n\r\n")) {
                headersComplete = true;
                int firstSpace = line.indexOf(' ');
                int secondSpace = line.indexOf(' ', firstSpace + 1);
                method = line.substring(0, firstSpace);
                path = trimString(line.substring(firstSpace + 1, secondSpace)); // Use trimString to remove spaces
                Serial.print("Method: "); Serial.println(method);
                Serial.print("Path: "); Serial.println(path);

                // Handle the request immediately if it's a GET request
                if (method == "GET") {
                    Serial.println("GET request received.");
                    handleGetRequest(client, path);
                    break;  // Handle only one request at a time
                } else if (method == "POST") {
                    // For POST, further processing might be needed to handle the body
                    body = line.substring(line.indexOf("\r\n\r\n") + 4);  // Assuming body starts immediately after headers
                    handlePostRequest(client, path, body);
                    break;  // Handle only one request at a time
                }

                // Clear the line buffer after processing the request
                line = "";  
            }
        }
    }

    // Ensure the client connection is closed after handling the request
    client.stop();
}

void ServerHandler::handleGetRequest(WiFiClient &client, String &path) {
  if (path == "/") {
    serveHomePage(client);
  }
  // Other GET paths can be handled here later
}

void ServerHandler::handlePostRequest(WiFiClient &client, String &path, String &body)
{
  Serial.println("Post request received.");
  if (path == "/drive")
  {
    handleDriveRequest(client, body);
  }
  // Other POST paths can be handled here later
}

void ServerHandler::serveHomePage(WiFiClient &client)
{
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
}

void ServerHandler::handleDriveRequest(WiFiClient &client, String &body)
{
    Serial.println("Body: " + body);  // Debug: Output the body to see what is received exactly

    // Assuming body is in the form "gate=number"
    int gate = 0;
    int startPos = body.indexOf("gate="); // Find the start position of 'gate='
    if (startPos != -1) {
        startPos += 5;  // Move past 'gate=' to start of the number
        int endPos = body.indexOf('&', startPos); // Check for another parameter
        if (endPos == -1) { // No more parameters, end is the length of the body
            endPos = body.length();
        }
        String gateValue = body.substring(startPos, endPos); // Extract the gate number as a string
        gate = gateValue.toInt();  // Convert the string to an integer
        Serial.print("Extracted Gate: "); Serial.println(gate);  // Debug: Output the extracted gate value
    } else {
        Serial.println("Gate parameter not found.");
    }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("Command received");
}