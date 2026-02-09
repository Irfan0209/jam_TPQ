
//////hijiriyah voidku/////////////////////////////////////////////////
void islam() {
  RtcDateTime now = Rtc.GetDateTime();
  static uint32_t sv=0;
  uint32_t timer = millis();
  if(now.Hour() == 00 && now.Minute() == 00 && now.Second() == 00){
    stateBuzzWar = 1;
  }

  if(timer - sv > 5000){
    //RtcDateTime now = Rtc.GetDateTime();
    JWS.Update(config.zonawaktu, config.latitude, config.longitude, config.altitude, now.Year(), now.Month(), now.Day()); // Jalankan fungsi ini untuk update jadwal sholat
    JWS.setIkhtiSu = dataIhty[0];
    JWS.setIkhtiDzu = dataIhty[1];
    JWS.setIkhtiAs = dataIhty[2];
    JWS.setIkhtiMa = dataIhty[3];
    JWS.setIkhtiIs = dataIhty[4];
    Hijir.Update(now.Year(), now.Month(), now.Day(), config.Correction);
    sv = timer;
  }
}

// digunakan untuk menghitung hari pasaran
int jumlahhari() { 
  RtcDateTime now = Rtc.GetDateTime();
  int d = now.Day();
  int m = now.Month();
  int y = now.Year();

  static const int hb[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  int ht = (y - 1970) * 365 - 1;
  int hs = hb[m - 1] + d;

  if (y % 4 == 0 && m > 2) hs++; // Tambahkan 1 hari jika tahun kabisat dan lewat Februari

  int kab = (y - 1969) / 4;  // Hitung langsung jumlah tahun kabisat

  return (ht + hs + kab);
}
