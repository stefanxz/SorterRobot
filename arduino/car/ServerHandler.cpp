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
        while (client.available()) {
            char c = client.read();
            line += c;

            // Capture body as soon as headers are complete
            if (headersComplete) {
                if (method == "POST") {
                    // Continue reading the body until no more data is available
                    body += c;
                }
            }

            // Check if we've reached the end of the headers
            if (!headersComplete && line.endsWith("\r\n\r\n")) {
                headersComplete = true;
                int firstSpace = line.indexOf(' ');
                int secondSpace = line.indexOf(' ', firstSpace + 1);
                method = line.substring(0, firstSpace);
                path = trimString(line.substring(firstSpace + 1, secondSpace));
                Serial.print("Method: "); Serial.println(method);
                Serial.print("Path: "); Serial.println(path);

                if (method == "POST") {
                    // Prepare to capture the body immediately after the headers
                    int bodyStartIndex = line.indexOf("\r\n\r\n") + 4;
                    body = line.substring(bodyStartIndex);
                }

                // Clear the line buffer after headers to prevent re-processing
                line = "";
            }
        }

        // Handle the request based on the method
        if (headersComplete) {
            if (method == "GET") {
                Serial.println("Handling GET request.");
                handleGetRequest(client, path);
                break;  // Exit after handling
            } else if (method == "POST") {
                Serial.println("Handling POST request.");
                handlePostRequest(client, path, body);
                break;  // Exit after handling
            }
        }
    }

    // Ensure the client connection is closed after handling the request
    Serial.println("Closing client connection.");
    client.stop();
}


void ServerHandler::handleGetRequest(WiFiClient &client, String &path) {
  if (path == "/") {
    serveHomePage(client);
  }
  // Other GET paths can be handled here later
  if (path == "/ready") {
    handleIsReadyRequest(client);
  }
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

void ServerHandler::handleIsReadyRequest(WiFiClient &client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("Connection: close");
  client.println();
  if (car.isReady()){
    client.println("1: car ready");
  } else {
    client.println("0: car not ready");
  }
}

void ServerHandler::handleDriveRequest(WiFiClient &client, String &body)
{
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


  car.driveToGate(gate);
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("Command received");
}