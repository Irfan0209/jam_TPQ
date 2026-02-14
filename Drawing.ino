//==================== animasi jam dan running text =================//
void dwMrq(const char* msg, int Speed, int dDT,int fontt) //running teks ada jam nya
  { 
    static uint16_t   x; 
    static uint16_t fullScroll = 0;
    if(adzan) return;
    if (reset_x !=0) { x=0; reset_x = 0; fullScroll = 0;}      

    uint32_t          Tmr = millis();
    static uint32_t lss=0;
    
    
     
    if (fullScroll == 0) { // Hitung hanya sekali
       fType(fontt);
       (show == ANIM_BIG)? fullScroll = Disp.textWidth(msg) + DWidth + 20 : fullScroll = Disp.textWidth(msg) + DWidth ; 
    }   
    
    
    
    if((Tmr-lss)> Speed)
      { lss=Tmr;
        if (x < fullScroll) {++x;}
        else {
          RtcDateTime now = Rtc.GetDateTime();
          if(show==ANIM_JAM){show=ANIM_DATE; Serial.println("TIME:" + String(now.Hour()) + "," + String(now.Minute()) + "," + String(now.Second()) + "," + String(now.DayOfWeek()));}
          else if(show==ANIM_BIG){show=ANIM_BIG; Serial.println("TIME:" + String(now.Hour()) + "," + String(now.Minute()) + "," + String(now.Second()) + "," + String(now.DayOfWeek()));}
         // else if(show==ANIM_BIG){show=ANIM_DATE;}
          x = 0; 
          fullScroll = 0;
          return;}
     if(dDT==1)
        {
        //fType(1);  //Marquee    jam yang tampil di bawah
        Disp.drawText(DWidth - x, 0, msg); //runing teks diatas
        //fType(1);
        if (x<=6)                     { drawGreg_TS(16-x);}
        else if (x>=(fullScroll-6))   { drawGreg_TS(16-(fullScroll-x));}
        else                          { drawGreg_TS(9);}//posisi jamnya yang bawah
   
        }
     else if(dDT==2) //jam yang diatas
        {    
        //fType(1);
        if (x<=6)                     { drawGreg_TS(x-6);}
        else if (x>=(fullScroll-6))   { drawGreg_TS((fullScroll-x)-6);}
        else                          { drawGreg_TS(0);}  //posisi jam nya yang diatas
        //fType(1); //Marquee  running teks dibawah
        Disp.drawText(DWidth - x, 9 , msg);//runinng teks dibawah
        
        }
      else if(dDT==3) //jam yang diatas
      {
        //fType(1);  //Marquee    jam yang tampil di bawah
        Disp.drawText(DWidth - x, 0, msg); //runing teks diatas
      }
        DoSwap = true;
      }          
     
  }

void drawGreg_TS(int y)   // Draw Time
  {
    RtcDateTime now = Rtc.GetDateTime();
    char  Buff[8];
    sprintf(Buff,(now.Second() % 2)?"%02d:%02d":"%02d %02d",now.Hour(),now.Minute());
    dwCtr(0,y,Buff);
    DoSwap = true;
  }

void runningInfoDanDate() {
  static int xInfo = 0;
  static int xDate = 0;
  static int fullScrollInfo = 0;
  static int fullScrollDate = 0;

  static unsigned long lastInfo = 0;
  static unsigned long lastDate = 0;

  static bool initDone = false;
  static bool infoDone = false;
  static bool dateDone = false;

  static char buff_date[60];

  if (reset_x) {
    xInfo = 0;
    xDate = 0;
    infoDone = false;
    dateDone = false;
    initDone = false;
    reset_x = 0;
  }

  if (adzan) return;

  if (!initDone) {
    fType(1);

    fullScrollInfo = Disp.textWidth(text1) + Disp.width();

    const char *pasar[] = {"WAGE","KLIWON","LEGI","PAHING","PON"};
    const char *Hari[]  = {"MINGGU","SENIN","SELASA","RABU","KAMIS","JUM'AT","SABTU"};
    const char *namaBulanHijriah[] = {
      "MUHARRAM","SHAFAR","RABIUL AWAL","RABIUL AKHIR",
      "JUMADIL AWAL","JUMADIL AKHIR","RAJAB",
      "SYA'BAN","RAMADHAN","SYAWAL",
      "DZULQA'DAH","DZULHIJAH"
    };

    RtcDateTime now = Rtc.GetDateTime();

    sprintf(
      buff_date,
      "%s %s %02d-%02d-%04d %02d %s %02dH",
      Hari[now.DayOfWeek()],
      pasar[jumlahhari() % 5],
      now.Day(), now.Month(), now.Year(),
      Hijir.getHijriyahDate,
      namaBulanHijriah[Hijir.getHijriyahMonth - 1],
      Hijir.getHijriyahYear
    );

    fullScrollDate = Disp.textWidth(buff_date) + Disp.width();
    initDone = true;
  }

  unsigned long nowMs = millis();
  bool needSwap = false;

  // ===== TEXT INFO =====
  if (!infoDone && (nowMs - lastInfo) >= speedText1) {
    lastInfo = nowMs;
    if (xInfo < fullScrollInfo) {
      xInfo++;
    } else {
      infoDone = true;
    }
    needSwap = true;
  }

  // ===== TEXT DATE =====
  if (!dateDone && (nowMs - lastDate) >= speedDate) {
    lastDate = nowMs;
    if (xDate < fullScrollDate) {
      xDate++;
    } else {
      dateDone = true;
    }
    needSwap = true;
  }

  if (!needSwap) return;

  //Disp.clear();

  if (!infoDone)
    Disp.drawText(Disp.width() - xInfo, 9, text1);

  if (!dateDone)
    Disp.drawText(Disp.width() - xDate, 0, buff_date);

  DoSwap = true;

  // ===== PINDAH ANIMASI JIKA KEDUANYA SELESAI =====
  if (infoDone && dateDone) {
    xInfo = 0;
    xDate = 0;
    infoDone = false;
    dateDone = false;
    show = ANIM_SHOLAT;   // animasi berikutnya
  }
}

