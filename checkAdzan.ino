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
      if(jam == hours && menit == minutes && detik < 4){
      Disp.clear();
      sholatNow = counter;
      adzan = 1;
      reset_x = 1;
      show=ANIM_ADZAN;
      list    = 0;
      
      }
    }
    //if(counter==5)
    counter++;
    if(counter==7){counter=0;}
    
  }
  //Serial.println("sholatNow:" + String(sholatNow));
  //Serial.println("counter  :" + String(counter));
  //Serial.println("adzan    :" + String(adzan));
}

void Adzan(){
  const char *jadwal[] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGRIB", "ISYA'"};
  String sholat = jadwal[sholatNow];
   RtcDateTime now = Rtc.GetDateTime();
   int detik = now.Second();
  
  if(detik <= 15){
    fType(0);
    Disp.drawText(0,0,sholat);
  }else{
      sholatNow=-1;
      adzan = 0;
      show = ANIM_JAM;
      Disp.clear();
  }

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
   
    if(Tmr - lsRn > 500)
      {
        lsRn = Tmr;
        if((ct%2) == 0)
          { //Disp.drawRect(1,2,62,13);
            
            fType(0); Disp.drawText(0,0,"ADZAN");
            fType(1); Disp.drawText(0,8,sholat);
            // fType(1);
            // dwCtr(0,8,sholatN(SholatNow));
            Buzzer(1);
          }
        else 
          { Buzzer(0); Disp.clear();}
        //DoSwap = true; 
        ct++;
        Serial.println("ct:" + String(ct));
      }
    if ( ct >= config.durasiadzan)
      {
        sholatNow=-1;
        flag1=1;
        adzan = 0;
        (mode==1)?show = ANIM_JAM : show = ANIM_ZONK;
        Disp.clear();
        ct = 0;
        Buzzer(0);
      }
  }

void checkAdzan(){
  RtcDateTime now = Rtc.GetDateTime();
  int Hari = now.DayOfWeek();
  int Hor = now.Hour();
  int Min = now.Minute();
  //int detik = now.Second();
  int hours, minutes;
  char buff_jadwal[10];
  char buff_jam[10];

  // static int        y=-20;
  // static bool       s; // 0=in, 1=out   
  // static uint32_t   lsRn;
  // uint32_t          Tmr = millis();   

  /////////////////////ALARM ADZAN SUBUH ////////////////////////////////////////////
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    adzan = 1;
    Disp.clear();
    show=ANIM_ZONK;
    if(Hor == hours && Min == minutes && now.Second() <= 16) {
      fType(0); 
    Disp.drawText(0,0, "shubuh");
    if(now.Second() == 15){  adzan=0; Disp.clear();  show=ANIM_JAM;}
    }
    
  }
  /////////////////////////////end/////////////////////////////////////////
  /////////////////////ALARM ADZAN DZUHUR NON JUMAT////////////////////////////////////////////
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    adzan = 1;
    while(Hor == hours && Min == minutes && now.Second() <= 7) {
    fType(0); 
    Disp.drawText(0,0, "duhur");
    if(now.Second() == 7){ break; adzan=0;}
    }
    
  }
  //////////////////////////////end////////////////////////////////////////

  /////////////////////ALARM ADZAN DZUHUR HARI JUMAT////////////////////////////////////////////
  // get_float_time_parts(times[2], hours, minutes);
  // minutes = minutes + config.ihti;
  // if (minutes >= 60) {
  //   minutes = minutes - 60;
  //   hours ++;
  // }
  // if (Hor == hours && Min == minutes && Hari == 5) {
  //   if (Hor == hours && Min == minutes && now.Second() <= 7) {
  //     Type(0); 
  //   Disp.drawText(0,0, "shubuh");
  //   }
    
  //   }
  //////////////////////////////end////////////////////////////////////////

  /////////////////////ALARM ADZAN ASHAR ////////////////////////////////////////////
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    adzan = 1;
    while(Hor == hours && Min == minutes && now.Second() <= 7) {
      fType(0); 
    Disp.drawText(0,0, "ashar");
      if(now.Second() == 7){ break; adzan=0;}
    }
   
  }
  //////////////////////////////end////////////////////////////////////////
  /////////////////////ALARM ADZAN MAGHRIB ////////////////////////////////////////////
  get_float_time_parts(times[5], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    adzan = 1;
    while(Hor == hours && Min == minutes && now.Second() <= 7) {
      fType(0); 
      Disp.drawText(0,0, "magrib");
      if(now.Second() == 7){break; adzan=0;}
    }
    
  }
  //////////////////////////////end////////////////////////////////////////
  /////////////////////ALARM ADZAN ISYA ////////////////////////////////////////////
  get_float_time_parts(times[6], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    adzan = 1;
    while(Hor == hours && Min == minutes && now.Second() <= 7) {
      fType(0); 
      Disp.drawText(0,0, "isya");
      if(now.Second() == 7){break; adzan=0;}
    }
    
  }
}

void runAnimasiSholat(){
 
  if(adzan) {Serial.println("run back"); return;}
  RtcDateTime now = Rtc.GetDateTime();
  static int        y=0;
  static int        x=0;
  static byte       s=0; // 0=in, 1=out   
  static byte       s1=0;
  static byte lastList;
  if(list != lastList){s=0; s1=0; x=0; y=0;lastList = list; }
  //if (reset_x !=0) {list=0; s=0; s1=0; Serial.println("run reset sholat: list s  s1"+ String(list)+String(s)+String(s1)); reset_x = 0;}
  static uint32_t   lsRn;
  uint32_t          Tmr = millis(); 
  
  const char *jadwal[] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGRIB", "ISYA'"};
  int hours, minutes;
  int tahun = now.Year();
  int bulan = now.Month();
  int tanggal = now.Day();
  char buff_jam[10];

  Serial.println(String()+"list          :"+ list);
  Serial.println(String()+"lastList      :"+ lastList);
  // set_calc_method(Karachi);
  // set_asr_method(Shafii);
  // set_high_lats_adjust_method(AngleBased);
  // set_fajr_angle(20);
  // set_isha_angle(18);

  

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

  //Serial.println("list:"+ String(list));
  //Serial.println("y   :"+ String(y));

  
  Serial.println("list   :"+ String(list));
  String sholat = jadwal[list];

  get_float_time_parts(times[list], hours, minutes);

    minutes = minutes + config.ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }
  sprintf(buff_jam,"%02d:%02d",hours,minutes);

  if(s1==0){
    fType(1);
    Disp.drawText(1,y-9, sholat);
    Disp.drawText(1,17-y, buff_jam);
  }
  else{
    Disp.drawLine((list<6)?x-1:x,-1,(list<6)?x-1:x,16,1);
    Disp.drawLine((list<6)?x-2:x-1,-1,(list<6)?x-2:x-1,16,0);
  }
}


void dwCtr(int x, int y, String Msg){
   int   tw = Disp.textWidth(Msg);
    int   th = 16;
    int   c = int((DWidth-x-tw)/2);
    //Disp.drawFilledRect(x+c-1,y,x+tw+c,y+th,0);
    Disp.drawText(x+c,y,Msg);
}