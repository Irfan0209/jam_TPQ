void runningInfo(){
   
  byte Speed = 75;
  static int x; 
  static unsigned long lsRn;
  unsigned long Tmr = millis();
  static char *msg[] = {"config.nama"};
  
  int fullScroll = Disp.textWidth(msg[0]) + Disp.width() ;
  //Serial.println(String()+"fullScroll:" + fullScroll); //debugging

  if((Tmr-lsRn)> Speed)
    { lsRn = Tmr;
      if (x < fullScroll) { 
        ++x; 
        Serial.println(String()+"x:" + x); 
      }
      else {  
        x=0; 
        return;
      }
         
      fType(0); 
      Disp.drawText(Disp.width() - x, 9, msg[0]);  
    }
}

void runAnimasi(){
  RtcDateTime now = Rtc.GetDateTime();
  static int    y=-20;
  static int    s; // 0=in, 1=out              
  static unsigned long   lsRn;
  unsigned long          Tmr = millis();
  static bool       state = true;
  static int x; 
  int daynow   = now.DayOfWeek();    // load day Number

  char buff_jam[20];
  char buff_date[50];

  sprintf(buff_jam,"%02d:%02d",now.Hour(),now.Minute());
  sprintf(buff_date,"%s %s %02d-%02d-%04d",Hari[daynow],pasar[jumlahhari()%5],now.Day(),now.Month(),now.Year());

  int fullScroll = Disp.textWidth(buff_date) + Disp.width() ;
  //Serial.println(String()+"fullScroll:" + fullScroll); //debugging
  //sprintf(out,"%s %s %02d-%02d-%04d  %02d-%s-%dH\0",daysOfTheWeek[daynow-1],pasar[jumlahhari()%5],now.day(),now.month(),now.year(),nowH.hD,mounthJawa[nowH.hM-1],nowH.hY);
  if((Tmr-lsRn)>75) 
      { 
        if(s==0 and y<=0){lsRn=Tmr;y++; }
        if(s==1 and y>=-20){lsRn=Tmr;y--; }
        
      }
  if((Tmr-lsRn)>3000 and y ==1) {s=1;}
  
  

  
  if(state){ fType(0); Disp.drawText(0,y, buff_jam); Serial.println(String()+"y:" + y); if (y == -20 and s==1) { s=0; state = !state;}}
  else{
    if((Tmr-lsRn)> 75)
      { lsRn = Tmr;
        if (x < fullScroll) { 
          ++x; 
          Serial.println(String()+"x:" + x); 
        }
        else {  
          x=0; 
          s=0; 
          state = !state;
          return;
        }
      }
    fType(0); Disp.drawText(Disp.width() - x,0, buff_date);
  }
}