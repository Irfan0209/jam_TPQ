void runningInfo(){
                   
  byte Speed = 60;
  static int x; 
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  const int sizeChar = setText.length()+1;
  
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
         
      fType(0); 
      Disp.drawText(Disp.width() - x, 9, text);  
    }
}

void runAnimasiJam(){

  RtcDateTime now = Rtc.GetDateTime();
  static int    y=-20;
  static int    s; // 0=in, 1=out              
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  int dot    = now.Second();
  char buff_jam[20];
  
  if(dot%2){sprintf(buff_jam,"%02d:%02d",now.Hour(),now.Minute());}
  else{sprintf(buff_jam,"%02d %02d",now.Hour(),now.Minute());}
  

  
  //Serial.println(String()+"fullScroll:" + fullScroll); //debugging
  //sprintf(out,"%s %s %02d-%02d-%04d  %02d-%s-%dH\0",daysOfTheWeek[daynow-1],pasar[jumlahhari()%5],now.day(),now.month(),now.year(),nowH.hD,mounthJawa[nowH.hM-1],nowH.hY);
  if((Tmr-lsRn)>75) 
      { 
        if(s==0 and y<9){lsRn=Tmr;y++; }
        if(s==1 and y>0){lsRn=Tmr;y--; if(y == 1){ Disp.drawText(0,0, "          "); }}
        
      }
   if((Tmr-lsRn)>10000 and y ==9) {s=1;}

   if (y == 0 and s==1) { s=0; tampilan = 2;}
  
  fType(0); Disp.drawText(2,y-9, buff_jam); 
  //Serial.println("y:" + String(y-9)); 
  
}

void runAnimasiDate(){

  RtcDateTime now = Rtc.GetDateTime();
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  static int x; 
  const int Speed = 70;
  int daynow   = now.DayOfWeek();    // load day Number
  char buff_date[50];
  
  //Serial.println(String()+"daynow:" + daynow); 

  sprintf(buff_date,"%s %s %02d-%02d-%04d %02d %s %02dH",Hari[daynow],pasar[jumlahhari()%5],now.Day(),now.Month(),now.Year(),tanggalHijriah.tanggal, namaBulanHijriah[tanggalHijriah.bulan - 1], tanggalHijriah.tahun);
  //sprintf(tanggalanhij, "%02d %s %02dH", tanggalHijriah.tanggal, namaBulanHijriah[tanggalHijriah.bulan - 1], tanggalHijriah.tahun);
  Serial.println("tanggalHijriah.bulan - 1:" + String(tanggalHijriah.bulan - 1));
  Serial.println("tanggalHijriah.tahun    :" + String(tanggalHijriah.tahun));
  int fullScroll = Disp.textWidth(buff_date) + Disp.width() ;

    if((Tmr-lsRn)> Speed)
      { lsRn = Tmr;
        if (x < fullScroll) { 
          ++x; 
          //Serial.println(String()+"x:" + x); 
        }
        else {  
          x=0; 
          tampilan = 1;
          return;
          
        }
      }
    fType(0); Disp.drawText(Disp.width() - x,0, buff_date);
  

}