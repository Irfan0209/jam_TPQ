void islam() {
  RtcDateTime now = Rtc.GetDateTime();
  
  //cekJadwalPanel(now.Hour(), now.Minute());

//  static int8_t lastHalfPlay = -1;
//
//  // Bunyi jam tepat
//  if (now.Minute() == 0 && now.Second() == 0 && now.Hour() != lastHalfPlay && config.stateBuzzerClock) {
//    lastHalfPlay = now.Hour();
//    stateBuzzWar = 1;
//  }

  // --- PEMICU TENGAH MALAM (Ganti Hari) ---
  // Pastikan jadwal dihitung ulang setiap jam 00:00:01
  static int8_t lastDayCalc = -1;
  if (now.Day() != lastDayCalc) {
    lastDayCalc = now.Day();
    butuhHitungJadwal = true; 
  }

  // Restart otomatis (opsional, dari kodemu)
  if (now.Minute() == 0 && now.Second() == 0 && now.Hour() == 0){
    Serial.println("restart");
    // ESP.restart(); // (Aktifkan jika memang alat diset untuk restart harian)
  }

  // --- EKSEKUSI RUMUS HANYA JIKA FLAG AKTIF ---
  if(butuhHitungJadwal){
    JWS.Update(config.zonawaktu, config.latitude, config.longitude, config.altitude, now.Year(), now.Month(), now.Day());
    JWS.setIkhtiSu = dataIhty[0];
    JWS.setIkhtiDzu = dataIhty[1];
    JWS.setIkhtiAs = dataIhty[2];
    JWS.setIkhtiMa = dataIhty[3];
    JWS.setIkhtiIs = dataIhty[4];
    Hijir.Update(now.Year(), now.Month(), now.Day(), config.Correction);
    
    butuhHitungJadwal = false; // Matikan flag setelah selesai menghitung agar CPU kembali santai
    Serial.println(F("[INFO] Jadwal Sholat dan Hijriah Berhasil Dikalkulasi Ulang!"));
  }
}
