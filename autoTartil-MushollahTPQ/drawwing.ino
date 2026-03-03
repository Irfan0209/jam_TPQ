char* jadwal[] = {"IMSAK","SUBUH", "TERBT", "DUHUR", "ASHAR", "MAGRB", "ISYA'"};

void showDisplay(){
  static uint32_t saveTimer = 0;
  static uint8_t sNum = 0;
  
  float sholatT[]={JWS.floatImsak,JWS.floatSubuh,JWS.floatTerbit,JWS.floatDzuhur,JWS.floatAshar,JWS.floatMaghrib,JWS.floatIsya};

  if(millis() - saveTimer > 1000){
    saveTimer = millis();
    sNum = (sNum + 1) % 7;
  }
  // ===== SHOLAT =====
  float st = sholatT[sNum];
  uint8_t hh = (uint8_t)st;
  uint8_t mm = (uint8_t)((st - hh) * 60);

  char timeBuf[15];
  snprintf(timeBuf, sizeof(timeBuf), %s "%02d:%02d", jadwal[sNum], hh, mm);

  lcd.setCursor(0,0);
  lcd.print(timeBuf);

}
