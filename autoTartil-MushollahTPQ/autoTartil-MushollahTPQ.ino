
//LIBRARY UNTUK AUTO TARTIL
#include <DFRobotDFPlayerMini.h>
#include <EEPROM.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <Prayer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>

//EEPROM AUTO TARTIL
#define EEPROM_SIZE 3000

#define dfSerial Serial2

#define PASSWORD_LEN 20   // maksimal 15 karakter + '\0'
//KONFIGURASI WIFI
char ssid[PASSWORD_LEN]     = "JAM_PANEL";
char password[PASSWORD_LEN] = "00000000";

const char* otaSsid = "KELUARGA02";
const char* otaPass = "khusnul23";
const char* otaHost = "SERVER";

//create object
RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime now;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Prayer JWS;
Hijriyah Hijir;

DFRobotDFPlayerMini dfplayer;

WebServer server(80);

IPAddress local_IP(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);


//PIN IO AUTO TARTIL
#define RELAY_PIN         26
#define RUN_LED           13
#define NORMAL_STATUS_LED 14
#define BUZZ              27

// OneButton UP(BUTTON_UP, false);
// OneButton DOWN(BUTTON_DOWN, false);

//VARIABEL PARAMETER UNTUK AUTO TARTIL
#define HARI_TOTAL  8 // 7 hari + SemuaHari (index ke-7)
#define WAKTU_TOTAL 5
#define MAX_FILE    30
#define MAX_FOLDER  3
#define JEDA_ANTAR_TARTIL 20 //500 jeda antar file tartil dalam milidetik

//#define DEBUG 1

struct WaktuConfig {
  byte aktif;
  byte aktifAdzan;
  byte fileAdzan;
  byte tartilDulu;
  byte folder;
  byte list[5];
};

enum Show{
  ANIM_CLOCK,
  ANIM_ADZAN
};
Show show = ANIM_CLOCK;


uint8_t    DWidth        = 16;
uint8_t    DHeight       = 2;

//================== VARIABEL AUTO TARTIL =============================//
WaktuConfig jadwal[HARI_TOTAL][WAKTU_TOTAL];
uint8_t durasiAdzan[MAX_FILE];
uint16_t durasiTartil[MAX_FOLDER][MAX_FILE];
byte volumeDFPlayer ;
uint8_t jamSholat[WAKTU_TOTAL]; //= {4, 12, 15, 18, 19};
uint8_t menitSholat[WAKTU_TOTAL];// = {30, 0, 30, 0, 30};

bool tartilSedangDiputar = false;
uint32_t tartilMulaiMillis = 0;
byte tartilFolder = 0;
byte tartilIndex = 0;

uint16_t tartilCounter = 0;
uint16_t targetDurasi = 0;
uint32_t lastTick = 0;

bool jedaAktif = false;
uint32_t jedaMulaiMillis = 0;

WaktuConfig *currentCfg = nullptr;

uint32_t lastTriggerMillis = 0;
bool sudahEksekusi = false;
bool adzanSedangDiputar = false;
uint32_t adzanMulaiMillis = 0;
uint16_t adzanDurasi = 0;

uint32_t lastAdzanTick = 0;
uint16_t adzanCounter = 0;
uint16_t targetDurasiAdzan = 0;

byte currentDay = 0;

// Tambahan untuk relay delay dan manual
uint32_t relayOffDelayMillis = 0;
bool relayMenungguMati = false;
bool manualSedangDiputar = false;
bool adzanManualSedangDiputar = false;

//variabel untuk led status system
static uint8_t m_Counter = 0;
constexpr uint16_t waveStepDelay = 20;  // Delay antar frame LED breathing (ms)
static uint32_t lastWaveMillis = 0;
bool STATUS_MODE = false;
bool lastStatusMode = !STATUS_MODE;     // agar langsung update saat pertama kali
bool lastNormalStatus = false;
uint32_t lastTimeReceived = 0;
constexpr uint32_t TIMEOUT_INTERVAL = 70000; // 70 detik, lebih dari 1 menit
//===================== END ==========================//

uint8_t dataIhty[]      = {0,0,0,0,0,10};

