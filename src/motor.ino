void moveMotor(int L, int R) {
  if (L >= 0) {
    analogWrite(LMF, L * 1.045);
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
  // offsetStraight(110);
  P = 0;
  I = 0;
  D = 0;
  lastError = 0;
  encoderposL = 0;
  encoderposR = 0;
  desiredposL = map(pos, 0, 300, 0, 370);
  desiredposR = map(pos, 0, 300, 0, 370);
  int encoderpos = (encoderposL + encoderposR) / 2;
  int desiredpos = (desiredposL + desiredposR) / 2;
  int encoderbackupL = 0;
  int encoderbackupR = 0;
  yaw = 0;
  delay(50);
  if ((desiredpos - encoderpos) > 5) {
    while ((desiredpos - encoderpos) > 10 && accX < 20) {
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
        err = yaw;
        P = 0;
        I = 0;
        D = 0;
        lastError = 0;
      }
      P = 0.3 * err; // 1.818
      I += 0 * err;  // 0.4
      D = 0 * (err - lastError);
      lastError = err;
      offset = P + I + D;
      moveMotor(pwm + offset, pwm - offset);
      if (leftBumpFlag && getDistance(FRONT) > 80) {
        bumpcheck = false;
        encoderbackupL = encoderposL;
        encoderbackupR = encoderposR;
        moveMotor(-30, -30);
        delay(50);
        moveMotor(-40, -40);
        delay(500);
        moveMotor(40, -40);
        delay(150);
        moveMotor(0, 0);
        delay(50);
        moveMotor(50, 60);
        delay(400);
        moveMotor(0, 0);
        delay(200);
        encoderposL = encoderbackupL;
        encoderposR = encoderbackupR;
        leftBumpFlag = false;
        rightBumpFlag = false;
        bumpcheck = true;
      } else if (rightBumpFlag && getDistance(FRONT) > 80) {
        bumpcheck = false;
        encoderbackupL = encoderposL;
        encoderbackupR = encoderposR;
        moveMotor(-30, -30);
        delay(50);
        moveMotor(-40, -40);
        delay(500);
        moveMotor(-40, 40);
        delay(150);
        moveMotor(0, 0);
        delay(50);
        moveMotor(60, 50);
        delay(400);
        moveMotor(0, 0);
        delay(200);
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
    while ((encoderpos - desiredpos) > 5 && accX < 20) {
      encoderpos = (encoderposL + encoderposR) / 2;
      int pwm = constrain((encoderpos - desiredpos), 0, MAXSPEED / 1.5);
      int err = yaw;
      P = 2 * err;  // 1.818
      I += 0 * err; // 0.4
      D = 0 * (err - lastError);
      lastError = err;
      int offset = P + I + D;
      moveMotor(-(pwm - offset), -(pwm + offset));
      delayMicroseconds(500);
      yield();
    }
  }
  beep(50);
  P = 0;
  I = 0;
  D = 0;
  lastError = 0;
  if (accX < RAMPANGLE / 3) {
    moveMotor(-50, -50);
    delay(50);
    moveMotor(0, 0); // stop motors
    delay(300);
    offsetStraight(125, false);
  }
  if (pos > 0) {
    switch (HEAD) {
    case 0:
      maze[mazeNum].gridY++;
      break;
    case 1:
      maze[mazeNum].gridX++;
      break;
    case 2:
      maze[mazeNum].gridY--;
      break;
    case 3:
      maze[mazeNum].gridX--;
      break;
    }
  } else {
    switch (HEAD) {
    case 0:
      maze[mazeNum].gridY--;
      break;
    case 1:
      maze[mazeNum].gridX--;
      break;
    case 2:
      maze[mazeNum].gridY++;
      break;
    case 3:
      maze[mazeNum].gridX++;
      break;
    }
  }
}

void turnBot(int angle, int dir, bool align) {
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
    offsetStraight(70, true);
  } else {
    offsetStraight(110, true);
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
    if (abs(pitch) > 4.5) {
      moveMotor(dir * 150, -dir * 150);
      beep(30);
      moveMotor(0, 0);
      delay(10);
    }
    Pr = err;
    // Ir += err * 0.001;
    int diff = Pr + Ir;
    moveMotor(dir * constrain(diff, 25, 100), -dir * constrain(diff, 25, 100));
    delay(1);
    yield();
  } while (abs(err) > 22 && digitalRead(Chkpt));
  // delay(10);
  beep(50);
  moveMotor(0, 0);
  delay(50);

  if (flag && align) {
    moveMotor(-50, -50);
    delay(800);
    moveMotor(0, 0);
    yaw = 0;
  } else {
    delay(500);
  }

  // update heading on rotation
  HEAD += dir;
  if (HEAD > 3)
    HEAD = 0;
  if (HEAD < 0)
    HEAD = 3;

  leftBumpFlag = false;
  rightBumpFlag = false;
  delay(50);
  bumpcheck = true;
}

void offsetStraight(int value, bool ifturning) {
  if (getDistance(FRONT) > 8100 && !ifturning) {
    // moveMotor(0, 80);
    // delay(100);
    moveStraight(120);
    moveMotor(0, 0);
    return;
  }
  // yaw = 0;
  delay(20);
  int expGain = 0;
  int dist = (getDistance(1) % 300);
  if (dist > value) {
    while (dist - value > 15 && digitalRead(Chkpt)) {
      if (getDistance(FRONT) > 8100) {
        moveMotor(50, 50);
        delay(200);
        moveMotor(0, 0);
        return;
      }
      dist = (getDistance(1) % 300);
      float pwm = constrain((dist - value), 35, 70);
      expGain += (dist - value) * 0.05;
      pwm += expGain;
      int err = 0.9 * yaw;
      moveMotor(pwm + err, pwm - err);
      delay(2);
    }
  } else {
    while (value - dist > 5 && digitalRead(Chkpt)) {
      if (getDistance(FRONT) > 8100) {
        moveMotor(50, 50);
        delay(200);
        moveMotor(0, 0);
        return;
      }
      dist = (getDistance(1) % 300);
      float pwm = constrain((value - dist), 35, 70);
      expGain += (value - dist) * 0.05;
      pwm += expGain;
      int err = -0.9 * yaw;
      moveMotor(-(pwm + err), -(pwm - err));
      delay(2);
    }
  }
  moveMotor(-50, -50);
  delay(50);
  moveMotor(0, 0); // stop motors
}

bool ramp() {
  if (accX > RAMPANGLE / 2.5) {
    moveMotor(100, 100);
    beep(500);
    bumpcheck = false;
    int I = 0;
    clearPixels();
    for (int i = 2; i < 7; i++) {
      pixels.setPixelColor(i, pixels.Color(10, 20, 5));
      pixels.show();
    }
    while (accX > RAMPANGLE / 1.5) {
      int pwm = 95;
      int offset = 0;
      // offset = -accY * 1.2;
      offset = 0.15 * (wallDistance - getDistance(LEFT));
      // I += 0.003 * (wallDistance - getDistance(LEFT));
      // if (abs(wallDistance - getDistance(LEFT)) < 10)
      // offset = 0;
      // I = 0;
      // offset -= I;
      moveMotor(pwm + offset, pwm - offset);
      // delay(1);
      yield();
      // moveMotor(offset / 2, -offset / 2);
    }
    clearPixels();
    // moveMotor(-80, -80);
    // delay(80);
    moveMotor(0, 0);
    // beep(300);
    // CalibrateMPU6050_Acc(50);
    // beep(300);
    // if (getDistance(FRONT) < WALLDISTANCE) {
    //   offsetStraight(55);
    // } else {
    //   offsetStraight(100);
    // }
    bumpcheck = true;
    yaw = 0;
    return true;
  } else if (accX < -RAMPANGLE / 2.5) {
    // moveMotor(100, 100);
    // delay(300);
    bumpcheck = false;
    for (int i = 2; i < 7; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 20));
      pixels.show();
    }
    while (accX < -RAMPANGLE / 1.2) {
      int pwm = 90;
      int offset = 0;
      // offset = -accY * 1.2;
      offset = 0.9 * (wallDistance - getDistance(LEFT));
      moveMotor(pwm + offset, pwm - offset);
      delay(5);
      yield();
    }
    clearPixels();
    moveMotor(80, 80);
    delay(500);
    moveMotor(0, 0);
    // beep(300);
    // CalibrateMPU6050_Acc(50);
    // beep(300);
    // delay(1000);
    // if (getDistance(FRONT) < WALLDISTANCE) {
    //   offsetStraight(55);
    // } else {
    //   offsetStraight(100);
    //}
    bumpcheck = true;
    yaw = 0;
    return true;
  }
  return false;
}

int rpmOffset() {
  int L = 60000 / (((currentpulsetime1) / 1000.0) * 562.215);
  L = constrain(L, 0, 255);
  int R = 60000 / (((currentpulsetime2) / 1000.0) * 562.215);
  R = constrain(R, 0, 255);
  int offset = (L - R) * 2;
  return offset;
}
