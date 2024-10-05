/*void checkAdzan(){
  RtcDateTime now = Rtc.GetDateTime();
  int Hari = now.DayOfWeek();
  int jam = now.Hour();
  int menit = now.Minute();
  int detik = now.Second();
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
    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      
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
  if (Hor == hours && Min == minutes && Hari != 5) {
    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      //buzzerku();
    }
    
  }
  //////////////////////////////end////////////////////////////////////////

  /////////////////////ALARM ADZAN DZUHUR HARI JUMAT////////////////////////////////////////////
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes && Hari == 5) {
    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      ////buzzerku();
    }
    
    }
  //////////////////////////////end////////////////////////////////////////

  /////////////////////ALARM ADZAN ASHAR ////////////////////////////////////////////
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes + config.ihti;
  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  if (Hor == hours && Min == minutes ) {
    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      //buzzerku();
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
    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      //buzzerku();
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

    if (Hor == hours && Min == minutes && now.Second() <= 7) {
      //
      //buzzerku();
    }
    
  }
}*/

void runAnimasiSholat(){

  RtcDateTime now = Rtc.GetDateTime();
  static int        y=0;
  static int        x=0;
  static bool       s,s1; // 0=in, 1=out   
  static uint32_t   lsRn;
  uint32_t          Tmr = millis(); 
  static uint8_t     list;
  const char *jadwal[] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGRIB", "ISYA'"};
  int hours, minutes;
  int tahun = now.Year();
  int bulan = now.Month();
  int tanggal = now.Day();
  char buff_jam[10];

  set_calc_method(Karachi);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  

  if((Tmr-lsRn)>55) 
  { 
    //if(s==0 and x>-1){lsRn=Tmr;x--; }
    if(s==1 and x<33){lsRn=Tmr;x++; }

    if(s1==0 and y<9){lsRn=Tmr; y++;}
    //if(s1==1 and y>0){lsRn=Tmr; y--;}
  }
  //if((Tmr-lsRn)>4000 and x ==-1) {s=1; }
  if((Tmr-lsRn)>4000 and y == 9) {s1=1; s=1;}

  if (x == 33 and s==1 and s1 == 1) {
     get_prayer_times(tahun, bulan, tanggal, config.latitude, config.longitude, config.zonawaktu, times); 
     s=0;
     s1=0;
     x=0;
     y=0;
     //if(list<=7){
      list++; 
      if(list==4){list=5;} 
      if(list==7){list=0; show=ANIM_JAM;}else{list=list;}
     //}
  }
  // if(s1==1 and y==0){
  //   s1=0;
  // }
  //if(list==7){list=0;}

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
  //Disp.drawRect(-1,-1,y,16,1,0);
  if(s1==0){
    fType(1);
    Disp.drawText(1,y-9, sholat);
    Disp.drawText(1,17-y, buff_jam);
    //Serial.println("y   :"+ String(y-9));
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