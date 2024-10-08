void runningInfoMode1(){
  static unsigned int x;
  if (reset_x !=0) {x=0;reset_x = 0;}
  if(adzan==1 || flag1==0) { return; }
  
  int Speed = speedText;
  
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  
  int fullScroll = Disp.textWidth(text) + Disp.width() + 10;
  

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      fType(0);
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
        Disp.drawText(Disp.width() - x, 9, text);
      }
      else {  
        x=0; 
        return;
      }
    }
}

void runningInfoMode2(){
  static unsigned int x;
  if (reset_x !=0) {x=0;reset_x = 0;}
  if(adzan) return;
  
  int Speed = speedText;
  
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  
  int fullScroll = Disp.textWidth(text) + Disp.width() + 10;
  

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      fType(2);
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
        Disp.drawText(Disp.width() - x, 0 , text);
      }
      else {  
        x=0; 
        return;
      }
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

}

void runAnimasiDate(){
  static unsigned int x;
  if (reset_x !=0) { x=0;reset_x = 0;}
  if(adzan) return;
  const char *pasar[]     ={"WAGE", "KLIWON", "LEGI", "PAHING", "PON"}; 
  const char *Hari[]      ={"MINGGU","SENIN","SELASA","RABU","KAMIS","JUM'AT","SABTU"};
  const char *sholatt[]   ={"SUBUH","TERBIT","DZUHUR","ASHAR","TRBNM","MAGRIB","ISYA"};
  const char *namaBulanHijriah[] = {
    "Muharram", "Safar", "Rabiul awal",
    "Rabiul Akhir", "Jumadil Awal", 
    "Jumadil Akhir", "Rajab",
    "Sya'ban", "Ramadhan", "Syawal",
    "Dzulkaidah", "Dzulhijjah"
  };

  RtcDateTime now = Rtc.GetDateTime();
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  // bool flag=1;
   
  int Speed = speedDate;
  byte daynow   = now.DayOfWeek();    // load day Number
  
  char buff_date[50];

  sprintf(buff_date,"%s %s %02d-%02d-%04d %02d %s %02dH",Hari[daynow],pasar[jumlahhari()%5],now.Day(),now.Month(),now.Year(),tanggalHijriah.tanggal, namaBulanHijriah[tanggalHijriah.bulan - 1], tanggalHijriah.tahun);
 
  int fullScroll = Disp.textWidth(buff_date) + Disp.width() ;
  //Serial.println("scroll:" + String(fullScroll));
    if((Tmr-lsRn)> Speed)
    { 
      lsRn = Tmr;
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
        fType(0); 
        Disp.drawText(Disp.width() - x,0, buff_date);
      }
      else {  
        flag1=0;
        Disp.clear();
        x=0; 
        reset_x=1;
        show = ANIM_SHOLAT;
        return;
      }
    }
}

void dwCtr(int x, int y, String Msg){
   int   tw = Disp.textWidth(Msg);
    int   th = 16;
    int   c = int((DWidth-x-tw)/2);
    //Disp.drawFilledRect(x+c-1,y,x+tw+c,y+th,0);
    Disp.drawText(x+c,y,Msg);
}
