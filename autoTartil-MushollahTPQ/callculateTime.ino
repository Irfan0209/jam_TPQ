void islam() {
  
  RtcDateTime now = Rtc.GetDateTime();
  uint8_t tgl = now.Day();
  uint8_t bln = now.Month();
  uint16_t thn = now.Year();
  
  if(thn < 2010){
    Rtc.SetDateTime(RtcDateTime(2010, 10, 1,now.Hour(), now.Minute(), now.Second()));
  }
  static uint32_t sv=0;
  uint32_t timer = millis();
  

  if(timer - sv > 5000){
    //RtcDateTime now = Rtc.GetDateTime();
    JWS.Update(config.zonawaktu, config.latitude, config.longitude, config.altitude, thn, bln, tgl); // Jalankan fungsi ini untuk update jadwal sholat
    JWS.setIkhtiSu = dataIhty[0];
    JWS.setIkhtiDzu = dataIhty[1];
    JWS.setIkhtiAs = dataIhty[2];
    JWS.setIkhtiMa = dataIhty[3];
    JWS.setIkhtiIs = dataIhty[4];
    Hijir.Update(thn, bln, tgl, config.Correction);
    sv = timer;
  }
}
