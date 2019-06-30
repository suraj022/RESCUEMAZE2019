void storeCheckpoint() {
  if (silverFlag) {
    clearPixels();
    clearScreen();
    displayTxt(0, 0, "checkpoint stored");
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 20));
      pixels.show();
    }
    beep(200);
    clearScreen();
    clearPixels();
    chkCount = maze[mazeNum].COUNT;
    saveHead = HEAD;
    saveX = maze[mazeNum].gridX;
    saveY = maze[mazeNum].gridY;
  }
}

void restartCheckpoint() {
  waitForSignal();
  for (int i = maze[mazeNum].COUNT + 1; i > chkCount; i--) {
    tile tmp;
    maze[mazeNum].cell[i] = tmp;
  }
  maze[mazeNum].COUNT = chkCount;
  maze[mazeNum].gridX = saveX;
  maze[mazeNum].gridY = saveY;
  yaw = 0;
  HEAD = startHead;
  orient(saveHead);
}
