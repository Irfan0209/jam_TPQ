void runningInfo(){
                   
  int Speed = speedText;
  static unsigned int x; 
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  //const int sizeChar = setText.length()+1;
  
 // msg = msg1;
  int fullScroll = Disp.textWidth(text) + Disp.width() + 10;

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      if (x < fullScroll) { 
        ++x; 
        //Serial.println(String()+"x:" + x); 
      }
      else {  
        x=0; 
        return;
      }

        //  if(      mode == 1 ){fType(0);  Disp.drawText(Disp.width() - x, 9, text); }
        //  else if( mode == 2 ){fType(2);  Disp.drawText(Disp.width() - x, 0, text); }
        (mode==1)? fType(1):fType(2);
        Disp.drawText(Disp.width() - x, (mode==1)? 9 : 0 , text);
    }
}

void runAnimasiJam(){

  RtcDateTime now = Rtc.GetDateTime();
  static int    y=-20;
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
  Disp.drawText(2,y-9, buff_jam); 

}

void runAnimasiDate(){

  RtcDateTime now = Rtc.GetDateTime();
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  static unsigned int x; 
  int Speed = speedDate;
  byte daynow   = now.DayOfWeek();    // load day Number
  char buff_date[50];

  sprintf(buff_date,"%s %s %02d-%02d-%04d %02d %s %02dH",Hari[daynow],pasar[jumlahhari()%5],now.Day(),now.Month(),now.Year(),tanggalHijriah.tanggal, namaBulanHijriah[tanggalHijriah.bulan - 1], tanggalHijriah.tahun);
 
  int fullScroll = Disp.textWidth(buff_date) + Disp.width() + 50;
  //Serial.println("scroll:" + String(fullScroll));
    if((Tmr-lsRn)> Speed)
      { lsRn = Tmr;
        if (x < fullScroll) { 
          ++x; 
          //Serial.println(String()+"x:" + x); 
        }
        else {  
          x=0; 
          show = ANIM_SHOLAT;
          return;
          
        }
        fType(0); 
        Disp.drawText(Disp.width() - x,0, buff_date);
      }
}




void animasiSholat(){
  
}
