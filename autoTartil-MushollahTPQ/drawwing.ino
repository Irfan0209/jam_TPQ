char* Jadwal[] = {"IMSAK ","SUBUH ", "TERBIT", "DZUHUR", "ASHAR ", "MAGRIB", "ISYA' "};
char* jadwalAzzan[] = {"SUBUH","DZUHUR", "ASHAR", "MAGRIB", "ISYA'"};

void showDisplay(){
  static uint32_t saveTimer = 0;
  static uint8_t sNum = 0;
  static uint32_t sv = 0;

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

  char tm[6];
  snprintf(tm, sizeof(tm), (now.Second()%2)?"%02d:%02d":"%02d %02d",now.Hour(), now.Minute());

  char dt[12];
  snprintf(dt, sizeof(dt), "%02d:%02d:%04d",now.Day(), now.Month(), now.Year());

  lcd.setCursor(0,0);
  lcd.print(timeBuf);
  
  lcd.setCursor(0,1);
  lcd.print(tm);

  lcd.setCursor(6,1);
  lcd.print(dt);
   
}

void drawAzzan()
{
    const char *sholat = jadwalAzzan[sholatNow]; 
    static uint8_t ct = 0;
    static uint32_t lsRn = 0;
    uint32_t Tmr = millis();
    const uint8_t limit = config.durasiadzan;

    if (Tmr - lsRn > 500 && ct <= limit)
    {
        lsRn = Tmr;
        if (!(ct & 1))  // Lebih cepat dibandingkan ct % 2 == 0
        {
          dwCtr(0, 0, "ADZAN");
          dwCtr(0, 1, sholat);
          Buzzer(1);
           
        }
        else
        {
          lcd.clear();
          Buzzer(0);
        }
        ct++;
    }
    
    if ((Tmr - lsRn) > 1500 && (ct > limit))
    {
        show = ANIM_CLOCK;
        ct = 0;
        Buzzer(0);
        lcd.clear();
    }
}


void dwCtr(int8_t x, int8_t y, String Msg){
   uint16_t   tw = Msg.length();
   uint16_t   c = int8_t((DWidth-x-tw)/2);
   lcd.setCursor(x+c,y);
   lcd.print(Msg);
}
