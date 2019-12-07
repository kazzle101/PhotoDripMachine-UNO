///// set settings
//void addSetting(char n, unsigned int current) {
//
//  char settingNew[2];
//  char settingStr[5];
//
//  settingNew[0] = char(n);  // Char to append to string
//  settingNew[1] = '\0';
//
//  //itoa(n, settingNew, 10);    // int to char array
//  //itoa(setting, settingStr, 10);
//  strcat(setting, settingNew);
//
//  strcat(oledText.line1,"Settings");
//  strcat(oledText.line2,"new setting");
//  sprintf(oledText.line3,"%s", settingNew);
//  oledTextOutput();
//
//  int slen = strlen(setting);
//  if (slen > 3 || slen < 1) {
//  //  Serial.println(" invalid setting number (len), using current");
//    setting[0] = '\0';
//    settingNo = current;
//    return;
//  }
//
//  settingNo = atoi(setting); // char array to int
//  if (settingNo > 1024) {
////    Serial.println(" invalid setting number (val), using current");
//    setting[0] = '\0';
//    settingNo = current;
//  }
//
//  return;
//}
//
//void setSettings(unsigned int settingsVal[4]) {
//
//  if (settingsVal[0] != flashWait) {
//    flashWait = settingsVal[0];
//  }
//  if (settingsVal[1] != solenoidWait) {
//    solenoidWait = settingsVal[1];
//  }
//  if (settingsVal[2] != betweenDrips) {
//    betweenDrips = settingsVal[2];
//  }
//  if (settingsVal[3] != thresholdTol) {
//    thresholdTol = settingsVal[3];
//  }
//  return;
//}
//
//void printSetting(byte p, char *settingStr, unsigned int settingVal) {
//
//  strcat(oledText.line1,"Settings");
//  strcat(oledText.line2,"Up/Dwn & OK to pick");
//  sprintf(oledText.line3,"%i: %s=%i", p, settingStr, settingVal);
//  oledTextOutput();
//}

void changeFlashWait(int n) {

  unsigned int old = flashWait;
  flashWait = flashWait + n;
  flashWait = constrain(flashWait, 10, 600);

  strcat(oledText.line1,"Settings");
  strcat(oledText.line2,"Change: flashWait");
  sprintf(oledText.line3,"%i >> %i", old, flashWait);
  oledTextOutput();

}
void changeBetweenDrips(int n) {

  unsigned int old = betweenDrips;
  betweenDrips = betweenDrips + n;
  betweenDrips = constrain(betweenDrips, 100, 300);

  strcat(oledText.line1,"Settings");
  strcat(oledText.line2,"Change: betweenDrips");
  sprintf(oledText.line3,"%i >> %i", old, betweenDrips);
  oledTextOutput();
}

void changeLaserTolerance(int n) {

  unsigned int old = thresholdTol;
  thresholdTol = thresholdTol + n;
  thresholdTol = constrain(thresholdTol, 2, 40);

  strcat(oledText.line1,"Settings");
  strcat(oledText.line2,"Change: thresholdTol");
  sprintf(oledText.line3,"%i >> %i", old, thresholdTol);
  oledTextOutput();

}

// menu on remote
//void changeSettings() {
//
//  char *settings[] = { "flashWait", "solenoidWait", "betweenDrips", "thresholdTol"};
//  unsigned int settingsVal[4] = {flashWait, solenoidWait, betweenDrips, thresholdTol};
//  unsigned int currentVal[4] = {flashWait, solenoidWait, betweenDrips, thresholdTol};
//
//  byte p = 0;
//  int irNo;
//  boolean inMenu = true;
//  boolean change = false;
//  settingNo = settingsVal[0];
//
//  delay(600);
//  do {
//    if (irrecv.decode(&IRresult)) {
//      if (IRresult.value != 0xC05E) {
//        inMenu = false;
//        }
//      irrecv.resume();
//    }      
//  } while (inMenu == true);
//  inMenu=true;
//  
//
//  do {
//    printSetting(p, settings[p], settingsVal[p]);
//
//    if (irrecv.decode(&IRresult)) {
//      
//     Serial.println(IRresult.value, HEX);
//      switch (IRresult.value) {
//        
//        case (0xC067): // OK
//          inMenu = false;
//          break;
//
//        case (0xC05E):    // menu
//          inMenu = false;
//          change = true;
//          break;
//
//         case (0xC03E):  // arrow up
//           p++;
//           p = (p > 3 ? p = 0 : p);
//           break;
//
//         case (0xC0DE):  // arrow down
//           p--;
//           p = (p > 3 ? p = 3 : p);    // a byte does not go negative  
//           break;
//      }
//      irrecv.resume(); // Receive the next value
//    }
//    
//  } while (inMenu == true);
//
//
//  Serial.println("ok");
//
//  if (change == true) {
//    oledShowMainOptions();
//    return;  
//  }
//
//  
//
//  do {
//    if (irrecv.decode(&IRresult)) {
//      switch (IRresult.value) {
//        case (0xC091):   // Play
//          if (strlen(setting) > 0) {
//          //  Serial.println("");
//            setting[0] = '\0';
//          }
//          change = true;
//          break;
//
//        case (0xC061):   // Stop
//       //   Serial.println("Settings Updated");
//          change = false;
//          inMenu = false;
//          break;
//
//        case (0xC0A0):   // recall
//        //  Serial.println("cancel");
//          setting[0] = '\0';
//          printSetting(p, settings[p], settingsVal[p]);
//          break;
//
//        case (0xC004):    // 000
//          addSetting('0', settingsVal[p]);
//          break;
//        case (0xC084):    // 001
//          addSetting('1', settingsVal[p]);
//          break;
//        case (0xC044):    // 002
//          addSetting('2', settingsVal[p]);
//          break;
//        case (0xC0C4):    // 003
//          addSetting('3', settingsVal[p]);
//          break;
//        case (0xC024):    // 004
//          addSetting('4', settingsVal[p]);
//          break;
//        case (0xC0A4):    // 005
//          addSetting('5', settingsVal[p]);
//          break;
//        case (0xC064):    // 006
//          addSetting('6', settingsVal[p]);
//          break;
//        case (0xC0E4):    // 007
//          addSetting('7', settingsVal[p]);
//          break;
//        case (0xC014):    // 008
//          addSetting('8', settingsVal[p]);
//          break;
//        case (0xC094):    // 009
//          addSetting('9', settingsVal[p]);
//          break;
//      }
//
//      irrecv.resume(); // Receive the next value
//    }
//
//    if (change == true) {
//      // if the setting is a new value - display it
//      if (settingNo != settingsVal[p]) {
//        settingsVal[p] = settingNo;
//        printSetting(p, settings[p], settingsVal[p]);
//      }
//      change = false;
//
//      p++;
//      if (p >= 3) {
//        p = 0;
//      }
//      settingNo = settingsVal[p];
//      printSetting(p, settings[p], settingsVal[p]);
//    }
//  } while (inMenu == true);
//
//  setSettings(settingsVal);
//  for (int i = 0; i <= 3; i++) {
//    if (currentVal[i] != settingsVal[i]) {
//      printSetting(i, settings[i], settingsVal[i]);
//    }
//    else {
//      printSetting(i, settings[i], settingsVal[i]);
//    }
//  }
// // Serial.println("----------------");
// // setColours(colours[2]);   // green
//  return;
//}
