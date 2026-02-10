// ESP8266 Server with OTA Upload Mode
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP_EEPROM.h>
#include <ArduinoOTA.h>

#define EEPROM_SIZE 512
#define ADDR_MODE        0
#define ADDR_PASSWORD    2

char ssid[20]     = "JAM_PANEL";
char password[20] = "00000000";

const char* otaSsid = "KELUARGA02";
const char* otaPass = "khusnul23";
const char* otaHost = "SERVER";

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

IPAddress local_IP(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

bool clientReady[5] = { false, false, false, false, false };
bool modeOTA = false;

unsigned long lastTimeSend = 0;
const unsigned long intervalSendTime = 60000; // 1 menit

void getData(String input) {
  Serial.println(input);
  // Di sini bisa tambahkan pengolahan data lebih lanjut
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      clientReady[num] = false;
      break;
    case WStype_DISCONNECTED:
      clientReady[num] = false;
      break;
    case WStype_TEXT: {
      String msg = String((char*)payload);
      if (msg == "CLIENT_READY") {
        clientReady[num] = true;
      } else if (msg == "restart") {
        getData(msg + "=1");
        delay(500);
        ESP.restart();
      } else if (msg == "jadwal") {
        getData(msg + "=1");
      }else {
        getData(msg);
      }
      break;
    }
  }
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
  if (server.hasArg("mode")) {
    data = server.arg("mode"); // Atur status mode
    EEPROM.write(ADDR_MODE, data.toInt());
    EEPROM.commit();
    data = "mode=" + data;
    kirimDataKeClient(data);
    getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
    delay(500);
    ESP.restart();
  }
   if (server.hasArg("PLAY")) {//
    data = server.arg("PLAY"); // Atur status play
    //Serial.println("data mentah: " + data);
    int idx = 0;
    byte folder = getIntPart(data,idx);
    byte file   = getIntPart(data,idx);
    data = "PLAY:" + String(folder) + "," + String(file);
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("PLAD")) {//
    data = server.arg("PLAD"); // Atur status play
    //Serial.println("data mentah: " + data);
    int idx = 0;
    byte file   = getIntPart(data,idx);
    data = "PLAD:" + String(file);
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
   if (server.hasArg("STOP")) {
    data = "STOP";;
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("VOL")) {
    data = server.arg("VOL"); // Atur status mode
    data = "VOL:" + data;
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("HR")) {
    data = server.arg("HR"); // Ambil argumen HR
    kirimDataKeClient("HR:" + data); // (Opsional) Kirim juga ke semua client via WebSocket
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
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("ADZAN")) {//
    data = server.arg("ADZAN"); // Atur status play
    int idx = 0;
    byte file = getIntPart(data,idx);
    int durasi   = getIntPart(data,idx);
    data = "ADZAN:" + String(file) + "," + String(durasi);
    kirimDataKeClient(data);
    //getData(data);
    server.send(200, "text/plain","OK");// (stateBuzzer) ? "Suara Diaktifkan" : "Suara Dimatikan");
  }
  if (server.hasArg("status")) {
    server.send(200, "text/plain", "CONNECTED");
  }
 
  if (server.hasArg("newPassword")) {
      data = server.arg("newPassword");
      data = "newPassword=" + data;
      //Serial.println(data);
      getData(data);
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
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void ONLINE() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(otaSsid, otaPass);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //Serial.println("OTA WiFi gagal. Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname(otaHost);
 
  ArduinoOTA.onEnd([]() {
    Serial.println("restart=1");
    delay(1000);
    ESP.restart();
  });
  
  ArduinoOTA.begin();
  //Serial.println("OTA Ready");
}

void kirimDataKeClient(String data) {

  for (uint8_t i = 0; i < 5; i++) {
    if (clientReady[i] && webSocket.clientIsConnected(i)) {
      webSocket.sendTXT(i, data);
    } else {
      clientReady[i] = false;
    }
  }
}

void cekSerialMonitor() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    //Serial.print("[Serial] Kirim ke semua client: ");
    //Serial.println(input);
    for (uint8_t i = 0; i < 5; i++) {
      if (clientReady[i] && webSocket.clientIsConnected(i)) {
        webSocket.sendTXT(i, input);
      }
    }
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
  modeOTA = EEPROM.read(ADDR_MODE);

  if (modeOTA) {
    EEPROM.write(ADDR_MODE, 0);
    EEPROM.commit();
    ONLINE();
  } else {
    AP_init();
  }
}

void loop() {
  if (modeOTA) {
    ArduinoOTA.handle();
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      if (input.equalsIgnoreCase("restart")) {
        delay(1000);
        ESP.restart();
      }
    }
  } else {
    server.handleClient();
    webSocket.loop();
    cekSerialMonitor();
  }
}
