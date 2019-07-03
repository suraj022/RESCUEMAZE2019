void setupIO() {
  pinMode(RMF, OUTPUT);
  pinMode(RMR, OUTPUT);
  pinMode(LMF, OUTPUT);
  pinMode(LMR, OUTPUT);
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder1PinA, INPUT_PULLUP);
  pinMode(encoder1PinB, INPUT_PULLUP);
  pinMode(buzzPin, OUTPUT);
  pinMode(LEFTBUMP, INPUT);
  pinMode(RIGHTBUMP, INPUT);
  pinMode(Chkpt, INPUT_PULLUP);
}

void beginSerialUSB() {
  int count = 0;
  SerialUSB.begin(9600);
  while (!SerialUSB && count < 4) {
    beep(50);
    delay(32);
    count++;
  }
}

void beep(int duration) {
  digitalWrite(buzzPin, HIGH);
  delay(duration);
  digitalWrite(buzzPin, LOW);
}

void waitForSignal() {
  while (getDistance(1) > 80) {
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(20, 20, 20));
      pixels.show();
    }
    delay(20);
  }
  // Move one tile sequence
  clearPixels();
  beep(50);
  while (getDistance(1) < 100) {
    delay(20);
  }
  delay(500);
}

void bumpLoop() {
  if (bumpcheck) {
    if (!digitalRead(LEFTBUMP)) {
      leftBumpFlag = true;
      delay(100);
    }
    if (!digitalRead(RIGHTBUMP)) {
      rightBumpFlag = true;
      delay(100);
    }
  } else {
    delay(100);
  }
  yield();
}

void colourLoop() {
  int val = analogRead(COLOURPIN);
  val = kalmanColour.updateEstimate(val);
  switch (val) {
  case (SILVER - 50)...(SILVER + 50):
    silverFlag = true;
    whiteFlag = false;
    blackFlag = false;
    break;
  case (NORMAL - 50)...(NORMAL + 50):
    silverFlag = false;
    whiteFlag = true;
    blackFlag = false;
    break;
  case (HAZARD - 50)...(HAZARD + 50):
    silverFlag = false;
    whiteFlag = false;
    blackFlag = true;
    break;
  }
  delay(10);
  yield();
}

void CalibrationProcess() {
#ifdef COLOURCHECK
  while (1) {
    int val = analogRead(COLOURPIN);
    val = kalmanColour.updateEstimate(val);
    clearScreen();
    displayTxt(0, 0, "Colour");
    String tmp = "";
    tmp += val;
    displayTxt(0, 21, tmp);
    SerialUSB.println(val);
    delay(100);
  }
#endif
#ifdef TEMPCHECK
  while (1) {
    int val = mlxL.readObjectTempC();
    int val1 = mlxL.readAmbientTempC();
    int val2 = mlxR.readObjectTempC();
    int val3 = mlxR.readAmbientTempC();
    clearScreen();
    displayTxt(0, 0, "tempL");
    String tmp = "";
    tmp += val;
    displayTxt(0, 21, tmp);
    SerialUSB.print(val - val1);
    SerialUSB.print(",");
    SerialUSB.println(val2 - val3);
    delay(100);
    yield();
  }
#endif
#ifdef ACCELCHECK
  while (1) {
    clearScreen();
    displayTxt(0, 0, "Accel");
    String tmp = "";
    tmp += accX;
    displayTxt(0, 21, tmp);
    SerialUSB.println(accX);
    delay(10);
    yield();
  }
#endif

#ifdef CAMCHECK
  while (1) {
    String tmp = "CAM: ";
    tmp += VisualVictim();
    clearScreen();
    displayTxt(0, 21, tmp);
    SerialUSB.println(accX);
    delay(100);
  }
#endif
}
