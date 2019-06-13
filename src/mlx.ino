void beginMLX() {
  mlxL.begin();
  mlxR.begin();
}

void MLXloop() {
  if (mlxL.readObjectTempC() > victimTemperature) Ltemp = true;
  if (mlxR.readObjectTempC() > victimTemperature) Rtemp = true;
//  SerialUSB.print(pitch);
//  SerialUSB.print(",");
//  SerialUSB.print(roll);
//  SerialUSB.print(",");
//  SerialUSB.println(yaw);
  delay(10);
//  yield();
}
