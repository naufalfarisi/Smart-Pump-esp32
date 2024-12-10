#define BLYNK_TEMPLATE_ID           "TMPL6MFliVGNv"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "XUpcEWhyhwv-ujWHG8XDtryoiw7ZZqCP"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoOTA.h>

// Konfigurasi Wi-Fi
const char* ssid = "wifiddp#2";
const char* password = "ahlanwasahlan";

// Konfigurasi IP Statis
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// Token Blynk
char auth[] = BLYNK_AUTH_TOKEN ;  // Ganti dengan token Anda

// Pin Relay
#define RELAY_PIN 13  // Ganti sesuai pin relay Anda

// LED indikator
#define LED_PIN 2    // Pin LED (bisa gunakan LED_BUILTIN jika sesuai)

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Pastikan relay OFF saat start
  pinMode(LED_PIN, OUTPUT);

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

  // Inisialisasi Blynk
  Blynk.begin(auth, ssid, password);

  // Konfigurasi OTA
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
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
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  
  Serial.println("OTA ready");
}

// Fungsi Blynk untuk mengontrol relay
BLYNK_WRITE(V1) {  // Virtual pin V1
  int relayState = param.asInt();  // Membaca nilai dari aplikasi Blynk
  digitalWrite(RELAY_PIN, relayState);  // Mengubah state relay
  digitalWrite(LED_PIN, relayState);   // LED indikator relay
  Serial.println(relayState ? "Relay ON" : "Relay OFF");
}

void loop() {
  Blynk.run();          // Menjaga koneksi Blynk
  ArduinoOTA.handle();  // Menjaga koneksi OTA
}
