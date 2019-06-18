void setupTOF() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);

  Wire.begin();

  digitalWrite(10, HIGH);
  delay(50);
  sensorL.init(true);
  // sensorL.setSignalRateLimit(0.1);
  sensorL.setTimeout(400);
  sensorL.startContinuous();
  delay(50);
  sensorL.setAddress((uint8_t)21);
  sensorL.startContinuous();

  digitalWrite(9, HIGH);
  delay(50);
  sensorF.init(true);
  // sensorF.setSignalRateLimit(0.1);
  sensorF.setTimeout(400);
  sensorF.startContinuous();
  delay(50);
  sensorF.setAddress((uint8_t)22);
  sensorF.startContinuous();

  digitalWrite(8, HIGH);
  delay(50);
  sensorR.init(true);
  // sensorR.setSignalRateLimit(0.1);
  sensorR.setTimeout(400);
  sensorR.startContinuous();
  delay(50);
  sensorR.setAddress((uint8_t)23);
  sensorR.startContinuous();
}

int getDistance(int x) {
  int dist;
  switch (x) {
  case 0:
    dist = sensorL.readRangeContinuousMillimeters();
    break;
  case 1:
    dist = sensorF.readRangeContinuousMillimeters();
    break;
  case 2:
    dist = sensorR.readRangeContinuousMillimeters();
    break;
  default:
    dist = -1;
    break;
  }
  return dist;
}
