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
}

void beginSerialUSB() {
  int count = 0;
  SerialUSB.begin(9600);
  while (!SerialUSB && count < 4) {
    beep();
    delay(32);
    count++;
  }
}

void beep() {
  digitalWrite(buzzPin, HIGH);
  delay(50);
  digitalWrite(buzzPin, LOW);
}
