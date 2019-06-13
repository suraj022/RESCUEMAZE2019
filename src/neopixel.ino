void beginNeopixel() {
  pixels.begin();
}

void clearPixels() {
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void setPixelColour(int x, int colour, int Brightness) {
  if (colour == RED) {
    pixels.setPixelColor(x, pixels.Color(Brightness, 0, 0));
    pixels.show();
  } else if (colour == GREEN) {
    pixels.setPixelColor(x, pixels.Color(0, Brightness, 0));
    pixels.show();
  } else if (colour == BLUE) {
    pixels.setPixelColor(x, pixels.Color(0, 0, Brightness));
    pixels.show();
  }
}

void indicateWalls() {
  clearPixels();
  delay(10);
  if (getDistance(LEFT) < WALLDISTANCE)
    setPixelColour(LEFTPIXEL, RED, 20);
  else
    setPixelColour(LEFTPIXEL, GREEN, 20);

  if (getDistance(FRONT) < WALLDISTANCE)
    setPixelColour(FRONTPIXEL, RED, 20);
  else
    setPixelColour(FRONTPIXEL, GREEN, 20);

  if (getDistance(RIGHT) < WALLDISTANCE)
    setPixelColour(RIGHTPIXEL, RED, 20);
  else
    setPixelColour(RIGHTPIXEL, GREEN, 20);
}

void indicatePath(int x) {
  delay(200);
  clearPixels();
  delay(10);
  switch (x) {
    case 0:
      setPixelColour(LEFTPIXEL - 1, GREEN, 5);
      setPixelColour(LEFTPIXEL, GREEN, 50);
      setPixelColour(LEFTPIXEL + 1, GREEN, 5);
      break;
    case 1:
      setPixelColour(FRONTPIXEL - 1, GREEN, 5);
      setPixelColour(FRONTPIXEL, GREEN, 50);
      setPixelColour(FRONTPIXEL + 1, GREEN, 5);
      break;
    case 2:
      setPixelColour(RIGHTPIXEL - 1, GREEN, 5);
      setPixelColour(RIGHTPIXEL, GREEN, 50);
      setPixelColour(RIGHTPIXEL + 1, GREEN, 5);
      break;
    default:
      break;
  }
}
