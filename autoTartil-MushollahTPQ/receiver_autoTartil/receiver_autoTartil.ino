/*
 * PROGRAM RECEIVER UNTUK AUTO TARTIL
 * PARAMETER DATA YANG DIBUTUHKAN 
 * 
 * -TIME:04,00,0,0 
 *      04 :JAM 
 *      00 :MENIT 
 *      0  :DETIK 
 *      0  :HARI
 *      
 * -HR:0|W0:1,1,1,1,1,1-2-3
 *      Hari: Minggu (0)
 *      Waktu: Subuh (0)
 *      Aktif: ya
 *      Aktif Adzan: ya
 *      File Adzan: 3
 *      Tartil sebelum adzan: ya
 *      Folder: 1
 *      List rekaman: 1, 2, 3
 *      
 * -JWS:4,30|12,0|15,30|18,0|19,30
 *      Waktu 0: 4:30
 *      Waktu 1: 12:0
 *      Waktu 2: 15:30
 *      Waktu 3: 18:0
 *      Waktu 4: 19:30
 */

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoOTA.h>
#include <ESP_EEPROM.h>

const char* ssid = "JAM_PANEL";
const char* password = "00000000";

// Untuk OTA mode
const char* ota_ssid = "KELUARGA02";
const char* ota_pass = "suhartono";

WebSocketsClient webSocket;

//unsigned long lastSend = 0;
unsigned long lastWiFiAttempt = 0;
const unsigned long wifiRetryInterval = 5000;

bool wifiConnected = false;
bool wsConnected = false;
bool modeSetting = false;
bool modeR = false;

//variabel untuk led status system
static uint8_t m_Counter = 0;
static uint16_t waveStepDelay = 20;  // Delay antar frame LED breathing (ms)
static uint32_t lastWaveMillis = 0;
//uint32_t lastTimeReceived = 0;
//const uint16_t TIMEOUT_INTERVAL = 70000; // 70 detik, lebih dari 1 menit

#define LED_WIFI 2
#define EEPROM_SIZE 512
#define ADDR_MODE_R 100

// ------------------- EEPROM -------------------
void saveModeR(uint8_t value) {
  EEPROM.write(ADDR_MODE_R, value);
  EEPROM.commit();
}
uint8_t loadModeR() {
  return EEPROM.read(ADDR_MODE_R);
}

// ------------------- WebSocket Event -------------------
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      //Serial.println("[WS] Terputus dari server");
      wsConnected = false;
      break;
    case WStype_CONNECTED:
      //Serial.println("[WS] Terhubung ke server");
      wsConnected = true;
      webSocket.sendTXT("CLIENT_READY");
      break;
    case WStype_TEXT: {
      String msg = String((char*)payload);
      //Serial.println(msg);
      if (msg == "mode=1") {
        saveModeR(1);
        delay(500);
        ESP.restart();
      }else if(msg == "restart"){
        delay(500);
        ESP.restart();
      }else{
        Serial.println(msg);
      }
      break;
    }
  }
}

// ------------------- Arduino OTA Mode -------------------
void startOTAMode() {
  //Serial.println("[OTA] Masuk ke mode OTA Upload");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ota_ssid, ota_pass);

 while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //Serial.println("OTA WiFi gagal. Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("CLIENT");

   ArduinoOTA.onEnd([]() {
    delay(1000);
    ESP.restart();
  });
  ArduinoOTA.begin();
}

// ------------------- Setup -------------------
void setup() {
  Serial.begin(9600);
  pinMode(LED_WIFI, OUTPUT);
  digitalWrite(LED_WIFI, LOW);
  EEPROM.begin(EEPROM_SIZE);
  modeR = loadModeR();
  
  // Cek jika mode OTA aktif
  if (modeR == 1) {
    saveModeR(0); // Reset modeR agar kembali ke normal setelah OTA
    startOTAMode();
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  lastWiFiAttempt = millis();

  Serial.println("⏳ Mencoba koneksi WiFi...");
  Serial.print("MODER: ");Serial.println(modeR);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

// ------------------- Loop -------------------
void loop() {
  // Jika mode OTA aktif, jalankan OTA
  if (modeR == 1) {
    ArduinoOTA.handle();
    getStatusRun();
    return;
  }else{
    checkSerialCommand();
  }
  


  if (!wifiConnected && millis() - lastWiFiAttempt >= wifiRetryInterval) {
    lastWiFiAttempt = millis();

    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;

      if (!modeSetting) {
        webSocket.begin("192.168.2.1", 81, "/");
      }
    } 
  }

  if (wifiConnected && !modeSetting && !modeR) {
    webSocket.loop();
  }

  digitalWrite(LED_WIFI, (wifiConnected && wsConnected && !modeSetting && !modeR) ? HIGH : LOW);
}

// ------------------- Perintah Serial -------------------
void checkSerialCommand() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.equalsIgnoreCase("restart")) {
      if (wsConnected) {
        webSocket.sendTXT("restart");
      } 
    } else if (cmd.equalsIgnoreCase("SETTING")) {
      modeSetting = true;

      if (wsConnected) {
        webSocket.disconnect();
        wsConnected = false;
      }

      if (wifiConnected) {
        WiFi.disconnect();
        wifiConnected = false;
      }

    } else if (cmd.equalsIgnoreCase("NORMAL")) {
      modeSetting = false;

      if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        lastWiFiAttempt = millis();
      }

    }
//    else if (cmd.startsWith("modeR=1")) {
//      saveModeR(1);
//      delay(500);
//      ESP.restart();
//
//    }
    else if (cmd.equalsIgnoreCase("jadwal")) {
      if (wsConnected) {
        webSocket.sendTXT("jadwal");
      } 
    }
  }
}

void getStatusRun() {
  uint32_t now = millis();
  if (now - lastWaveMillis >= waveStepDelay) {
    lastWaveMillis = now;
    updateWaveLED();
  }
}

void updateWaveLED() {
  // brightness naik turun dari 0 - 255 - 0
  uint8_t brightness = (m_Counter < 128) ? m_Counter * 2 : (255 - m_Counter) * 2;
  setLED(brightness);

  m_Counter = (m_Counter + 1) % 256;  // loop kembali ke 0 setelah 255
}

void setLED(uint8_t brightness) {
  analogWrite(LED_WIFI, brightness);
}
