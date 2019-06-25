void beginMotion() {
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void IMUloop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.beginTransmission(MPU_addr);
  Wire.requestFrom(MPU_addr, 14); // request a total of 6 registers
  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  // Accelerometer values
  accX -= accCX;
  accY -= accCY;
  accZ = 0;
  if (accX < -32768)
    accX += 65536;
  if (accY > 32768)
    accY -= 65536;

  accX /= 100;
  accY /= 100;

  accX = map(accX, -163, 163, -90, 90);
  accY = map(accY, -163, 163, -90, 90);

  accX = kalmanX.updateEstimate(accX);
  accY = kalmanY.updateEstimate(accY);

  // Gyroscope values
  GyX = (GyX - CGyX) * 0.01;
  GyY = (GyY - CGyY) * 0.01;
  GyZ = (GyZ - CGyZ) * 0.01;
  if (abs(GyX) < 0.4)
    GyX = 0;
  if (abs(GyY) < 0.4)
    GyY = 0;
  if (abs(GyZ) < 0.4)
    GyZ = 0;

  pitch = pitch + GyY * 0.01 * 0.81;
  roll = roll + GyX * 0.01 * 0.81;
  yaw = yaw + GyZ * 0.01 * 0.81;

  delay(9);
  yield();
}

void CalibrateMPU6050(int samples) {
  CGyX = 0;
  CGyY = 0;
  CGyZ = 0;
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers
    CGyX += Wire.read() << 8 | Wire.read();
    CGyY += Wire.read() << 8 | Wire.read();
    CGyZ += Wire.read() << 8 | Wire.read();
    delay(9);
  }
  CGyX /= samples;
  CGyY /= samples;
  CGyZ /= samples;
}

void CalibrateMPU6050_Acc(int samples) {

  accCX = 0;
  accCY = 0;
  accCZ = 0;
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6); // request a total of 6 registers
    accCX += Wire.read() << 8 | Wire.read();
    accCY += Wire.read() << 8 | Wire.read();
    accCZ += Wire.read() << 8 | Wire.read();
  }
  accCX /= samples;
  accCY /= samples;
  accCZ /= samples;
}
