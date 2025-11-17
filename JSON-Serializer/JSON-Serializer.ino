#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "ARIFY";
const char* passwd = "arifESAT?";
const char* serverName = "http://api.open-notify.org/iss-now.json";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, passwd);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected");

  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName);
    int httpResponseCode = http.GET();

    if(httpResponseCode > 0){
      String payload = http.getString();

      // Allocate memory for JSON data
      // This allocates 1024 bytes of memory
      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if(error){
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return;
      }

      float latitude = doc["iss_position"]["latitude"].as<float>();
      float longitude = doc["iss_position"]["longitude"].as<float>();
      long timestamp = doc["timestamp"];
      
      Serial.println("----------------------");
      Serial.println("ISS Location Report");
      Serial.print("Latitude: ");
      Serial.println(latitude, 4);
      Serial.print("Longitude: ");
      Serial.println(longitude, 4);
      Serial.print("Time (unix): ");
      Serial.println(timestamp);
    }else{
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

void loop() {}