void tampilkanVolume() {
  char buff[15];
  snprintf(buff, sizeof(buff), "%s=%02d", "VOLUME", volume);
  fType(1); 
  dwCtr(0,4,buff); //tulisan nama
  DoSwap  = true ;
}

//==================== tampilkan jadwal sholat ====================//

const char* jadwal[] PROGMEM = {"SUBUH", "TERBIT", "DHUHA", "DZUHUR", "ASHAR", "MAGRIB", "ISYA'"};
void animasiJadwalSholat() {
  if(adzan) return;
  
  RtcDateTime now = Rtc.GetDateTime();
  static int y = 0, y1 = 0;
  static uint8_t s = 0, s1 = 0;
  static bool run = false;

  if(reset_x != 0){ 
      y = 0;
      y1 = 0;
      s = 0;
      s1 = 0;
      reset_x = 0;
  }

  
  static uint32_t lsRn_y1 = 0;
  static uint32_t lsRn_y = 0;
  static uint32_t tHold = 0;

  uint32_t Tmr = millis();

  // Pilih waktu sholat sesuai list
  float stime;
  switch (list) {
    case 0: stime = JWS.floatSubuh; break;
    case 1: stime = JWS.floatTerbit; break;
    case 2: stime = JWS.floatDhuha; break;
    case 3: stime = JWS.floatDzuhur; break;
    case 4: stime = JWS.floatAshar; break;
    case 5: stime = JWS.floatMaghrib; break;
    case 6: stime = JWS.floatIsya; break;
    default: stime = 0; break;
  }

  // Transisi vertikal y1 (jam muncul/hilang)
  if ((Tmr - lsRn_y1) > 55) {
    lsRn_y1 = Tmr;

    if (s1 == 0 && y1 < 17) { y1++; } 
    else if (s1 == 1 && y1 > 0) { y1--; }
  }

  // Saat y1 selesai muncul, mulai animasi jadwal
  if (y1 == 17 && s1 == 0) {
    run = true; 
  }

  // Animasi gerakan teks (y)
  if (run && (Tmr - lsRn_y) > 55) {
    lsRn_y = Tmr;

    if (s == 0 && y < 9) {
      y++;
    } else if (s == 1 && y > 0) {
      y--;
    }
  }

  // Delay sebelum animasi keluar (reverse)
  if (y == 9 && s == 0 && tHold == 0) {
    tHold = millis();
  }
  if (tHold > 0 && (millis() - tHold > 800)) {
    s = 1;     // mulai keluar
    tHold = 0; // reset timer
  }

  // Setelah animasi selesai
  if (y == 0 && s == 1) {
    s = 0;
    list = (list + 1) % 7;
    if (list == 0) {
      run = false;
      s1 = 1; // trigger keluar vertikal
    }
  }

  // Tampilkan teks jadwal sholat
  uint8_t shour = (uint8_t)stime;
  uint8_t sminute = (uint8_t)((stime - shour) * 60);

  char buf[6];
  buf[0] = '0' + shour / 10;
  buf[1] = '0' + shour % 10;
  buf[2] = ':';
  buf[3] = '0' + sminute / 10;
  buf[4] = '0' + sminute % 10;
  buf[5] = '\0';

  fType(0);
  dwCtr(0, y - 9, jadwal[list]);
  fType(1);
  dwCtr(0, 18 - y, buf);
  DoSwap = true;
  if (y1 == 0 && s1 == 1) {
    s1 = 0;
    show = ANIM_JAM; // ganti mode jika perlu
  }
}
//=========================================================================//

/*======================= animasi memasuki waktu sholat ====================================*/
void drawAzzan()
{
    static const char *jadwal[] = {"SUBUH", "DZUHUR", "ASHAR", "MAGRIB","ISYA'"};
    const char *sholat = jadwal[sholatNow]; 
    static uint8_t ct = 0;
    static uint32_t lsRn = 0;
    uint32_t Tmr = millis();
    const uint8_t limit = config.durasiadzan;

    if (Tmr - lsRn > 500 && ct <= limit)
    {
        lsRn = Tmr;
        if (!(ct & 1))  // Lebih cepat dibandingkan ct % 2 == 0
        {
            fType(1);
            dwCtr(1, 0, "ADZAN");
            fType(0);
            dwCtr(1, 8, sholat);
            Buzzer(1);
            
        }
        else
        {
            Buzzer(0);
        }
        ct++;
        DoSwap = true;
    }
    
    if ((Tmr - lsRn) > 1500 && (ct > limit))
    {
        adzan = 0;
        (modeShowBig)? show = ANIM_BIG : show = ANIM_JAM;
        Disp.clear();
        ct = 0;
        Buzzer(0);
        sholatNow = -1;
    }
}

//=========================== setingan untuk tampilan text=================//
void fType(int x)
  {
    if(x==0) Disp.setFont(Font0);
     else if(x==1) Disp.setFont(Font1); 
    //else if(x==2) Disp.setFont(Font2);
    else if(x==3) Disp.setFont(Font3);
    //else if(x==4) Disp.setFont(Font4);
    else if(x==5) Disp.setFont(Font5);
  
  }

  void dwCtr(int x, int y, String Msg){
   uint16_t   tw = Disp.textWidth(Msg);
   uint16_t   c = int((DWidth-x-tw)/2);
   Disp.drawText(x+c,y,Msg);
}
