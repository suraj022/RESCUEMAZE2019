void beginMLX() {
  mlxL.begin();
  mlxR.begin();
}

void MLXloop() {
  if (mlxL.readObjectTempC() > 35)
    Lvictim = true;
  if (mlxR.readObjectTempC() > 35)
    Rvictim = true;
  delay(10);
}
