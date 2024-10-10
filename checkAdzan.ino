void check(){
  RtcDateTime now = Rtc.GetDateTime();
  int Hari = now.DayOfWeek();
  int jam = now.Hour();
  int menit = now.Minute();
  int detik = now.Second();
  int hours, minutes;
  static uint8_t counter=0;
  static uint32_t lsTmr;
  uint32_t tmr = millis();
 
  if(tmr - lsTmr > 100){
    lsTmr = tmr;
    
    get_float_time_parts(times[counter], hours, minutes);
    minutes = minutes + config.ihti;
    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }

    if(counter!=1 and counter!=4 ){
      if(jam == hours && menit == minutes && detik < 1){
      Disp.clear();
      sholatNow = counter;
      adzan = 1;
      reset_x = 1;
      show=ANIM_ADZAN;
      list    = 0;
      
      }
    }

    if(counter == 5){
      if(hours != trigJam || minutes != trigMenit){ trigJam  = hours; trigMenit=minutes;
      Serial.println("trigJam  :" + String(trigJam));
      Serial.println("trigMenit:" + String(trigMenit));} 
    }
  
    counter++;
    if(counter==7){counter=0;}
    
  }
  //Serial.println("sholatNow:" + String(sholatNow));
  // Serial.println("trigJam  :" + String(trigJam));
  // Serial.println("trigMenit:" + String(trigMenit));
}

void drawAzzan()
  {
    const char *jadwal[] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGRIB", "ISYA'"};
    String sholat = jadwal[sholatNow];
    // uint8_t           ct_limit =40;  //harus angka genap
    static uint8_t    ct;
    static uint32_t   lsRn;
    uint32_t          Tmr = millis();
    //RtcDateTime now = Rtc.GetDateTime();
    //int detik = now.Second();
   
    if(Tmr - lsRn > 500  and ct <= config.durasiadzan)
      {
        lsRn = Tmr;
        if((ct%2)==0)
          { 
            fType(3); dwCtr(0,0,"ADZAN");
            fType(3); dwCtr(0,8,sholat);
            Buzzer(1);
          }
        else 
          { Buzzer(0); Disp.clear();}
        //DoSwap = true; 
        ct++;
        Serial.println("ct:" + String(ct));
      }
    if ((Tmr-lsRn)>1500 and (ct > config.durasiadzan))
      {
        sholatNow=-1;
        flag1=1;
        adzan = 0;
        (mode==1)?show = ANIM_JAM : show = ANIM_ZONK;
        Disp.clear();
        ct = 0;
        Buzzer(0);
        Serial.println("adzan end");
      }
  }

void runAnimasiSholat(){
 
  if(adzan) return;
  RtcDateTime now = Rtc.GetDateTime();
  static int        y=0;
  static int        x=0;
  static byte       s=0; // 0=in, 1=out   
  static byte       s1=0;
  static byte lastList;

  if(list != lastList){s=0; s1=0; x=0; y=0;lastList = list; }

  static uint32_t   lsRn;
  uint32_t          Tmr = millis(); 
  
  const char *jadwal[] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGRIB", "ISYA'"};
  int hours, minutes;
  int tahun = now.Year();
  int bulan = now.Month();
  int tanggal = now.Day();
  char buff_jam[10];

  if((Tmr-lsRn)>55) 
  { 
    if(s1==0 and y<9){lsRn=Tmr; y++; }
    if(s==1 and x<33){lsRn=Tmr; x++; }
  }

  if((Tmr-lsRn)>4000 and y == 9) {s1=1; s=1;}

  if (x == 33 and s==1 and s1 == 1) { 
    s=0;
    s1=0;
    x=0;
    y=0;
     
    list++; 
    if(list==4){list=5;} 
    
    if(list==7){list=0; Disp.clear(); show=ANIM_JAM; flag1=1;}//else{list=list;}
  }

  String sholat = jadwal[list];

  get_float_time_parts(times[list], hours, minutes);

    minutes = minutes + config.ihti;

  if(minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
  }

  // if(list==5){ 
    
  // }

  sprintf(buff_jam,"%02d:%02d",hours,minutes);

  if(s1==0){
    fType(3);
    dwCtr(0,y-9, sholat);
    dwCtr(0,18-y, buff_jam);
  }
  else{
    Disp.drawLine((list<6)?x-1:x,-1,(list<6)?x-1:x,16,1);
    Disp.drawLine((list<6)?x-2:x-1,-1,(list<6)?x-2:x-1,16,0);
  }
}