struct Config {
  uint8_t durasiadzan = 60;
  uint8_t altitude = 5;
  double latitude = -7.364057;
  double longitude = 112.646222;
  uint8_t zonawaktu = 7;
  int8_t Correction = -1; //Koreksi tanggal hijriyah, -1 untuk mengurangi, 0 tanpa koreksi, 1 untuk menambah
  bool       stateBuzzer;//   = 1;
};
Config config;

bool      stateSendSholat = false; 
bool      stateRestart = false;
int8_t    sholatNow     = -1;
bool      adzan         = 0;
bool      reset_x       = 0;
bool      autoTartilEnable = true;
bool      voiceClock = true; 

byte ikonSpeaker[8] = {
  0b00001, //      *
  0b00011, //     **
  0b01111, //   ****
  0b01111, //   ****
  0b01111, //   ****
  0b00011, //     **
  0b00001, //      *
  0b00000  //
};


void getData(const String& input) {
  const char* data = input.c_str();
  const char* eq_ptr = strchr(data, '=');

  if (eq_ptr != nullptr) {
    size_t key_len = eq_ptr - data; // Mengambil panjang kunci
    const char* ptr = eq_ptr + 1;   // Pointer untuk membaca nilai

   if (key_len == 2 && strncmp(data, "Kr", 2) == 0) {
      uint8_t indexSholat = atoi(ptr);
      ptr = strchr(ptr, '-'); if (!ptr) return; ptr++;
      uint8_t indexKoreksi = atoi(ptr);
      dataIhty[indexSholat] = indexKoreksi;
      saveToEEPROM();
    } 
    else if (key_len == 6 && strncmp(data, "status", 6) == 0) {
      bool state = atoi(ptr);
      if (state) {
        for (byte i = 0; i < 3; i++) {
          Buzzer(1); delay(80);
          Buzzer(0); delay(80);
        }
        Serial.println(F("PANEL_OK"));
      }
    }   
    else if (key_len == 6 && strncmp(data, "jadwal", 6) == 0) {
      stateSendSholat = atoi(ptr);
    } 
  }
}

