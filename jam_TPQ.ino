/*===============================

#feature
-baris atas jam & tulisan berjalan hari nasional+hari jawa,tanggal bulan tahun hijriah dan nasional
-barisan bawah tulisan berjalan "TPQ"

Program V1.0

==================================*/
/*------------------------------------------------------------------------------------
http://<ESP_IP_ADDRESS>/setTime?jam=12:00:00&tanggal=01-01-2024&text1=Hello&text2=Welcome&speedText1=100&speedText2=150&brightness=200&newPassword=newpassword123
-------------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <HJS589.h>

#include <Wire.h>
#include <RtcDS3231.h>
#include <EEPROM.h>

#include <fonts/SystemFont5x7.h>
#include <fonts/Font4x6.h>

//SETUP DMD
#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1

#define BUZZ  3 // PIN BUZZER
#define LED   2 // LED Internal

#define Font0 SystemFont5x7
#define Font1 Font4x6

//////////hijriyah
#define epochHijriah          1948439.5f //math.harvard.edu
#define tambahKurangHijriah   0
#define chijir                0

// Ukuran EEPROM (pastikan cukup untuk semua data)
#define EEPROM_SIZE 512

//create object
RtcDS3231<TwoWire> Rtc(Wire);
HJS589  Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  // Jumlah Panel P10 yang digunakan (KOLOM,BARIS)
RtcDateTime now;
ESP8266WebServer server(80);

const char *pasar[]     ={"WAGE", "KLIWON", "LEGI", "PAHING", "PON"}; 
int maxday[]            = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char *Hari[] =    {"MINGGU","SENIN","SELASA","RABU","KAMIS","JUM'AT","SABTU"};
const char *sholatt[]   = {"SUBUH","TERBIT","DZUHUR","ASHAR","TRBNM","MAGRIB","ISYA"};
char namaBulanHijriah[][14] = {"Muharram", "Safar", "Rabiul awal",
                               "RabiulAkhir", "JumadilAwal", "JumadilAkir", "Rajab",
                               "Sya'ban", "Ramadhan", "Syawal",
                               "Dzulkaidah", "Dzulhijjah"
                              };

int         DWidth  = Disp.width();
int         DHeight = Disp.height();

struct TanggalDanWaktu
{
  uint8_t detik;
  uint8_t menit;
  uint8_t jam;
  uint8_t hari;
  uint8_t tanggal;
  uint8_t bulan;
  uint8_t tahun;
};

struct Tanggal
{
  uint8_t tanggal;
  uint8_t bulan;
  uint16_t tahun;
};

struct TanggalJawa
{
  uint8_t pasaran;
  uint8_t wuku;
  uint8_t tanggal;
  uint8_t bulan;
  uint16_t tahun;
};

struct JamDanMenit
{
  uint8_t jam;
  uint8_t menit;
};

TanggalDanWaktu tanggalMasehi;
Tanggal tanggalHijriah;
TanggalJawa tanggalJawa;
JamDanMenit waktuMagrib;

// Pengaturan hotspot WiFi dari ESP8266
 char ssid[20] = "JAM_PANEL";
 char password[20] = "00000000";

// Variabel untuk waktu, tanggal, teks berjalan, dan kecerahan
String setJam = "00:00:00";
String setTanggal = "01-01-2024";
String setText = "Selamat Datang!";
int brightness = 100;
static char text[200];
int speedDate = 70; // Kecepatan default text 1
int speedText = 60; // Kecepatan default text 2

//----------------------------------------------------------------------
// HJS589 P10 FUNGSI TAMBAHAN UNTUK NODEMCU ESP8266

void ICACHE_RAM_ATTR refresh() { 
  
  Disp.refresh();
  timer0_write(ESP.getCycleCount() + 80000);  

}

void Disp_init() {
  EEPROM.begin(100); // Inisialisasi EEPROM dengan ukuran yang ditentukan
  Disp.start();
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(refresh);
  timer0_write(ESP.getCycleCount() + 80000);
  interrupts();
  Disp.clear();
  Disp.setBrightness(20);
  Serial.println("Setup dmd selesai");
}

void AP_init(){
  // Konfigurasi hotspot WiFi dari ESP8266
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Atur server untuk menerima permintaan set waktu, tanggal, teks, dan kecerahan
  server.on("/setTime", handleSetTime);
  server.begin();
  Serial.println("Server dimulai.");  
}
  
// Fungsi untuk mengatur jam, tanggal, running text, dan kecerahan
void handleSetTime(){
  Serial.println("hansle run");
  bool saveRequired = false;
  if (server.hasArg("jam")) {
    setJam = server.arg("jam"); 
    Serial.println("setJam:"+String(setJam));
    RtcDateTime now = Rtc.GetDateTime();
    int jam   = setJam.substring(0, 2).toInt();
    int menit = setJam.substring(3, 5).toInt();
    int detik = setJam.substring(6, 8).toInt();
    Rtc.SetDateTime(RtcDateTime(now.Year(), now.Month(), now.Day(), jam, menit, detik));
    saveRequired = true;
  } 
  if (server.hasArg("tanggal")) {
    setTanggal = server.arg("tanggal"); 
    Serial.println(String()+"setTanggal:"+setTanggal);

    //Serial.println(String()+"setTanggal:"+setTanggal);
    RtcDateTime now = Rtc.GetDateTime();
    int day   = setTanggal.substring(0, 2).toInt();    // Ambil 2 karakter pertama (hari)
    int month = setTanggal.substring(3, 5).toInt();  // Ambil karakter 4 dan 5 (bulan)
    int year  = setTanggal.substring(6, 10).toInt();  // Ambil karakter 7 sampai 10 (tahun)
    Rtc.SetDateTime(RtcDateTime(year, month, day, now.Hour(), now.Minute(), now.Second()));
    saveRequired = true;  
  }
  if (server.hasArg("text")) {
    setText = server.arg("text"); 
    
    setText.toCharArray(text,setText.length()+1);
    Serial.println(String()+"setText:"+setText);
    Serial.println(String()+"text   :"+text);
//    saveStringToEEPROM(25, setText);
//    EEPROM.commit();
  }
  if (server.hasArg("brightness")) {
    //Serial.println(String()+"brightness:"+brightness);
    brightness = server.arg("brightness").toInt(); 
    Serial.println(String()+"brightness:"+brightness);
    Disp.setBrightness(brightness);
    //EEPROM.write(0, brightness);
    //EEPROM.commit();

  }
  if (server.hasArg("speedDate")) {
    speedDate = server.arg("speedDate").toInt(); // Atur kecepatan text 1
    Serial.println(String()+"speedDate:"+speedDate);
    //EEPROM.write(5, speedDate);
    //EEPROM.commit();

  }
  if (server.hasArg("speedText")) {
    speedText = server.arg("speedText").toInt(); // Atur kecepatan text 2
    Serial.println(String()+"speedText:"+speedText);
    //EEPROM.write(10, speedText);
    //EEPROM.commit();
  }
  if (server.hasArg("newPassword")) {
    String newPassword = server.arg("newPassword");
    Serial.println(String()+"newPassword:"+newPassword);
    newPassword.toCharArray(password, newPassword.length() + 1); // Set password baru
    WiFi.softAP(ssid, password); // Restart AP dengan password baru
//    saveStringToEEPROM(15, password); // Simpan password AP
//    EEPROM.commit();
    server.send(200, "text/plain", "Password AP berhasil diubah, silakan sambungkan ulang!");
    delay(2000);
    ESP.restart(); // Restart ESP untuk menerapkan perubahan AP
  } //EEPROM.commit();
    //EEPROM.end();
    server.send(200, "text/plain", "Pengaturan berhasil diupdate dan disimpan ke EEPROM!");
}


// Fungsi untuk menyimpan string ke EEPROM
//void saveStringToEEPROM(int startAddr, String data) {
//  char buff[100]; // Buffer untuk string yang akan dibaca
//  int size = data.length()+1;
//  data.toCharArray(buff, size);
//  Serial.println(buff);
//  EEPROM.put(startAddr,buff);
//  data="";
//}
// Fungsi untuk menyimpan string ke EEPROM
void saveStringToEEPROM(int startAddr, const String &data) {
  int len = data.length();
  for (int i = 0; i < len; i++) {
    EEPROM.write(startAddr + i, data[i]);
  }
  EEPROM.write(startAddr + len, '\0'); // Null terminator
}

// Fungsi untuk membaca string dari EEPROM
String readStringFromEEPROM(int startAddr) {
  char data[100]; // Buffer untuk string yang akan dibaca
  int len = 0;
  unsigned char k;
  k = EEPROM.read(startAddr);
  while (k != '\0' && len < 100) { // Membaca hingga null terminator
    data[len] = k;
    len++;
    k = EEPROM.read(startAddr + len);
  }
  data[len] = '\0';
  return String(data);
}

// Simpan semua data ke EEPROM
void saveToEEPROM() {}
  // saveStringToEEPROM(0, setJam);
  // saveStringToEEPROM(50, setTanggal);
  
  //delay(100);
  
  //delay(100);
  
  //delay(100);
  
  //delay(100);
  
  

// Membaca semua data dari EEPROM
void loadFromEEPROM() {
  // setJam = readStringFromEEPROM(0);
  // setTanggal = readStringFromEEPROM(50);
  setText = readStringFromEEPROM(0);
  brightness = EEPROM.read(100);
  speedDate = EEPROM.read(200);
  speedText = EEPROM.read(300);
  String loadedPassword = readStringFromEEPROM(400); // Baca password dari EEPROM
  loadedPassword.toCharArray(password, loadedPassword.length() + 1); // Set password AP
  // Serial.println(String()+"setJam        :"+setJam);
  // Serial.println(String()+"setTanggal    :"+setTanggal);
  Serial.println(String()+"setText       :"+setText);
  Serial.println(String()+"brightness    :"+brightness);
  Serial.println(String()+"speedDate     :"+speedDate);
  Serial.println(String()+"speedText     :"+speedText);
  Serial.println(String()+"loadedPassword:"+loadedPassword);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZ, OUTPUT); 
  pinMode(LED, OUTPUT);
  
  
 
   // Load data dari EEPROM
  //loadFromEEPROM();

  int rtn = I2C_ClearBus(); // clear the I2C bus first before calling Wire.begin()
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
  for(int i = 0; i < 2; i++){
      Buzzer(1);
      delay(80);
      Buzzer(0);
      delay(80);
  }
  Disp_init();
  AP_init();
}
byte tampilan=1;
void loop() {

  server.handleClient(); // Menangani permintaan dari MIT App Inventor
  
  islam();
  
  runningInfo();
  switch(tampilan){
    case 1 :
     runAnimasiJam();
    break;
    case 2 :
      
     runAnimasiDate();
    break;
  };
  
  
}




void Buzzer(uint8_t state)
  {
    if(state ==1 ) //dapat dikasih kondisi jika diwaktu tertentu buzzer tidak aktif
      {digitalWrite(BUZZ,HIGH);}
    else 
      {digitalWrite(BUZZ,LOW);}
  }

void fType(int x)
  {
    if(x==0)       Disp.setFont(Font0);
    else if(x==1)   Disp.setFont(Font1); 
    // else if(x==2)  Disp.setFont(Font2);
    // else if(x==3)  Disp.setFont(Font3);
    // else if(x==4)  Disp.setFont(Font4);
    //  else if(x==5) Disp.setFont(Font5);
    // else if(x==6)  Disp.setFont(Font6);  
  }

 //----------------------------------------------------------------------
// I2C_ClearBus menghindari gagal baca RTC (nilai 00 atau 165)

int I2C_ClearBus() {
  
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
  int clockCount = 20; // > 2x9 clock

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
    int counter = 20;
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
