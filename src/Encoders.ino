void doEncoderL() { //ISR routine for left encoder
  if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
    encoderposL++;
  } else {
    encoderposL--;
  }
  currentpulsetime1 = micros() - previoustime1;
  previoustime1 = micros();
}

void doEncoderR() { //ISR routine for right encoder
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoderposR++;
  } else {
    encoderposR--;
  }
  currentpulsetime2 = micros() - previoustime2;
  previoustime2 = micros();
}