void handleSetTime() {
  char dataBuffer[250];
  
   if (server.hasArg("Tm")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Tm=%s", server.arg("Tm").c_str());
     parseData(dataBuffer);
    server.send(200, "text/plain", "OK");//"Settingan jam berhasil diupdate");
    return;
  }
  if (server.hasArg("Kr")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Kr=%s", server.arg("Kr").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"Selisih jadwal sholat diupdate");
    return;
  }
  if (server.hasArg("Lt")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Lt=%s", server.arg("Lt").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"latitude diupdate");
    return;
  }
  if (server.hasArg("Lo")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Lo=%s", server.arg("Lo").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"longitude diupdate");
    return;
  }
  if (server.hasArg("Tz")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Tz=%s", server.arg("Tz").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"timezone diupdate");
    return;
  }
  if (server.hasArg("Al")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Al=%s", server.arg("Al").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"altitude diupdate");
    return;
  }
  if (server.hasArg("Da")) { 
    snprintf(dataBuffer, sizeof(dataBuffer), "Da=%s", server.arg("Da").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");// "durasi adzan diupdate");
    return;
  }
  if (server.hasArg("CoHi")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "CoHi=%s", server.arg("CoHi").c_str());
    getData(dataBuffer);
    server.send(200, "text/plain", "OK");//"coreksi hijriah diupdate");
    return;
  }
  if (server.hasArg("PLAY")) {
    // 1. Ambil data mentah sebagai String hanya untuk parsing
    String mentah = server.arg("PLAY"); 
    
    int idx = 0;
    byte folder = getIntPart(mentah, idx);
    byte file   = getIntPart(mentah, idx);

    // 2. Format data langsung ke dalam dataBuffer
    // %d adalah placeholder untuk integer/byte
    snprintf(dataBuffer, sizeof(dataBuffer), "PLAY:%d,%d", folder, file);

    // 3. Kirim data yang sudah rapi di buffer ke client
    parseData(dataBuffer); 

    server.send(200, "text/plain", "OK");
    return; // Keluar dari fungsi agar lebih efisien
}

// --- PLAD ---
  if (server.hasArg("PLAD")) {
    String mentah = server.arg("PLAD");
    int idx = 0;
    byte file = getIntPart(mentah, idx);
    snprintf(dataBuffer, sizeof(dataBuffer), "PLAD:%d", file);
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- STOP ---
  if (server.hasArg("STOP")) {
    parseData("STOP"); // Langsung kirim teks statis
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- VOL ---
  if (server.hasArg("VOL")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "VOL:%s", server.arg("VOL").c_str());
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- HR ---
  if (server.hasArg("HR")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "HR:%s", server.arg("HR").c_str());
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- NAMAFILE ---
  if (server.hasArg("NAMAFILE")) {
    String mentah = server.arg("NAMAFILE");
    int idx = 0;
    byte folder = getIntPart(mentah, idx);
    byte file   = getIntPart(mentah, idx);
    int durasi  = getIntPart(mentah, idx);
    snprintf(dataBuffer, sizeof(dataBuffer), "NAMAFILE:%d,%d,%d", folder, file, durasi);
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- ADZAN ---
  if (server.hasArg("ADZAN")) {
    String mentah = server.arg("ADZAN");
    int idx = 0;
    byte file  = getIntPart(mentah, idx);
    int durasi = getIntPart(mentah, idx);
    snprintf(dataBuffer, sizeof(dataBuffer), "ADZAN:%d,%d", file, durasi);
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- At ---
  if (server.hasArg("At")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "At:%s", server.arg("At").c_str());
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }

  // --- Vc ---
  if (server.hasArg("Vc")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "Vc:%s", server.arg("Vc").c_str());
    parseData(dataBuffer);
    server.send(200, "text/plain", "OK");
    return;
  }
  
  if (server.hasArg("status")) {
    snprintf(dataBuffer, sizeof(dataBuffer), "%s","status=1");
    getData(dataBuffer);
    server.send(200, "text/plain", "CONNECTED");
     return;
  }
 
 if (server.hasArg("newPassword")) {
      // 1. Ambil password baru dari argumen server
      String passwordBaru = server.arg("newPassword");

      // 2. Format untuk kirim ke serial/monitor (dataBuffer)
      snprintf(dataBuffer, sizeof(dataBuffer), "newPassword:%s", passwordBaru.c_str());
      
      // 3. Simpan ke variabel global 'pass' untuk digunakan fungsi Restart nanti
      // Kita langsung isi, tidak perlu ditambah-tambah (+) agar tidak menumpuk
     // pass = dataBuffer; 

      parseData(dataBuffer);

      server.send(200, "text/plain", "OK");
      return;
  }
}

int getIntPart(String &s, int &pos) {
  int comma = s.indexOf(',', pos);
  if (comma == -1) comma = s.length();
  int val = s.substring(pos, comma).toInt();
  pos = comma + 1;
  return val;
}

void AP_init() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid,password);

  server.on("/setPanel", handleSetTime);
  server.begin();
 
}



void setup() {
  EEPROM.begin(EEPROM_SIZE);//
  digitalWrite(RELAY_PIN, HIGH); // Awal mati
  pinMode(RUN_LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(NORMAL_STATUS_LED, OUTPUT);
  Buzzer(1);
  
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, ikonSpeaker);

  dwCtr(0,0,"AUTO TARTIL");
  dwCtr(0,1,"V1");
  
  uint8_t rtn = I2C_ClearBus(); // clear the I2C bus first before calling Wire.begin()
    if (rtn != 0) {
      Serial.println(F("I2C bus error. Could not clear"));
      if (rtn == 1) {
        Serial.println(F("SCL clock line held low"));
      } else if (rtn == 2) {
        Serial.println(F("SCL clock line held low by slave clock stretch"));
      } else if (rtn == 3) {
        Serial.println(F("SDA data line held low"));
      }
    } 
    else { // bus clear, re-enable Wire, now can start Wire Arduino master
      Wire.begin();
    }

  Rtc.Begin();
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  
  delay(1000);
  Buzzer(0);
  Serial.begin(9600);
  dfSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
 
  if (!dfplayer.begin(dfSerial,/*isACK = */true, /*doReset = */true)) {
    Serial.println("DFPlayer tidak terdeteksi!");
    while (1);
  }
  loadFromEEPROM();
  delay(1000);
  AP_init();
  dfplayer.enableDAC(); // Pakai output DAC (line out)
  Serial.println("Sistem Auto Tartil Siap.");
  delay(50);
  dfplayer.volume(volumeDFPlayer);
  digitalWrite(RELAY_PIN, HIGH); // Awal mati
  lcd.clear();

}

