#include <WiFi.h>

const char* ssid = "ARIFY";       // 🔹 Replace with your Wi-Fi name
const char* password = "arifESAT?"; // 🔹 Replace with your Wi-Fi password

WiFiServer server(80);     // Create a web server on port 80
const int ledPin = 2;      // Change if your LED is on a different GPIO

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the web server
}

void loop() {
  WiFiClient client = server.available(); // Wait for a client to connect
  if (!client) return;

  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();
  Serial.println(request);

  if (request.indexOf("/ON") != -1) {
    digitalWrite(ledPin, HIGH);
  } else if (request.indexOf("/OFF") != -1) {
    digitalWrite(ledPin, LOW);
  }

  // Send a web page response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>ESP32 LED Control</title></head>");
  client.println("<body style='text-align:center;'>");
  client.println("<h1>💡 ESP32 LED Control</h1>");
  client.println("<p><a href=\"/ON\"><button style='font-size:20px;'>ON</button></a></p>");
  client.println("<p><a href=\"/OFF\"><button style='font-size:20px;'>OFF</button></a></p>");
  client.print("<p>Current LED state: ");
  client.print(digitalRead(ledPin) ? "ON" : "OFF");
  client.println("</p>");
  client.println("</body></html>");
  client.println();
}
