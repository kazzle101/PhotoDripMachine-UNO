// clear the oled char arrays - not the display
void clearOLEDtext() {

  oledText.line1[0] = {0};
  oledText.line2[0] = {0};
  oledText.line3[0] = {0};
  oledText.line4[0] = {0};
}

void oledBlankDisplay() {
  display.clearDisplay();
  display.display();  
}

// display the text to screen
void oledTextOutput() {

  display.clearDisplay();
//
  display.setRotation(2);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  display.println(oledText.line1);
  display.println(oledText.line2);
  display.println(oledText.line3);
  display.println(oledText.line4);

  display.display();

  clearOLEDtext();
}


void oledShowMainOptions() {
  strcat(oledText.line1,"Water Drop Control");
  //strcat(oledText.line2,"Press Menu button to");
  //strcat(oledText.line3,"change settings");

  
  oledTextOutput();
}
