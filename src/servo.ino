void initServo() {
  dispencer.attach(A2); // attaches the servo on pin 9 to the servo object
  dispencer.write(100);
  delay(300);
  dispencer.detach();
}

void servoMovement() {
  dispencer.attach(A2);
  for (pos = 100; pos >= 0; pos -= 1) { // goes from 100 degrees to 0 degrees
    dispencer.write(pos); // tell servo to go to position in variable 'pos'
    delay(5);             // waits 5ms for the servo to reach the position
  }
  delay(100);
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 100 degrees
    // in steps of 1 degree
    dispencer.write(pos); // tell servo to go to position in variable 'pos'
    delay(5);             // waits 5ms for the servo to reach the position
  }
  dispencer.detach();
  delay(10);
}

void dispence(int8 times, int8_t side) {
  moveMotor(-50, -50);
  delay(60);
  moveMotor(0, 0);
  delay(200);
  if (times > 0) {
    if (side == -1)
      turnBot(90, 1, false);
    else
      turnBot(90, -1, false);
  }
  moveMotor(0, 0);
  for (int halt = 0; halt < 4; halt++) {
    digitalWrite(buzzPin, HIGH);
    for (int i = 0; i < 8; i++) {
      setPixelColour(i, RED, 20);
    }
    delay(500);
    digitalWrite(buzzPin, LOW);
    clearPixels();
    delay(500);
  }
  for (int i = 0; i < times; i++) {
    servoMovement();
    delay(500);
  }
  if (times > 0) {
    if (side == -1)
      turnBot(90, -1, false);
    else
      turnBot(90, 1, false);
  } else {
    delay(800);
  }
  moveMotor(0, 0);
  delay(100);
}