void loop() {
  if (sudahEksekusi && millis() - lastTriggerMillis > 60000) {
    sudahEksekusi = false;
  }
  server.handleClient();
  bacaDataSerial();
  cekDanPutarSholatNonBlocking();
  cekSelesaiTartil();
  cekSelesaiAdzan();
  cekSelesaiAdzanManual();
  cekSelesaiManual();
  getStatusRun();
  islam();
  check();
  
  switch(show){
    case ANIM_CLOCK : 
      showDisplay();
    break;

    case ANIM_ADZAN :
      drawAzzan();
    break;
  };
  //checkHourlyChime();

}

/*/ --- Fungsi cek bunyi jam & setengah jam ---
void checkHourlyChime() {
  if(!voiceClock) return;

 static int lastHalfPlay = -1;

  // Bunyi jam tepat
  if (now.Minute() == 0 && now.Second() == 0 && now.Hour() != lastHalfPlay && tartilSedangDiputar==false && adzanSedangDiputar==false && manualSedangDiputar == false) {
    lastHalfPlay = hour();
    uint8_t jam = hour() % 12;
    if (jam == 0) { jam = 12; }
    dfplayer.volume(volumeDFPlayer);
    dfplayer.play(jam);  // Folder 1 = suara jam 1-12
  }
}*/


void bacaDataSerial() {
  // Booking memori statis sebesar 512 byte (sesuaikan jika data lebih panjang)
  static char buffer[50]; 
  static uint16_t index = 0;

  while (Serial.available() > 0) {
    char c = Serial.read();

    // FILTER 1: Hanya terima karakter teks yang valid (ASCII 32 sampai 126)
    // Karakter sampah/noise (seperti '⸮') akan otomatis diabaikan.
    if (c >= 32 && c <= 126) {
      if (index < sizeof(buffer) - 1) { // Pelindung dari Buffer Overflow
        buffer[index++] = c;
      }
    }
    // FILTER 2: Tanda pesan selesai (Newline '\n' atau Carriage Return '\r')
    else if (c == '\n' || c == '\r') {
      if (index > 0) {
        buffer[index] = '\0'; // Kunci teks dengan null-terminator

        // --- DEBUG PINTU MASUK ---
        // Serial.print(F("\n>> Memanggil parseData() dengan Teks Bersih: '"));
        // Serial.print(buffer);
        // Serial.println(F("'"));
        // -------------------------

        parseData(buffer); // Kirim teks ke fungsi pembongkar data
        index = 0;         // Reset index ke 0 untuk siap menerima pesan baru
      }
    }
  }
}

