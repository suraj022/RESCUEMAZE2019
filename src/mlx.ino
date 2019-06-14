void beginMLX() {
  mlxL.begin();
  mlxR.begin();
}

void MLXloop() {
  if (mlxL.readObjectTempC() > victimTemperature) Ltemp = true;
  if (mlxR.readObjectTempC() > victimTemperature) Rtemp = true;
  delay(10);
}
