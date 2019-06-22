void moveMotor(int L, int R) {
  if (L >= 0) {
    analogWrite(LMF, L * 1.05);
    analogWrite(LMR, 0);
  } else {
    analogWrite(LMF, 0);
    analogWrite(LMR, -L);
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
  // offsetStraight(125);
  P = 0;
  I = 0;
  D = 0;
  lastError = 0;
  encoderposL = 0;
  encoderposR = 0;
  desiredposL = map(pos, 0, 300, 0, 350);
  desiredposR = map(pos, 0, 300, 0, 350);
  int encoderpos = (encoderposL + encoderposR) / 2;
  int desiredpos = (desiredposL + desiredposR) / 2;
  int encoderbackupL = 0;
  int encoderbackupR = 0;
  yaw = 0;
  delay(50);
  if ((desiredpos - encoderpos) > 5) {
    while ((desiredpos - encoderpos) > 10) {
      encoderpos = (encoderposL + encoderposR) / 2;
      int pwm = constrain((desiredpos - encoderpos), 0, MAXSPEED);
      int err = 0;
      int offset = 0;
      int wallL = getDistance(LEFT);
      int wallR = getDistance(RIGHT);
      if (wallL < WALLDISTANCE && wallR < WALLDISTANCE) {
        // err = (wallR - wallL);
        err = (wallDistance - wallL);
        P = 0;
      } else if (wallL > WALLDISTANCE && wallR < WALLDISTANCE) {
        err = (wallR - wallDistance);
        P = 0;
      } else if (wallR > WALLDISTANCE && wallL < WALLDISTANCE) {
        err = (wallDistance - wallL);
        P = 0;
      } else if (wallR > WALLDISTANCE && wallL > WALLDISTANCE) {
        err = yaw; // readGyroZ();
        P = 0;
        I = 0;
        D = 0;
        lastError = 0;
      }
      P = 0.35 * err;   // 1.818
      I += 0.001 * err; // 0.4
      D = 0 * (err - lastError);
      lastError = err;
      offset = P + I + D;
      moveMotor(pwm + offset, pwm - offset);
      if (leftBumpFlag && getDistance(FRONT) > 80) {
        bumpcheck = false;
        encoderbackupL = encoderposL;
        encoderbackupR = encoderposR;
        moveMotor(0, 0);
        delay(50);
        moveMotor(-40, -40);
        delay(500);
        moveMotor(40, -20);
        delay(300);
        moveMotor(0, 0);
        delay(50);
        moveMotor(60, 50);
        delay(600);
        encoderposL = encoderbackupL;
        encoderposR = encoderbackupR;
        leftBumpFlag = false;
        rightBumpFlag = false;
        bumpcheck = true;
      } else if (rightBumpFlag && getDistance(FRONT) > 80) {
        bumpcheck = false;
        encoderbackupL = encoderposL;
        encoderbackupR = encoderposR;
        moveMotor(0, 0);
        delay(50);
        moveMotor(-40, -40);
        delay(500);
        moveMotor(-20, 40);
        delay(300);
        moveMotor(0, 0);
        delay(50);
        moveMotor(50, 60);
        delay(600);
        encoderposL = encoderbackupL;
        encoderposR = encoderbackupR;
        leftBumpFlag = false;
        rightBumpFlag = false;
        bumpcheck = true;
      }
      delayMicroseconds(500);
      yield();
    }
  } else {
    while ((encoderpos - desiredpos) > 5) {
      encoderpos = (encoderposL + encoderposR) / 2;
      int pwm = constrain((encoderpos - desiredpos), 0, MAXSPEED / 1.5);
      int err = yaw;
      P = 2 * err;      // 1.818
      I += 0.001 * err; // 0.4
      D = 0 * (err - lastError);
      lastError = err;
      int offset = P + I + D;
      moveMotor(-(pwm - offset), -(pwm + offset));
      delayMicroseconds(500);
      yield();
    }
  }
  offsetStraight(80);
  beep();
  P = 0;
  I = 0;
  D = 0;
  lastError = 0;
  delay(50);
  moveMotor(0, 0); // stop motors
  lastError = 0;
  offsetStraight(125);
  switch (HEAD) {
  case 0:
    gridY++;
    break;
  case 1:
    gridX++;
    break;
  case 2:
    gridY--;
    break;
  case 3:
    gridX--;
    break;
  }
}

void turn90(int angle, int dir, bool align) {
  bumpcheck = false;
  bool flag = false;
  int ANGLE = angle;
  if (dir == 1)
    indicatePath(RIGHT);
  else
    indicatePath(LEFT);
  delay(200);

  // int ANGLE = angle - 6;
  if ((getDistance(0) < WALLDISTANCE && dir == 1) ||
      (getDistance(2) < WALLDISTANCE && dir == -1)) {
    flag = true;
  }
  if (getDistance(1) < WALLDISTANCE) {
    offsetStraight(53);
  } else {
    offsetStraight(78);
  }
  delay(10);
  moveMotor(0, 0);
  delay(10);
  int err = 0;
  Pr = 0, Ir = 0, Dr = 0;
  pitch = 0;
  roll = 0;
  yaw = 0;
  delay(10);
  do {
    err = (ANGLE - abs(yaw));
    if (err < 5)
      err *= 2;
    if (abs(pitch) > 2.7) {
      beep();
      delay(80);
    }
    Pr = err;
    // Ir += err * 0.001;
    int diff = 1.1 * Pr + Ir;
    moveMotor(dir * constrain(diff, 25, 100), -dir * constrain(diff, 25, 100));
    delay(1);
    yield();
  } while (abs(err) > 20);
  // delay(10);
  beep();
  moveMotor(0, 0);
  delay(50);

  if (flag && align) {
    moveMotor(-50, -50);
    delay(800);
    moveMotor(0, 0);
    yaw = 0;
  }

  // update heading on rotation
  HEAD += dir;
  if (HEAD > 3)
    HEAD = 0;
  if (HEAD < 0)
    HEAD = 3;

  leftBumpFlag = false;
  rightBumpFlag = false;
  bumpcheck = true;
}

void offsetStraight(int value) {
  yaw = 0;
  delay(100);
  int dist = (getDistance(1) % 300);
  if (dist > value) {
    while (dist - value > 10) {
      dist = (getDistance(1) % 300);
      float pwm = constrain((dist - value), 30, 70);
      pwm += (dist - value) * 0.001;
      // SerialUSB.println((dist - value));
      int err = 1.5 * yaw; // readGyroZ() / 14;
      moveMotor(pwm + err, pwm - err);
      delay(2);
      // yield();
    }
  } else {
    while (value - dist > 10) {
      dist = (getDistance(1) % 300);
      float pwm = constrain((value - dist), 30, 70);
      pwm += (value - dist) * 0.001;
      // SerialUSB.println(-(value - dist));
      int err = -1.5 * yaw; // readGyroZ() / 14;
      moveMotor(-(pwm + err), -(pwm - err));
      delay(2);
      // yield();
    }
  }
  moveMotor(0, 0);
}

// int rpmOffset() {
//  int L = 60000 / (((currentpulsetime1) / 1000.0) * 562.215);
//  L = constrain(L, 0, 255);
//  float KL = kalman.updateEstimate(L);
//  int R = 60000 / (((currentpulsetime2) / 1000.0) * 562.215);
//  R = constrain(R, 0, 255);
//  float KR = kalman.updateEstimate(R);
//  int offset = (KL - KR) * 2;
//  return offset;
//}
