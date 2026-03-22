uint16_t getDurasiTartil(byte folder, uint8_t file) {
  if (folder == 0 || folder > MAX_FOLDER || file >= MAX_FILE) return 0;
  return durasiTartil[folder - 1][file];
}

uint16_t getDurasiAdzan(uint8_t file) {
  if (file == 0 || file >= MAX_FILE) return 0;
  return durasiAdzan[file];
}

void cekDanPutarSholatNonBlocking() {
  if (tartilSedangDiputar || adzanSedangDiputar || sudahEksekusi) return;
  
  RtcDateTime now = Rtc.GetDateTime();
  uint32_t detikSekarang = now.Hour() * 3600UL + now.Minute() * 60UL + now.Second();  // cukup pakai uint16_t

  static bool stateJadwal = false;

  // Cetak hanya sekali pada menit tertentu
  if ((now.Minute() == 0 || now.Minute() == 30) && now.Second() == 0 && !stateJadwal) {
    stateJadwal = true;
    stateSendSholat = 1;
  } else if (now.Second() != 0) {
  stateJadwal = false;
  } 


  for (byte w = 0; w < WAKTU_TOTAL; w++) { 
    
    WaktuConfig &cfg = jadwal[currentDay][w];
    if (!cfg.aktif) continue;
    if (jamSholat[w] == 0 && menitSholat[w] == 0) continue;  // Lewati jadwal tidak valid
    
    uint16_t totalDurasi = 0;
    
    // Hitung total durasi dari file tartil
    for (byte i = 0; i < 5; i++) {
      byte f = cfg.list[i];
      if (f) {
        uint16_t d = getDurasiTartil(cfg.folder,f);
        if (d) totalDurasi += d;
//        Serial.println("f:" + String(f));
//        Serial.println("d:" + String(d));
      }
      
    }

    uint32_t jadwalDetik = jamSholat[w] * 3600UL + menitSholat[w] * 60UL;
    uint32_t triggerDetik = cfg.tartilDulu ? (jadwalDetik - totalDurasi) : jadwalDetik;
    
    
    if (triggerDetik > 86400) continue;  // Lewati jika melebihi 1 hari
//    Serial.println("triggerDetik:" + String(triggerDetik));
//    Serial.println("detikSekarang:" + String(detikSekarang));
   
    if (detikSekarang == triggerDetik) {
      /*/============ DEBUG =============//
      Serial.println("TRIGGER MATCH!");
      Serial.println("jam: " + String(hour()) + " " + "menit: " + String(minute()) + "detik: " + String(second()));
      Serial.println("jamSholat[w]: " + String(jamSholat[w]));
      Serial.println("menitSholat[w]: " + String(menitSholat[w]));
      Serial.println("jadwalDetik: " + String(jadwalDetik));
      Serial.println("totalDurasi: " + String(totalDurasi));
      Serial.println("triggerDetik: " + String(triggerDetik));
      Serial.println("detikSekarang: " + String(detikSekarang));
      //================================/*/
      lcd.setCursor(15,0);
      lcd.write(0);
      dfplayer.volume(volumeDFPlayer);
      digitalWrite(RELAY_PIN, LOW);//relay NYALA
      currentCfg = &cfg;
      lastTriggerMillis = millis();
      sudahEksekusi = true;

      if (cfg.tartilDulu && totalDurasi > 0) {
        tartilIndex = 0;
        tartilFolder = cfg.folder;
        tartilCounter = 0;
        tartilSedangDiputar = true;
        manualSedangDiputar = false;

        byte f = cfg.list[tartilIndex];
        targetDurasi = getDurasiTartil(tartilFolder, f);
        lastTick = millis();
        dfplayer.playFolder(tartilFolder, f); //TARTIL DIMULAI DINOMOR 20

#if DEBUG
        Serial.print("Tartil dimulai: ");
        Serial.println(f);
#endif

      } else if (cfg.aktifAdzan) {
        targetDurasiAdzan = getDurasiAdzan(cfg.fileAdzan);
        adzanCounter = 0;
        lastAdzanTick = millis();
        adzanSedangDiputar = true;
        dfplayer.playFolder(2, cfg.fileAdzan); //ADZAN DIMULAI DINOMOR 20

#if DEBUG
        Serial.print("Adzan langsung diputar: ");
        Serial.println(cfg.fileAdzan);
#endif
      }
    }
  }
}

void cekSelesaiTartil() {
  if (!tartilSedangDiputar) return;

  // Jeda antar file tartil
  if (jedaAktif) {
    if (millis() - jedaMulaiMillis >= JEDA_ANTAR_TARTIL) {
      jedaAktif = false;

      if (tartilIndex < 5) {
        byte f = currentCfg->list[tartilIndex];
        if (f) {
          targetDurasi = getDurasiTartil(tartilFolder, f);
          tartilCounter = 0;
          lastTick = millis();
          dfplayer.playFolder(tartilFolder, f);
#if DEBUG
          Serial.print("Memutar tartil selanjutnya: ");
          Serial.println(f);
#endif
        } else {
          tartilIndex = 5; // skip ke akhir
        }
      } else {
        tartilSedangDiputar = false;
      }
    }
    return;
  }

  // Counter tartil per detik
  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    if (++tartilCounter >= targetDurasi) {
      tartilIndex++;
      if (tartilIndex < 5) {
        if (currentCfg->list[tartilIndex]) {
          jedaAktif = true;
          jedaMulaiMillis = millis();
#if DEBUG
          Serial.println("Menunggu jeda antar file tartil...");
#endif
        } else {
          tartilIndex = 5;
        }
      } else {
        // Tartil selesai
        tartilSedangDiputar = false;
        if (currentCfg->aktifAdzan) {
          adzanCounter = 0;
          targetDurasiAdzan = getDurasiAdzan(currentCfg->fileAdzan);
          lastAdzanTick = millis();
          adzanSedangDiputar = true;
          dfplayer.playFolder(2, currentCfg->fileAdzan);
#if DEBUG
          Serial.println("Tartil selesai, memutar adzan.");
#endif
        } else {
          matikanSemuaAudio();
          //digitalWrite(RELAY_PIN, LOW);
#if DEBUG
          Serial.println("Tartil selesai, relay dimatikan.");
#endif
        }
      }
    }
  }
}


void matikanSemuaAudio() {
  dfplayer.stop();
  lcd.setCursor(15,0);
  lcd.print(" ");
  digitalWrite(RELAY_PIN, HIGH);//relay mati
  relayMenungguMati = false;
  tartilSedangDiputar = false;
  adzanSedangDiputar = false;
  manualSedangDiputar = false;
}

void cekSelesaiAdzan() {
  if (!adzanSedangDiputar) return;

  if (millis() - lastAdzanTick >= 1000) {
    lastAdzanTick = millis();
    adzanCounter++;

    if (adzanCounter >= targetDurasiAdzan) {
      dfplayer.stop();
      digitalWrite(RELAY_PIN, HIGH);//relay mati
      adzanSedangDiputar = false;
      lcd.setCursor(15,0);
      lcd.print(" ");
     // Serial.println("Adzan selesai. Relay dimatikan.");
    }
  }
}
