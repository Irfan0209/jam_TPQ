void runningInfoMode1(){
  static unsigned int x;
  if (reset_x !=0) {x=0;reset_x = 0;}
 // if(adzan==1 || flag1==0) { return; }
  
  int Speed = speedText1;
  
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  
  int fullScroll = Disp.textWidth(text1) + Disp.width() + 10;
  

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      fType(0);
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
        Disp.drawText(Disp.width() - x, 9, text1);
      }
      else {  
        x=0; 
        return;
      }
      DoSwap = true;
    }
}

void runningInfoMode2(){
  static unsigned int x;
  if (reset_x !=0) {x=0;reset_x = 0;}
  if(adzan) return;
  
  int Speed = speedText2;
  
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  
  int fullScroll = Disp.textWidth(text2) + Disp.width() + 10;
  

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      fType(2);
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
        Disp.drawText(Disp.width() - x, 0 , text2);
      }
      else {  
        x=0; 
        return;
      }
      DoSwap = true;
    }
}

void runAnimasiJam(){
  if(adzan) return;
  RtcDateTime now = Rtc.GetDateTime();
  static int    y=0;
  static bool    s; // 0=in, 1=out              
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  int dot    = now.Second();
  char buff_jam[20];
  
  if(dot%2){sprintf(buff_jam,"%02d:%02d",now.Hour(),now.Minute());}
  else{sprintf(buff_jam,"%02d %02d",now.Hour(),now.Minute());}
  
  if((Tmr-lsRn)>75) 
      { 
        if(s==0 and y<9){lsRn=Tmr;y++; }
        if(s==1 and y>0){lsRn=Tmr;y--; if(y == 1){ Disp.drawText(0,0, "          "); }}
      }
   if((Tmr-lsRn)>10000 and y ==9) {s=1;}

   if (y == 0 and s==1) { s=0; show = ANIM_DATE;}
  
  fType(0); 
  dwCtr(0,y-9, buff_jam); 
  DoSwap = true;
}

void runAnimasiDate(){
  static unsigned int x;
  if (reset_x !=0) { x=0;reset_x = 0;}
  if(adzan) return;
  const char *pasar[]     ={"WAGE", "KLIWON", "LEGI", "PAHING", "PON"}; 
  const char *Hari[]      ={"MINGGU","SENIN","SELASA","RABU","KAMIS","JUM'AT","SABTU"};
  //const char *sholatt[]   ={"SUBUH","TERBIT","DZUHUR","ASHAR","TRBNM","MAGRIB","ISYA"};
  const char *bulanMasehi[]={"JANUARI", "FEBRUARI", "MARET", "APRIL", "MEI", "JUNI", "JULI", "AGUSTUS", "SEPTEMBER", "OKTOBER", "NOVEMBER", "DESEMBER" };
  const char *namaBulanHijriah[] = {
    "MUHARRAM", "SHAFAR", "RABIUL AWAL",
    "RABIUL AKHIR", "JUMADIL AWAL", 
    "JUMADIL AKHIR", "RAJAB",
    "SYA'BAN", "RAMADHAN", "SYAWAL",
    "DZULQA'DAH", "DZULHIJAH"
  };
//"MUHARRAM","SHAFAR","RAB.AWAL","RAB.AKHIR","JUM.AWAL","JUM.AKHIR","RAJAB","SYA'BAN","RAMADHAN","SYAWAL","DZULQA'DAH","DZULHIJAH"

  RtcDateTime now = Rtc.GetDateTime();
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
   
  int Speed = speedDate;
  byte daynow   = now.DayOfWeek();    // load day Number
  
  char buff_date[50];

  sprintf(buff_date,"%s %s %02d-%02d-%04d %02d %s %02dH",Hari[daynow],pasar[jumlahhari()%5],now.Day(),now.Month(),now.Year(),Hijir.getHijriyahDate,namaBulanHijriah[Hijir.getHijriyahMonth - 1], Hijir.getHijriyahYear);
 
  int fullScroll = Disp.textWidth(buff_date) + Disp.width() ;
  //Serial.println("scroll:" + String(fullScroll));
    if((Tmr-lsRn)> Speed)
    { 
      lsRn = Tmr;
      if (x < fullScroll) { 
        ++x; 
        fType(0); 
        Disp.drawText(Disp.width() - x,0, buff_date);
      }
      else {  
        //flag1=1;
        Disp.clear();
        x=0; 
        reset_x=1;
        show = ANIM_JAM;
        return;
      }
      DoSwap = true;
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
