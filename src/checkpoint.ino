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
  }
}

void restartCheckpoint() {
  for (int i = maze[mazeNum].COUNT; i > chkCount; i--) {
    tile tmp;
    maze[mazeNum].cell[i] = tmp;
  }
  maze[mazeNum].COUNT = chkCount;
  HEAD = startHead;
  waitForSignal();
}
