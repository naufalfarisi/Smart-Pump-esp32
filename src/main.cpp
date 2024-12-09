#include <WiFi.h>
#include <ArduinoOTA.h>

// Konfigurasi Wi-Fi
const char* ssid = "wifiddp#2";     // Nama Wi-Fi
const char* password = "ahlanwasahlan"; // Password Wi-Fi

// Konfigurasi IP Statis
IPAddress local_IP(192, 168, 1, 184);   // IP statis ESP32
IPAddress gateway(192, 168, 1, 1);      // Gateway (router)
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);       // DNS Primer (opsional)
IPAddress secondaryDNS(8, 8, 4, 4);     // DNS Sekunder (opsional)

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  // Hubungkan ke Wi-Fi dengan IP Statis
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure IP address");
  }
  WiFi.begin(ssid, password);
  
  // Tunggu hingga terhubung ke Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Konfigurasi OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  Serial.println("OTA ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
    ArduinoOTA.handle(); // Periksa dan tangani permintaan OTA

    // Blink LED untuk memverifikasi program berjalan
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

}