// 1. Ubah parameter dari String menjadi const char*
void parseData(const char* data) {
  Serial.print(F("data=")); Serial.println(data);
  lastTimeReceived = millis();

  // --- Parsing TIME: ---

  const char* eq_ptr = strchr(data, '='); // Cari posisi '='
  
  if (eq_ptr != nullptr) {
    size_t key_len = eq_ptr - data; // Hitung panjang huruf sebelum '='
    const char* ptr = eq_ptr + 1;   // Mulai baca nilai/angka setelah '='

//    Serial.print(F("[DEBUG 2] Ditemukan '='. Panjang Key (seharusnya 2): "));
//    Serial.println(key_len);
    
    // Mengecek apakah command sebelum '=' adalah "Tm" (panjang 2 karakter)
    // strncmp sangat cepat di CPU dan tidak memakan RAM tambahan
    if (key_len == 2 && strncmp(data, "Tm", eq_ptr - data) == 0) {
      
      const char* ptr = eq_ptr + 1; // Mulai baca dari setelah tanda '='

      // Parsing menggunakan atoi() dan mencari delimiter dengan strchr()
      // Jika format tidak sesuai (strchr return null), fungsi akan return untuk mencegah error/crash
      
      uint8_t jam = atoi(ptr);
      ptr = strchr(ptr, ':'); if (!ptr) return; ptr++; // Lompat ke setelah ':'

      uint8_t menit = atoi(ptr);
      ptr = strchr(ptr, ':'); if (!ptr) return; ptr++; // Lompat ke setelah ':'

      uint8_t detik = atoi(ptr);
      ptr = strchr(ptr, '-'); if (!ptr) return; ptr++; // Lompat ke setelah '-'

      uint8_t tanggal = atoi(ptr);
      ptr = strchr(ptr, '-'); if (!ptr) return; ptr++; // Lompat ke setelah '-'

      uint8_t bulan = atoi(ptr);
      ptr = strchr(ptr, '-'); if (!ptr) return; ptr++; // Lompat ke setelah '-'

      uint16_t tahun = atoi(ptr);

      // Tampilkan hasil
//      Serial.println(tahun);
//      Serial.println(bulan);
//      Serial.println(tanggal);
//      Serial.println(jam);
//      Serial.println(menit);
//      Serial.println(detik);

      Rtc.SetDateTime(RtcDateTime(tahun, bulan, tanggal, jam, menit, detik));
      stateSendSholat = true;
      return;
    }
  }

  // --- Parsing VOL: ---
  else if (strncmp(data, "VOL:", 4) == 0) {
    volumeDFPlayer = atoi(data + 4);

    Serial.print(F("[DEBUG VOL] Volume diubah ke: ")); 
    Serial.println(volumeDFPlayer);
    
    dfplayer.volume(volumeDFPlayer);
    saveToEEPROM();
    return;
  }

  // --- Parsing HR: --- (Jadwal harian)
  // Format lama: HR:hari|W0:aktif,aktifAdzan,fileAdzan,tartilDulu,folder-list0-list1-list2-list3-list4
  else if (strncmp(data, "HR:", 3) == 0) {
    const char* ptr = data + 3;
    
    int hari = atoi(ptr);
    if (hari < 0 || hari >= HARI_TOTAL) return;

    for (int w = 0; w < WAKTU_TOTAL; w++) {
      char tag[8];
      snprintf(tag, sizeof(tag), "|W%d:", w);
      
      const char* w_ptr = strstr(data, tag);
      if (w_ptr == nullptr) continue;

      w_ptr += strlen(tag); // Lompat ke nilai setelah tag
      WaktuConfig &cfg = jadwal[hari][w];

      Serial.print(F("  -> Waktu [")); Serial.print(w);
      Serial.print(F("] Aktif:")); Serial.print(cfg.aktif);
      Serial.print(F(", Adzan:")); Serial.print(cfg.aktifAdzan);
      Serial.print(F(", FileAdzan:")); Serial.print(cfg.fileAdzan);
      Serial.print(F(", Tartil:")); Serial.print(cfg.tartilDulu);
      Serial.print(F(", Folder:")); Serial.print(cfg.folder);
      Serial.print(F(", ListFile: "));

      // Ambil 5 nilai berurutan (asumsi dipisah koma atau karakter non-angka)
      cfg.aktif      = atoi(w_ptr); while (*w_ptr && *w_ptr >= '0' && *w_ptr <= '9') w_ptr++; if (*w_ptr) w_ptr++;
      cfg.aktifAdzan = atoi(w_ptr); while (*w_ptr && *w_ptr >= '0' && *w_ptr <= '9') w_ptr++; if (*w_ptr) w_ptr++;
      cfg.fileAdzan  = atoi(w_ptr); while (*w_ptr && *w_ptr >= '0' && *w_ptr <= '9') w_ptr++; if (*w_ptr) w_ptr++;
      cfg.tartilDulu = atoi(w_ptr); while (*w_ptr && *w_ptr >= '0' && *w_ptr <= '9') w_ptr++; if (*w_ptr) w_ptr++;
      cfg.folder     = atoi(w_ptr); while (*w_ptr && *w_ptr >= '0' && *w_ptr <= '9') w_ptr++; if (*w_ptr) w_ptr++;

      // Ambil list file (dipisah dengan dash '-')
      for (int i = 0; i < 5; i++) {
        cfg.list[i] = atoi(w_ptr);

        Serial.print(cfg.list[i]); Serial.print(F("-"));
        
        const char* dash = strchr(w_ptr, '-');
        if (dash) {
          w_ptr = dash + 1;
        } else {
          break; // Keluar dari loop jika tidak ada dash lagi
        }
        Serial.println(); // Enter setelah selesai 1 waktu
      }
    }
    saveToEEPROM();
    return;
  }

  // --- Parsing PLAY: ---
  else if (strncmp(data, "PLAY:", 5) == 0) {
    const char* ptr = data + 5;
    byte folder = atoi(ptr);
    while (*ptr && *ptr >= '0' && *ptr <= '9') ptr++; if (*ptr) ptr++;
    byte file   = atoi(ptr);

    Serial.print(F("[DEBUG PLAY] Folder: ")); Serial.print(folder);
    Serial.print(F(", File: ")); Serial.print(file);

    if (folder >= 1 && folder < 12 && file >= 1 && file < MAX_FILE) {
      uint16_t durasi = durasiTartil[folder - 1][file];
      if (durasi > 0) {
        dfplayer.volume(volumeDFPlayer);
        dfplayer.playFolder(folder,file);
        lcd.setCursor(15,0);
        lcd.write(0);
        digitalWrite(RELAY_PIN, LOW); // Relay NYALA
        tartilCounter       = 0;
        targetDurasi        = durasi;
        lastTick            = millis();
        manualSedangDiputar = true;
      }
    }
    return;
  }

  // --- Parsing PLAD: ---
  else if (strncmp(data, "PLAD:", 5) == 0) {
    byte file = atoi(data + 5);
    uint16_t durasi = durasiAdzan[file];

    Serial.print(F("[DEBUG PLAD] Play Adzan Manual File: ")); Serial.print(file);
    Serial.print(F(", Durasi Target: ")); Serial.println(durasi);
    
    if (durasi > 0) {
      dfplayer.volume(volumeDFPlayer);
      dfplayer.playFolder(2,file);
      lcd.setCursor(15,0);
      lcd.write(0);
      digitalWrite(RELAY_PIN, LOW); // Relay NYALA
      adzanCounter             = 0;
      targetDurasiAdzan        = durasi;
      lastAdzanTick            = millis();
      adzanManualSedangDiputar = true;
    }
    return;
  }

  // --- Parsing STOP ---
  else if (strncmp(data, "STOP", 4) == 0) {
    dfplayer.stop();
    lcd.setCursor(15,0);
    lcd.print(" ");
    digitalWrite(RELAY_PIN, HIGH); // Relay MATI
    tartilSedangDiputar = false;
    adzanSedangDiputar  = false;
    manualSedangDiputar = false;
    Serial.println(F("[DEBUG STOP] DFPlayer & Relay telah DIMATIKAN secara paksa."));
    return;
  }

  // --- Parsing NAMAFILE: ---
  else if (strncmp(data, "NAMAFILE:", 9) == 0) {
    const char* ptr = data + 9;
    byte folder = atoi(ptr);
    while (*ptr && *ptr >= '0' && *ptr <= '9') ptr++; if (*ptr) ptr++;
    byte list   = atoi(ptr);
    while (*ptr && *ptr >= '0' && *ptr <= '9') ptr++; if (*ptr) ptr++;
    int durasi  = atoi(ptr);

    Serial.print(F("[DEBUG NAMAFILE] Durasi Tartil Disimpan -> Folder: ")); Serial.print(folder);
    Serial.print(F(", List: ")); Serial.print(list);
    Serial.print(F(", Durasi: ")); Serial.println(durasi);

    if (folder < MAX_FOLDER && list < MAX_FILE) {
      durasiTartil[folder][list] = durasi;
      saveToEEPROM();
    }
    return;
  }

  // --- Parsing ADZAN: ---
  else if (strncmp(data, "ADZAN:", 6) == 0) {
    const char* ptr = data + 6;
    byte file  = atoi(ptr);
    while (*ptr && *ptr >= '0' && *ptr <= '9') ptr++; if (*ptr) ptr++;
    int durasi = atoi(ptr);

    Serial.print(F("[DEBUG ADZAN] Durasi Adzan Disimpan -> File: ")); Serial.print(file);
    Serial.print(F(", Durasi: ")); Serial.println(durasi);

    if (file < MAX_FILE) {
      durasiAdzan[file] = durasi;
      saveToEEPROM();
    }
    return;
  }

  // --- Parsing JWS: ---
  // Format asumi: JWS:jam1,menit1|jam2,menit2|jam3,menit3...
  else if (strncmp(data, "JWS:", 4) == 0) {
    const char* ptr = data + 4;
    for (int i = 0; i < WAKTU_TOTAL; i++) {
      jamSholat[i] = atoi(ptr);
      ptr = strchr(ptr, ',');
      if (!ptr) break;
      ptr++; // Lewati koma
      
      menitSholat[i] = atoi(ptr);

      Serial.print(jamSholat[i]); Serial.print(F(":")); 
      Serial.print(menitSholat[i]); Serial.print(F(" | "));
      
      ptr = strchr(ptr, '|');
      if (!ptr) break; // Jika tidak ada pemisah lagi, selesai
      ptr++; // Lewati pipa
    }
    saveToEEPROM();
  }

  // --- Parsing At: --- (Auto Tartil)
  else if (strncmp(data, "At:", 3) == 0) {
    autoTartilEnable = atoi(data + 3);

    Serial.print(F("[DEBUG At] Status Auto Tartil diubah menjadi: ")); 
    Serial.println(autoTartilEnable ? F("AKTIF") : F("NONAKTIF"));
    saveToEEPROM();
    return;
  }

  // --- Parsing newPassword= ---
  else if (strncmp(data, "newPassword:", 12) == 0) {
    const char* pwd = data + 12;

    Serial.print(F("[DEBUG PASSWORD] Request ubah password ke: '"));
    Serial.print(pwd);
    Serial.println(F("'"));
    
    if (strlen(pwd) == 8) {
      strncpy(password, pwd, 9); // Copy 8 karakter + null terminator
      saveToEEPROM();
      delay(1000);
      ESP.restart();
    } else {
      Serial.println(F("Password invalid (harus 8 karakter)"));
    }
    return;
  }

  // Pemanggilan default jika ada data yang lolos tidak di-return sebelumnya
  // saveToEEPROM(); (Aktifkan jika memang setiap data yang tak teridentifikasi harus memicu simpan)
}

