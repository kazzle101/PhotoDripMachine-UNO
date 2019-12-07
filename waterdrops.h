
bool irStopCheck() {
    
   bool s = false;
   if (irrecv.decode(&IRresult)) {
        if (IRresult.value == 0xC061) { // Stop
          s = true;
        }
        irrecv.resume();
      }

  return s;
}


// #1, #2 and #3 on remote
// creates x drips for a single photo
void multipleDropsOfWater(byte noDrips) {

    setThreshold();

    strcat(oledText.line1, "Solenoid Trigger");

    if (noDrips == 1) {
      strcat(oledText.line2, "single drip");
    }
    else {
      sprintf(oledText.line2, "multi-drip, %i", noDrips);
    }
    sprintf(oledText.line3, "flashWait=%i", flashWait);
    sprintf(oledText.line4, "spot reading: %i", threshold);
    oledTextOutput();

    for (byte i = 0; i < noDrips; i++) {
      makeDropOfWater();
      if (i < noDrips - 1) {
        delay(betweenDrips);
      }
    }

    // wait for laser to detect drop, with a four second timeout
    nowMillis = millis();
    do {
      val = analogRead(PHOTO);
      if (val < (threshold - thresholdTol) || val > (threshold + thresholdTol)) {

        delay(flashWait);
        flash();  // flash flash. close shutter, turn lights on

        strcat(oledText.line1, "Solenoid Trigger");
        sprintf(oledText.line2, "multi-drip, %i", noDrips);
        sprintf(oledText.line3, "spot reading:  %i", threshold);
        sprintf(oledText.line4, "laser trigger: %i", val);
        oledTextOutput();
        delay(200);
        
        complete = true;
        break;
      }
    } while ((millis() - nowMillis) < timeout);

    if (complete == false) {
      timeoutReached();
    }
}

// #9 on remote
void solenoidOpen(int duration) {

  if (shutterOpen == true) {
    return;
  }


  strcat(oledText.line1, "Solenoid Trigger");
  strcat(oledText.line2, "solenoid open");
  sprintf(oledText.line3, "for %i seconds", duration);

  unsigned long target = millis() + (duration * 1000);
  digitalWrite(SOLENOID, HIGH);

  do {
    delay(5);
    if (irStopCheck()) {
      break;
    }
    
  } while (millis() < target);

  digitalWrite(SOLENOID, LOW);
  return;
}

// #0 on remote
void dripSequence() {

  unsigned int flashOri = flashWait;

  keepLampOff = true;
  digitalWrite(MAINS_SW, LOW);
  
  flashWait = 90;                   // start
  unsigned int flashWaitEnd = 550;   // finish
  unsigned int flashStep = 1;

  unsigned int i = 1;
  unsigned int noPhotos = (flashWaitEnd - flashWait) / flashStep;

  do {
    strcat(oledText.line1, "Solenoid Trigger");
    strcat(oledText.line2, "drip sequence");
    sprintf(oledText.line3, "%i of %i photos", i, noPhotos);
    oledTextOutput();

    irPWR = true;
    multipleDropsOfWater(2);
    
    nowMillis = millis();
    do {
     delay(5);
     if (irStopCheck()) {
       flashWait = flashWaitEnd + 5;
       break;
       }
    } while ((millis() - nowMillis) < 4000);

    flashWait = flashWait + flashStep;
    i++;
        
  } while (flashWait < flashWaitEnd);

  keepLampOff = false;
  digitalWrite(MAINS_SW, HIGH);
  irPWR = false;
  flashWait = flashOri;

  strcat(oledText.line1, "Solenoid Trigger");
  strcat(oledText.line2, "drip sequence");
  sprintf(oledText.line3, "end at %i photos", i);
  oledTextOutput();

  return;
}
