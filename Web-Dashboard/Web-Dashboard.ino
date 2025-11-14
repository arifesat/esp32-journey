#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "ARIFY";
const char* passwd = "arifESAT?";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const int ledPin = 2;
bool ledState = false;

unsigned long bootTime;

extern "C"{
  int temprature_sens_read();
}

float readInternalTempC(){
  int raw = temprature_sens_read();
  return raw - 80;
  //dahili sensor yeterince tutarli olmadigi icin normalize etmeye calisiyoruz
  //sonuclar demo icindir gercek degeri yansitmaz
}

String getStatusJSON(){
  long rssi = WiFi.RSSI(); //Recieved signal strength indicator
  unsigned long uptime = (millis() - bootTime) / 1000;
  float temp = readInternalTempC();

  String json = "{";
  json += "\"led\":\"" + String(ledState ? "ON" : "OFF") + "\",";
  json += "\"uptime\":\"" + String(uptime) + "\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"rssi\":\"" + String(rssi) + "\",";
  json += "\"temp\":\"" + String(temp, 1) + "\"";
  json += "}";
  return json;
} 

void handleRoot(){
  String html = R"====(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 Dashboard</title>
    <style>
      body { font-family: Arial; background: #111; color: white; text-align: center; }
      .card {
        background: #222; padding: 20px; margin: 20px auto; width: 300px;
        border-radius: 12px; box-shadow: 0px 0px 10px #000;
      }
      button {
        background: #0d6efd; color: white; padding: 10px 20px;
        border: none; border-radius: 8px; cursor: pointer; font-size: 18px;
      }
    </style>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>
      let ws = new WebSocket("ws://" + location.host + ":81");

      let tempData = [];
      let rssiData = [];
      let labels = [];

      let ctx = null;
      let chart = null;

      function createChart(){
        ctx = document.getElementById("chart").getContext("2d");
        chart = new Chart(ctx, {
          type: "line",
          data: {
            labels: labels,
            datasets: [
              {label : "RSSI (dBm)", data: rssiData, borderWidth: 2}
            ]
          }
        });
      }

      ws.onmessage = (event) => {
        let data = JSON.parse(event.data);
        document.getElementById("led").innerText = data.led;
        document.getElementById("uptime").innerText = data.uptime + "sec";
        document.getElementById("ip").innerText = data.ip;
        document.getElementById("rssi").innerText = data.rssi + "dBm";
        document.getElementById("temp").innerText = data.temp + "C";
  
        labels.push(new Date().toLocaleTimeString());
        // tempData.push(data.temp);
        rssiData.push(data.rssi);

        if(labels.length > 30){
          labels.shift();
          // tempData.shift();
          rssiData.shift();
        }

        chart.update();
      };

      function scanWifi(){
        let listDiv = document.getElementById("wifi-list");
        listDiv.innerHTML = "Taraniyor...";

        fetch("/scan")
          .then(response => response.json())
          .then(data => {
            listDiv.innerHTML = "";
            if(data.length === 0){
              listDiv.innerHTML = "Ag Bulunamadi.";
              return
            }

            let html = "<ul>";
            data.forEach(net => {
              html += `<li>${net.ssid}(${net.rssi} dBm) - ${net.enc}</li>`;
            });
            html += "</ul>";
            listDiv.innerHTML = html;
          })
          .catch(err => {
            listDiv.innerHTML = "Tarama hatasi: " + err;
          });
      }
    </script>
  </head>
  <body>
    <h1>ESP32 Dashboard</h1>

    <div class="card">
      <h2>LED Durumu: <span id="led">-</span></h2>
      <button onclick="fetch('/toggle')">LED Aç/Kapat</button>
    </div>

    <div class="card">
      <h2>Wi-Fi Taramasi</h2>
      <button onclick="scanWifi()">Ag Tara</button>
      <div id="wifi-list"></div>
    </div>

    <div class="card">
      <h3>IP Adresi:</h3>
      <p id="ip">-</p>

      <h3>Uptime:</h3>
      <p id="uptime">-</p>

      <h3>Wi-Fi Sinyal Gücü:</h3>
      <p id="rssi">-</p>

      <h3> Sıcaklık</h3>
      <p id="temp">-</p>
    </div>
    <canvas id="chart" width="300" height="200"></canvas>
    <script>createChart();</script>

  </body>
  </html>
  )====";

  server.send(200, "text/html; charset=UTF-8", html);
}

void handleToggle(){
  Serial.println("Toggle istegi geldi!");
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);
  server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_CONNECTED){
    Serial.println("WebSocket Connected");
  }
}

void handleScan(){
  Serial.println("Ag tarams istegi...");
  int n = WiFi.scanNetworks();
  Serial.printf("%d ag bulundu. \n", n);
  //Json olustur
  String json = "[";
  //her ag icin girdiler al
  for(int i = 0; i < n; ++i){
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    String enc = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "On" : "Password Secured");

    json += "{\"ssid\":\"" + ssid + "\",";
    json += "\"rssi\":" + String(rssi) + ",";
    json += "\"enc\":\"" + enc + "\"}";
    //bulunan son ag degilse virgul ekle
    if(i < (n - 1)){
      json += ",";
    }
  }
  json += "]";

  server.send(200, "application/json; charset=UTF-8", json);
}

void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  bootTime = millis();

  WiFi.begin(ssid, passwd);
  Serial.print("Connecting to Wi-Fi");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/scan", handleScan);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){
  server.handleClient();

  webSocket.loop();

  static unsigned long lastSend = 0;
  if(millis() - lastSend > 1000){
    lastSend = millis();
    String json = getStatusJSON();
    webSocket.broadcastTXT(json);
  }
}