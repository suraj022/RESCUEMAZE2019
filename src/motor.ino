void moveMotor(int L, int R) {
  if (L >= 0 ) {
    analogWrite(LMF, L * 1.07);
    analogWrite(LMR, 0);
  } else {
    analogWrite(LMF, 0);
    analogWrite(LMR, -L * 1.07);
  }
  if (R >= 0) {
    analogWrite(RMF, R);
    analogWrite(RMR, 0);
  } else {
    analogWrite(RMF, 0);
    analogWrite(RMR, -R);
  }
}

void moveStraight(int pos) {
  P = 0; I = 0; D = 0;
  lastError = 0;
  encoderposL = 0;
  encoderposR = 0;
  desiredposL = map(pos, 0, 300, 0, 400);
  desiredposR = map(pos, 0, 300, 0, 400);
  int encoderpos = (encoderposL + encoderposR) / 2;
  int desiredpos = (desiredposL + desiredposR) / 2;
  int state = 0, prevState = 0;
  yaw = 0;
  if ((desiredpos - encoderpos) > 0) {
    while ((desiredpos - encoderpos) > 10) {
      encoderpos = (encoderposL + encoderposR) / 2;
      int pwm = constrain((desiredpos - encoderpos), 0, MAXSPEED);
      int err = 0;
      int offset = 0;
      int wallL = getDistance(LEFT);
      int wallR = getDistance(RIGHT);
      if (wallL < WALLDISTANCE && wallR < WALLDISTANCE) {
        //err = (wallR - wallL);
        err = (wallDistance - wallL);
        P = 0;
        state = 0;
      } else if (wallL > WALLDISTANCE && wallR < WALLDISTANCE) {
        err = (wallR - wallDistance);
        P = 0;
        state = 1;
      } else if (wallR > WALLDISTANCE && wallL < WALLDISTANCE) {
        err = (wallDistance - wallL);
        P = 0;
        state = 2;
      } else if (wallR > WALLDISTANCE && wallL > WALLDISTANCE) {
        //err  = yaw;//readGyroZ();
        P = 0; I = 0; D = 0;
        lastError = 0;
        state = 3;
      }
      P = 0.3 * err;  //1.818
      I += 0 * err; //0.4
      D = 0 * (err - lastError);
      lastError = err;
      offset = P + I + D ;
      moveMotor(pwm + offset, pwm - offset);
      //delay(1);
      yield();
    }
  }
  //else {
  //    while ((encoderpos - desiredpos) > 0) {
  //      encoderpos = (encoderposL + encoderposR) / 2;
  //      int pwm = (encoderpos - desiredpos) / 1.5;
  //      if (pwm < 50)pwm *= 1.5;
  //      int yaw  = readGyroZ() / 7;
  //      analogWrite(LMF, 0);
  //      analogWrite(RMF, 0);
  //      analogWrite(LMR, constrain(pwm - yaw, 0, MAXSPEED) * MotorOffset);
  //      analogWrite(RMR, constrain(pwm + yaw, 0, MAXSPEED));
  //      delay(2);
  //      yield();
  //    }
  //  }
  beep();
  P = 0; I = 0; D = 0;
  lastError = 0;
  delay(50);
  moveMotor(0, 0); //stop motors
  P = 0; I = 0; D = 0;
  lastError = 0;
}

void turn90(int angle, int dir) {
  bool flag = false;
  int ANGLE = 0;
  if (getDistance(0) < WALLDISTANCE || getDistance(2) < WALLDISTANCE) {
    flag = true;
  }

  if (dir == -1)
    ANGLE = angle - 5;
  else
    ANGLE = angle;
  unsigned long lastTime;
  if (getDistance(1) < WALLDISTANCE) {
    offsetStraight(40);
  }
  delay(10);
  moveMotor(0, 0);
  delay(10);
  //CalibrateMPU6050(20);
  int err = 0;
  Pr = 0, Ir = 0, Dr = 0;
  pitch = 0; roll = 0; yaw = 0;
  delay(10);
  do {
    unsigned long now = millis();
    double timeChange = (double)(now - lastTime);
    err = (ANGLE - abs(yaw));
    if (err < 5) err *= 2;
      if (abs(pitch) > 2.7) {
        beep();
        delay(80);
      }
    Pr = err;
    Ir += err * timeChange;
    Dr = (err - lastErrorTurn) / timeChange;
    lastErrorTurn = err;
    lastTime = now;
    int diff = 1.5 * Pr;
    moveMotor(dir * constrain(diff, 30, 100), -dir * constrain(diff, 30, 100));
    delay(1);
    yield();
  } while (abs(err) > 16);
  //delay(10);
  moveMotor(0, 0);
  delay(50);
  if (flag) {
    moveMotor(-50, -50);
    delay(1000);
    moveMotor(0, 0);
  }
}


void offsetStraight(int value) {
  int offset = (getDistance(1) % 300);
  if (offset > value) { //needs update
    while (offset > value) {
      offset = (getDistance(1) % 300);
      int pwm = (offset - value);
      if (pwm < 20) pwm *= 4;
      else if (pwm < 30)pwm *= 2;
      else pwm *= 1.1;
      int YAW  = 0;//readGyroZ() / 14;
      moveMotor(pwm + YAW, pwm - YAW);
      delay(1);
      //yield();
    }
  } else {
    while (offset < value) {  //needs update
      offset = (getDistance(1) % 300);
      int pwm = (value - offset);
      if (pwm < 20) pwm *= 4;
      else if (pwm < 30)pwm *= 2;
      else pwm *= 1.1;
      int YAW  = 0;//readGyroZ() / 14;
      moveMotor(-(pwm + YAW), -(pwm - YAW));
      delay(1);
      //yield();
    }
  }
}


//int rpmOffset() {
//  int L = 60000 / (((currentpulsetime1) / 1000.0) * 562.215);
//  L = constrain(L, 0, 255);
//  float KL = kalman.updateEstimate(L);
//  int R = 60000 / (((currentpulsetime2) / 1000.0) * 562.215);
//  R = constrain(R, 0, 255);
//  float KR = kalman.updateEstimate(R);
//  int offset = (KL - KR) * 2;
//  return offset;
//}
