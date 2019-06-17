void beginMotion() {
  Wire.begin();  // join i2c bus (address optional for master)
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void gyroLoop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.beginTransmission(MPU_addr);
  Wire.requestFrom(MPU_addr, 6); // request a total of 6 registers
  while (Wire.available() < 6);
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  GyX = (GyX - CGyX) * 0.01;
  GyY = (GyY - CGyY) * 0.01;
  GyZ = (GyZ - CGyZ) * 0.01;
  if (abs(GyX) < 0.4) GyX = 0;
  if (abs(GyY) < 0.4) GyY = 0;
  if (abs(GyZ) < 0.4) GyZ = 0;

  pitch = pitch + GyY * 0.01 * 0.81;
  roll = roll + GyX * 0.01 * 0.81;
  yaw = yaw + GyZ * 0.01 * 0.81;

  delay(9);
  //yield();
}

void accelLoop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.beginTransmission(MPU_addr);
  Wire.requestFrom(MPU_addr, 6); // request a total of 6 registers
  while (Wire.available() < 6);
  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  accX -= accCX;
  accY -= accCY;
  accZ -= accCZ;
  accX = kalmanX.updateEstimate(accX);
  accY = kalmanY.updateEstimate(accY);
  accZ = kalmanZ.updateEstimate(accZ);
  SerialUSB.print(accX);
  SerialUSB.print(",");
  SerialUSB.print(accY);
  SerialUSB.print(",");
  SerialUSB.println(accZ);
  delay(9);
}

void CalibrateMPU6050(int samples) {
  CGyX = 0; CGyY = 0; CGyZ = 0;
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers
    CGyX += Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    CGyY += Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    CGyZ += Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    delay(9);
  }
  CGyX /= samples;
  CGyY /= samples;
  CGyZ /= samples;

  // accCX=0; accCY=0; accCZ=0;
  // for (int i=0;i<samples;i++){
  //   Wire.beginTransmission(MPU_addr);
  //   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  //   Wire.endTransmission(false);
  //   Wire.requestFrom(MPU_addr, 6); // request a total of 6 registers
  //   while (Wire.available() < 6);
  //   accCX += Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  //   accCY += Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  //   accCZ += Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  //   //delay(9);
  // }
  // accCX /= samples;
  // accCY /= samples;
  // accCZ /= samples;
}
