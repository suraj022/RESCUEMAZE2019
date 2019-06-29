void beginMLX() {
  mlxL.begin();
  mlxR.begin();
}

void MLXloop() {
  if (mlxL.readObjectTempC() > victimTemperature)
    Lvictim = true;
  if (mlxR.readObjectTempC() > victimTemperature)
    Rvictim = true;
  delay(10);
}
