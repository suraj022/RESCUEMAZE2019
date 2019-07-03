
char VisualVictim() {
  int32_t temp = 0;
  char buff[50] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) { // got length?

    temp = Wire.read() | (Wire.read() << 8);
    delay(1); // Give some setup time...

    Wire.requestFrom(0x12, temp);
    if (Wire.available() == temp) { // got full message?

      temp = 0;
      while (Wire.available())
        buff[temp++] = Wire.read();

    } else {
      while (Wire.available())
        Wire.read(); // Toss garbage bytes.
    }
  } else {
    while (Wire.available())
      Wire.read(); // Toss garbage bytes.
  }
  delay(50);
  yield();
  return buff[0];
}

void camLoop() {
  if (victim == 0) {
    char tmp = VisualVictim();
    switch (tmp) {
    case 'H':
      victim = 3;
      break;
    case 'S':
      victim = 2;
      break;
    case 'U':
      victim = 1;
      break;
    default:
      break;
    }
  }
  // beep(10);
  delay(10);
  yield();
}
