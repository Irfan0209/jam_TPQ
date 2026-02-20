// ESP8266 Server with OTA Upload Mode
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP_EEPROM.h>

#define EEPROM_SIZE 512
#define ADDR_MODE        0
#define ADDR_PASSWORD    2

#define LED_WIFI 2


char ssid[20]     = "JAM_PANEL";
char password[20] = "00000000";

//const char* otaSsid = "KELUARGA02";
//const char* otaPass = "khusnul23";
//const char* otaHost = "SERVER";

unsigned long lastWiFiAttempt = 0;
const unsigned long wifiRetryInterval = 5000;
bool wifiConnected = false;

ESP8266WebServer server(80);

IPAddress local_IP(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

//unsigned long lastTimeSend = 0;
//const unsigned long intervalSendTime = 60000; // 1 menit

void getData(String input) {
  Serial.println(input);
  // Di sini bisa tambahkan pengolahan data lebih lanjut
}

void handleSetTime() {
  String data = "";
   if (server.hasArg("Tm")) {
    data = server.arg("Tm");
    data = "Tm=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Settingan jam berhasil diupdate");
  }
  if (server.hasArg("text")) {
    data = server.arg("text");
    data = "text=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Settingan text berhasil diupdate");
  }
  if (server.hasArg("name")) {
    data = server.arg("name");
    data = "name=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Settingan nama berhasil diupdate");
  }
  if (server.hasArg("Br")) {
    data  = server.arg("Br");
    data = "Br=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Kecerahan berhasil diupdate");
  }
  if (server.hasArg("Spdt")) {
    data = server.arg("Spdt"); // Atur kecepatan date
    data = "Spdt=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Kecepatan kalender berhasil diupdate");
  }
  if (server.hasArg("Sptx1")) {
    data = server.arg("Sptx1"); // Atur kecepatan text
    data = "Sptx1=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Kecepatan info 1 berhasil diupdate");
  }
  if (server.hasArg("Sptx2")) {
    data = server.arg("Sptx2"); // Atur kecepatan text
    data = "Sptx2=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Kecepatan info 2 berhasil diupdate");
  }
  if (server.hasArg("Spnm")) {
    data = server.arg("Spnm"); // Atur kecepatan text
    data = "Spnm=" + data;
   // Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Kecepatan nama berhasil diupdate");
  }
  if (server.hasArg("Iq")) {
    data = server.arg("Iq"); // Atur koreksi iqomah
    data = "Iq=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"iqomah diupdate");
  }
  if (server.hasArg("Dy")) {
    data = server.arg("Dy"); // Atur durasi adzan
    data = "Dy=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"displayBlink diupdate");
  }
  if (server.hasArg("Kr")) {
    data = server.arg("Kr"); // Atur koreksi waktu jadwal sholat
    data = "Kr=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"Selisih jadwal sholat diupdate");
  }
  if (server.hasArg("Lt")) {
    data = server.arg("Lt"); // Atur latitude
    data = "Lt=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"latitude diupdate");
  }
  if (server.hasArg("Lo")) {
    data = server.arg("Lo"); // Atur latitude
    data = "Lo=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"longitude diupdate");
  }
  if (server.hasArg("Tz")) {
    data = server.arg("Tz"); // Atur latitude
    data = "Tz=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"timezone diupdate");
  }
  if (server.hasArg("Al")) {
    data = server.arg("Al"); // Atur latitude
    data = "Al=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"altitude diupdate");
  }
  if (server.hasArg("Da")) { 
    data = server.arg("Da"); 
    data = "Da=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");// "durasi adzan diupdate");
  }
  if (server.hasArg("CoHi")) {
    data = server.arg("CoHi"); // Atur latitude    data = "CoHi=" + data;
    data = "CoHi=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain", "OK");//"coreksi hijriah diupdate");
  }

  if (server.hasArg("Bzr")) {
    data = server.arg("Bzr"); // Atur status buzzer
    data = "Bzr=" + data;
    //Serial.println(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
//  if (server.hasArg("mode")) {
//    data = server.arg("mode"); // Atur status mode
//    // EEPROM.write(ADDR_MODE, data.toInt());
//    // EEPROM.commit();
//    data = "mode=" + data;
//    getData(data);
//    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
//    delay(500);
//    ESP.restart();
//  }
  if (server.hasArg("Ms")) {
    data = server.arg("Ms"); // Atur status mode show
    data = "Ms=" + data;
    getData(data);
    server.send(200, "text/plain","OK");// (mode show) 
  }
  if (server.hasArg("At")) {
    data = server.arg("At"); //ON/OFF AUTO TARTIL
    //autoTartilEnable = data;
    getData(data);
    server.send(200, "text/plain","OK");// 
   // saveToEEPROM();
  }
  if (server.hasArg("Vc")) {
    data = server.arg("Vc"); //ON/OFF VOICE CLOCK
    //voiceClock = data;
    getData(data);
    server.send(200, "text/plain","OK");// 
    //saveToEEPROM();
  }
   if (server.hasArg("PLAY")) {//
    data = server.arg("PLAY"); // Atur status play
    //Serial.println("data mentah: " + data);
    int idx = 0;
    byte folder = getIntPart(data,idx);
    byte file   = getIntPart(data,idx);
    data = "PLAY:" + String(folder) + "," + String(file);
   // parseData(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("PLAD")) {//
    data = server.arg("PLAD"); // Atur status play
    //Serial.println("data mentah: " + data);
    int idx = 0;
    byte file   = getIntPart(data,idx);
    data = "PLAD:" + String(file);
    //parseData(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
   if (server.hasArg("STOP")) {
    data = "STOP";
    //parseData(data);
    
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("VOL")) {
    data = server.arg("VOL"); // Atur status mode
    data = "VOL:" + data;
    //parseData(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("HR")) {
    data = server.arg("HR"); // Ambil argumen HR
    //parseData("HR:" + data); // (Opsional) Kirim juga ke semua client via WebSocket
    getData(data);
    server.send(200, "text/plain", "OK");
  }
  if (server.hasArg("NAMAFILE")) {//
    data = server.arg("NAMAFILE"); // Atur status play
    //Serial.println("data mentah: " + data);
    int idx = 0;
    byte folder = getIntPart(data,idx);
    byte file   = getIntPart(data,idx);
    int durasi = getIntPart(data,idx);
    data = "NAMAFILE:" + String(folder) + "," + String(file)+ "," + String(durasi);
    //parseData(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("ADZAN")) {//
    data = server.arg("ADZAN"); // Atur status play
    int idx = 0;
    byte file = getIntPart(data,idx);
    int durasi   = getIntPart(data,idx);
    data = "ADZAN:" + String(file) + "," + String(durasi);
    //parseData(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("status")) {
    data = "status=1" ;
    getData(data);
    server.send(200, "text/plain", "CONNECTED");
  }
 
  if (server.hasArg("newPassword")) {
      data = server.arg("newPassword");
      data.toCharArray(password, data.length() + 1);
      getData(data);
      //saveToEEPROM();
      server.send(200, "text/plain","OK");// "Password WiFi diupdate");
    } 
  data="";
  //EEPROM.commit();
}

void AP_init() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  server.on("/setPanel", handleSetTime);
  server.begin();

}

