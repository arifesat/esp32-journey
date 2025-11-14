#include "driver/ledc.h"
#include <WiFi.h>

const char* ssid = "ARIFY";
const char* password = "arifESAT?";

WiFiServer server(80);

const int ledPin = 2;      // Dahili LED pini
const int freq = 5000;     // PWM frekansı
const int resolution = 8;  // 8-bit (0–255)
int brightness = 128;      // Başlangıç parlaklığı

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nBa[laniliyor....]");

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi Connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  // LEDC zamanlayıcıyı ve kanalı ayarla
  ledc_timer_config_t ledTimer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = freq,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&ledTimer);

  ledc_channel_config_t ledChannel = {
    .gpio_num   = ledPin,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = LEDC_CHANNEL_0,
    .intr_type  = LEDC_INTR_DISABLE,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = brightness,
    .hpoint     = 0
  };
  ledc_channel_config(&ledChannel);

  // İlk duty değeri uygula
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void loop() {
  WiFiClient client = server.available();
  if(!client) return;

  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  client.flush();

  Serial.println(req);

  if(req.indexOf("/set?value=") != -1){
    int valStart = req.indexOf('=') + 1;
    int valEnd = req.indexOf(' ', valStart);
    String valStr = req.substring(valStart, valEnd);
    int val = valStr.toInt();

    if(val >= 0 && val <= 255){
      brightness = val;
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
      ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
      Serial.printf("Yeni Parlaklik: %d\n", brightness);
    }
  }

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
  <title>ESP32 LED Kontrol</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { font-family: Arial; text-align: center; }
input[type=range] { width: 80%%; }
</style>
</head>
<body>
  <h2>ESP32 LED Parlaklik Kontrolu</h2>
  <p>Parlaklik: <span id="val">%BRIGHT%</span></p>
  <input type="range" min="0" max="255" value="%BRIGHT%" id="slider" oninput="update(this.value)">
  <script>
    function update(val){
      document.getElementById('val').innerText = val;
      fetch('/set?value=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

  html.replace("%BRIGHT%", String(brightness));

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  client.stop();
}
