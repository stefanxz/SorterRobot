#include "WiFiS3.h"  // Ensure this library supports your hardware and WiFi functions similar to WiFiNINA

char ssid[] = "Holland2Stay";  // Enter your WIFI SSID
char pass[] = "solutio@365";   // Enter your WIFI password

WiFiServer server(80);

void setup() {
  Serial.begin(9600); // Initialize serial communication
  if (WiFi.begin(ssid, pass) == WL_CONNECTED) {
    Serial.println("Connected to the WiFi network");
    server.begin();  // Start the web server on port 80
    printWifiStatus();
  } else {
    Serial.println("Failed to connect to WiFi");
    while (true);  // Stay here forever since we cannot connect to WiFi
  }
}

void loop() {
  webServer();
}

void webServer() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {
    Serial.println("New Client.");          // Print a message out in the serial port
    String currentLine = "";                // Make a String to hold incoming data from the client
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);                    // Debug output of the read character
        if (c == '\n' || c == '\r') {
          Serial.println(currentLine);      // Debug output of the current line
          // Check if the current line is blank, indicating the end of the client's HTTP request
          if (currentLine.startsWith("GET /kleon ")) {
            sendCustomMessage(client);
            break;
          } else if (currentLineIsBlank) {
            sendHelloPage(client);
            break;
          }
          currentLine = ""; // Clear the current line after processing
        } else {
          currentLine += c; // Add it to the end of the currentLine
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void sendCustomMessage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain");
  client.println("Connection: close");
  client.println();
  client.println("I am a simple Albanian man");
  client.println();
}

void sendHelloPage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<title>Hello Page</title></head>");
  client.println("<body><h1>Hello</h1></body></html>");
  client.println();
}

void printWifiStatus() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("Open this URL in your browser to view the page: http://");
  Serial.println(ip);
}
