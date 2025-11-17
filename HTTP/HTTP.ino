#include <WiFi.h>
#include <HTTPClient.h> // ESP32'nin internet tarayıcısı gibi düşün

const char* ssid = "ARIFY";
const char* password = "arifESAT?";

// Hedef Adres (Endpoint) - İstanbul saati için API
const char* serverName = "http://api.open-notify.org/iss-now.json";

void setup() {
  Serial.begin(115200);

  // 1. Wi-Fi Bağlantısı
  WiFi.begin(ssid, password);
  Serial.print("Wi-Fi Baglaniyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Baglandi!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());

  // 2. HTTP İsteği Başlatma
  // Normalde bunu loop içinde belirli aralıklarla yaparız ama
  // deneme olduğu için setup içinde bir kere yapıp bırakalım.
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Sunucuya baglaniliyor...");
    
    // Adresi (URL) tanımla
    http.begin(serverName);

    // GET isteği gönder (Tarayıcıda Enter'a basmak gibi)
    int httpResponseCode = http.GET();

    // Cevap geldi mi? (Kod > 0 ise cevap geldi demektir)
    if (httpResponseCode > 0) {
      Serial.print("HTTP Cevap Kodu: ");
      Serial.println(httpResponseCode); // 200: OK, 404: Bulunamadı

      // Gelen veriyi (Payload) al
      String payload = http.getString();
      
      Serial.println("--- GELEN VERI (txt) ---");
      Serial.println(payload);
      Serial.println("-------------------------");
    }
    else {
      Serial.print("Hata Kodu: ");
      Serial.println(httpResponseCode);
    }

    // Bağlantıyı kapat (Memory Leak olmaması için çok önemli)
    http.end();
  }
}

void loop() {
  // Boş
}