void oledbegin(){
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.display();
  delay(100);
}

void clearScreen(){
  display.clearDisplay();
}

void displayPos(int x,int y,String txt,int posX,int posY){
  String msg = txt;
  msg+=posX;
  msg+=",";
  msg+=posY;
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(msg);
  display.display();
  delay(1);
}
void disp(){
clearScreen();
displayPos(0,0,"cur:",p_x,p_y);
displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
delay(200);
}
