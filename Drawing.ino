//==================== animasi jam dan running text =================//
void dwMrq(const char* msg, int Speed, int dDT,int fontt) //running teks ada jam nya
  { 
    static uint16_t   x; 
    if(adzan) return;
    if (reset_x !=0) { x=0; reset_x = 0;}      

    uint32_t          Tmr = millis();
    static uint32_t lss=0;
    static uint16_t fullScroll = 0;
    
     
    if (fullScroll == 0) { // Hitung hanya sekali
       fType(fontt);
       fullScroll = Disp.textWidth(msg) + DWidth ; 
    }   
    
    
    
    if((Tmr-lss)> Speed)
      { lss=Tmr;
        if (x < fullScroll) {++x;}
        else {
          if(show==ANIM_JAM){show=ANIM_DATE;}
          else if(show==ANIM_DATE){show=ANIM_JAM;}
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
    sprintf(Buff,"%02d:%02d",now.Hour(),now.Minute());
    dwCtr(0,y,Buff);
    DoSwap = true;
  }

uint16_t speedTextInfo = 40;   // kecepatan text1
uint16_t speedTextDate = 45;   // kecepatan tanggal

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
  if (!infoDone && (nowMs - lastInfo) >= speedTextInfo) {
    lastInfo = nowMs;
    if (xInfo < fullScrollInfo) {
      xInfo++;
    } else {
      infoDone = true;
    }
    needSwap = true;
  }

  // ===== TEXT DATE =====
  if (!dateDone && (nowMs - lastDate) >= speedTextDate) {
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

//==================== tampilkan jadwal sholat ====================//
void animasiJadwalSholat(){
  if(adzan) return;
  RtcDateTime now = Rtc.GetDateTime();
  static int        y=0;
  static int        x=0;
  static uint8_t    s=0; // 0=in, 1=out   
  static uint8_t    s1=0;
  
  float sholatT[]={JWS.floatSubuh,JWS.floatTerbit,JWS.floatDhuha,JWS.floatDzuhur,JWS.floatAshar,JWS.floatMaghrib,JWS.floatIsya};
  if(list != lastList){s=0; s1=0; x=0; y=0;lastList = list; }

  static uint32_t   lsRn;
  uint32_t          Tmr = millis(); 
  
  const char *jadwal[] = {"SUBUH","TERBIT","DHUHA", "DZUHUR", "ASHAR", "MAGRIB","ISYA'"};
  char buff_jam[10];

  if((Tmr-lsRn)>55) 
  { 
    if(s1==0 and y<9){lsRn=Tmr; y++; }
    if(s==1 and x<33){lsRn=Tmr; x++; }
  }

  if((Tmr-lsRn)>800 and y == 9) {s1=1; s=1;} //4 detik

  if (x == 33 and s==1 and s1 == 1) { 
    s=0;
    s1=0;
    x=0;
    y=0;
    list++; 
    //Serial.println(config.latitude,6);
    if(list==7){list=0; Disp.clear(); show=ANIM_JAM; }
  }

  float stime = sholatT[list];
  uint8_t shour = floor(stime);
  uint8_t sminute = floor((stime - (float)shour) * 60);
  uint8_t ssecond = floor((stime - (float)shour - (float)sminute / 60) * 3600);

  sprintf(buff_jam, "%02d:%02d", shour, sminute);

  if(s1==0){
    fType(3);
    dwCtr(0,y-9, jadwal[list]);
    fType(0);
    dwCtr(0,18-y, buff_jam);
  }
  else{
    Disp.drawLine((list<6)?x-1:x,-1,(list<6)?x-1:x,16,1);
    Disp.drawLine((list<6)?x-2:x-1,-1,(list<6)?x-2:x-1,16,0);
  }
  DoSwap = true;
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
            fType(0);
            dwCtr(1, 0, "ADZAN");
            fType(3);
            dwCtr(1, 9, sholat);
            Buzzer(1);
            DoSwap = true;
        }
        else
        {
            Buzzer(0);
            Disp.clear();
        }
        ct++;
    }
    
    if ((Tmr - lsRn) > 1500 && (ct > limit))
    {
        adzan = 0;
        show = ANIM_JAM;
        Disp.clear();
        ct = 0;
        Buzzer(0);
    }
}

//=========================== setingan untuk tampilan text=================//
void fType(int x)
  {
   // if(x==2) Disp.setFont(Font0);
     if(x==1) Disp.setFont(Font1); 
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