void cekSelesaiManual() {
if (manualSedangDiputar) {
  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    tartilCounter++;
    //Serial.print("Counter: "); Serial.println(tartilCounter);
    
    if (tartilCounter >= targetDurasi) {
      dfplayer.stop();
      lcd.setCursor(15,0);
      lcd.print(" ");
      digitalWrite(RELAY_PIN, HIGH);//relay mati
      manualSedangDiputar = false;
      //Serial.println("Manual tartil selesai.");
    }
  }
}
}

void cekSelesaiAdzanManual() {
  if (adzanManualSedangDiputar) 
{
  if (millis() - lastAdzanTick >= 1000) {
    lastAdzanTick = millis();
    adzanCounter++;

    if (adzanCounter >= targetDurasiAdzan) {
      dfplayer.stop();
      lcd.setCursor(15,0);
      lcd.print(" ");
      digitalWrite(RELAY_PIN, HIGH);//relay mati
      adzanManualSedangDiputar = false;
      adzanCounter=0;
     // Serial.println("Adzan selesai. Relay dimatikan.");
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
  analogWrite(RUN_LED, brightness);
}

uint8_t I2C_ClearBus() {

#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master.
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  uint8_t clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
    // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    uint8_t counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok
}

void Buzzer(uint8_t state)
  {
    if(!config.stateBuzzer) return;
    
     switch(state){
      case 0 :
        noTone(BUZZ);
      break;
      case 1 :
        tone(BUZZ,2000);
      break;
    };
  }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
