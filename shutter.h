
void shutter(boolean s) {

  if (s == true) {
    digitalWrite(CAMERA_SHOOT, HIGH);
    pcf0.digitalWrite(LED_B, HIGH);
    pcf0.digitalWrite(LED_A, LOW);
    return;
  }

  digitalWrite(CAMERA_SHOOT, LOW);
  pcf0.digitalWrite(LED_B, LOW);
  pcf0.digitalWrite(LED_A, HIGH);
  return;
}

void openShutter() {
  if (!keepLampOff) {
     digitalWrite(MAINS_SW, LOW);
  }
  delay(300);
  shutter(1);
  return;
}

void closeShutter() {
  shutter(0);
  delay(20);
  if (!keepLampOff) {
     digitalWrite(MAINS_SW, HIGH);
  }
  
  delay(10);
  return;
}
void flash() {

  digitalWrite(CAMERA_FLASH, HIGH);
  shutter(0);   // close shutter
  delay(3);
  digitalWrite(CAMERA_FLASH, LOW);
  delay(550);
  if (!keepLampOff) {
    digitalWrite(MAINS_SW, HIGH);
  }
  return;

}

void makeDropOfWater() {
  digitalWrite(SOLENOID, HIGH);
  delay(solenoidWait);
  digitalWrite(SOLENOID, LOW);
  return;
}

void setThreshold() {
  openShutter();  // turn lights off, open shutter
  delay(500);

  // set the threshold
  threshold = analogRead(PHOTO);
  return;
}

void timeoutReached() {
  irPWR = false;
  closeShutter();   // close shutter, turn lights on
  shutterOpen = false;
  return;
}