void cekSerialMonitor() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    //Serial.print("[Serial] Kirim ke semua client: ");
    //Serial.println(input);
  }
}

int getIntPart(String &s, int &pos) {
  int comma = s.indexOf(',', pos);
  if (comma == -1) comma = s.length();
  int val = s.substring(pos, comma).toInt();
  pos = comma + 1;
  return val;
}

void setup() {
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE);
  pinMode(LED_WIFI, OUTPUT);
  digitalWrite(LED_WIFI, LOW);
  
  AP_init();

}

void loop() {
    server.handleClient();
    cekSerialMonitor();

//    if (!wifiConnected && millis() - lastWiFiAttempt >= wifiRetryInterval) {
//    lastWiFiAttempt = millis();
//
//    if (WiFi.status() == WL_CONNECTED) {
//      wifiConnected = true;
//    } 
//  }
//  digitalWrite(LED_WIFI, (wifiConnected ) ? HIGH : LOW);

}

//void getStatusRun() {
//  uint32_t now = millis();
//  if (now - lastWaveMillis >= waveStepDelay) {
//    lastWaveMillis = now;
//    updateWaveLED();
//  }
//}
//
//void updateWaveLED() {
//  // brightness naik turun dari 0 - 255 - 0
//  uint8_t brightness = (m_Counter < 128) ? m_Counter * 2 : (255 - m_Counter) * 2;
//  setLED(brightness);
//
//  m_Counter = (m_Counter + 1) % 256;  // loop kembali ke 0 setelah 255
//}
//
//void setLED(uint8_t brightness) {
//  analogWrite(LED_WIFI, brightness);
//}
