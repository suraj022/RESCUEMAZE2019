void initServo() {
  dispencer.attach(A2); // attaches the servo on pin 9 to the servo object
  dispencer.write(100);
}

void servoMovement() {
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
}

void dispence() {
  if (Ltemp) {
    moveMotor(-20, -20);
    delay(200);
    moveMotor(0, 0);
    delay(200);
    moveMotor(100, -100);
    delay(300);
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
    servoMovement();
    delay(500);
    moveMotor(-100, 100);
    delay(300);
    moveMotor(0, 0);
    delay(100);
    Ltemp = false;
    Rtemp = false;
  }
  if (Rtemp) {
    moveMotor(-20, -20);
    delay(200);
    moveMotor(0, 0);
    delay(200);
    moveMotor(-100, 100);
    delay(300);
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
    servoMovement();
    delay(500);
    moveMotor(100, -100);
    delay(300);
    moveMotor(0, 0);
    delay(100);
    Rtemp = false;
    Ltemp = false;
  }
}
