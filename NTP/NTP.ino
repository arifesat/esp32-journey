#include <WiFi.h>
#include <time.h>

const char* ssid = "ARIFY";
const char* passwd = "arifESAT?";

const char* ntpServer = "pool.ntp.org";

// Turkiye GMT +3
// 3h * 60 min * 60 sec = 10800 sec
const long gmtOffset_sec = 10800;
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, passwd);
  Serial.println("Wi-Fi Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wi-Fi Connected!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Waiting time from server");
}

void loop() {
  printLocalTime();
  delay(1000);
}

void printLocalTime(){
  // tm is a struct that separates current time with date
  struct tm timeinfo; 

  if(!getLocalTime(&timeinfo)){
    Serial.println("Time didn't synced yet");
    return;
  }

  Serial.println(&timeinfo, "%A, %B, %d %Y %H:%M:%S");
}
