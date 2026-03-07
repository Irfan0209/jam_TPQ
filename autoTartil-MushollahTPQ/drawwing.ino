char* Jadwal[] = {"IMSAK","SUBUH", "TERBT", "DUHUR", "ASHAR", "MAGRB", "ISYA'"};

void showDisplay(){
  static uint32_t saveTimer = 0;
  static uint8_t sNum = 0;
  RtcDateTime now = Rtc.GetDateTime();
  
  float sholatT[]={JWS.floatImsak,JWS.floatSubuh,JWS.floatTerbit,JWS.floatDzuhur,JWS.floatAshar,JWS.floatMaghrib,JWS.floatIsya};

  if(millis() - saveTimer > 3000){
    saveTimer = millis();
    sNum = (sNum + 1) % 7;
  }
  // ===== SHOLAT =====
  float st = sholatT[sNum];
  uint8_t hh = (uint8_t)st;
  uint8_t mm = (uint8_t)((st - hh) * 60);

  char timeBuf[15];
  snprintf(timeBuf, sizeof(timeBuf), "%s %02d:%02d", Jadwal[sNum], hh, mm);

  char tm[10];
  snprintf(tm, sizeof(tm), "%02d:%02d:%02d",now.Hour(), now.Minute(), now.Second());

  lcd.setCursor(0,0);
  lcd.print(timeBuf);

  lcd.setCursor(0,1);
  lcd.print(tm);

}
