
void setVolume(byte newVolume) {

  if (newVolume > MAX_VOLUME) {
    volume = MAX_VOLUME;
    Buzzer(1);
    delay(50);
    Buzzer(0);
  }
  else if (newVolume < MIN_VOLUME) {
    volume = MIN_VOLUME;
    Buzzer(1);
    delay(50);
    Buzzer(0);
  }
  else {
    volume = newVolume;
  }

  // Aktifkan tampilan sementara
  showVolumeTemp = true;
  volumeDisplayMillis = millis();
}
