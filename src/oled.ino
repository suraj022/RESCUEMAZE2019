void oledbegin() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(100);
}

void clearScreen() { display.clearDisplay(); }

void displayTxt(int x, int y, String txt) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(txt);
  display.display();
  delay(1);
}
